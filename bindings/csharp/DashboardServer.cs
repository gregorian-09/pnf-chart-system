using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace PnF
{
    /// <summary>
    /// Localhost dashboard server for the C# binding.
    /// </summary>
    public sealed class DashboardServer : IDisposable
    {
        private readonly ConcurrentDictionary<Guid, WebSocket> _clients = new();
        private readonly JsonSerializerOptions _jsonOptions = new(JsonSerializerDefaults.Web);
        private long _sequence;
        private HttpListener? _listener;
        private CancellationTokenSource? _cts;
        private Task? _acceptLoop;
        private Timer? _publishTimer;
        private string _latestSnapshot;

        public DashboardServer()
        {
            _latestSnapshot = JsonSerializer.Serialize(BuildEmptySnapshot(), _jsonOptions);
        }

        public DashboardServer(Chart chart, Indicators indicators) : this()
        {
            Chart = chart;
            Indicators = indicators;
        }

        public Chart? Chart { get; private set; }
        public Indicators? Indicators { get; private set; }
        public string Host { get; private set; } = "127.0.0.1";
        public int Port { get; private set; }
        public bool IsRunning => _listener != null;

        public void SetChart(Chart chart) => Chart = chart;
        public void SetIndicators(Indicators indicators) => Indicators = indicators;

        public string Start(string host = "127.0.0.1", int port = 0)
        {
            if (_listener != null)
                return Url();

            Host = host;
            Port = port == 0 ? ReservePort() : port;
            _cts = new CancellationTokenSource();
            _listener = new HttpListener();
            _listener.Prefixes.Add($"http://{host}:{Port}/");
            _listener.Start();
            _acceptLoop = Task.Run(() => AcceptLoop(_cts.Token));
            return Url();
        }

        public string Url() => $"http://{Host}:{Port}/";

        public string SnapshotJson() => _latestSnapshot;

        public string Publish()
        {
            _latestSnapshot = JsonSerializer.Serialize(BuildSnapshot(Chart, Indicators, Interlocked.Increment(ref _sequence)), _jsonOptions);
            _ = BroadcastAsync(_latestSnapshot, CancellationToken.None);
            return _latestSnapshot;
        }

        public void StartAutoPublish(int intervalMs = 250)
        {
            StopAutoPublish();
            _publishTimer = new Timer(_ =>
            {
                if (IsRunning && Chart != null)
                    Publish();
            }, null, intervalMs, intervalMs);
        }

        public void StopAutoPublish()
        {
            _publishTimer?.Dispose();
            _publishTimer = null;
        }

        public void Stop()
        {
            StopAutoPublish();
            _cts?.Cancel();
            if (_listener != null)
            {
                _listener.Stop();
                _listener.Close();
                _listener = null;
            }
            foreach (var pair in _clients.ToArray())
            {
                try
                {
                    pair.Value.Abort();
                    pair.Value.Dispose();
                }
                catch
                {
                    // Ignore cleanup errors.
                }
                _clients.TryRemove(pair.Key, out _);
            }
        }

        public void Dispose()
        {
            Stop();
            _cts?.Dispose();
        }

        private async Task AcceptLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested && _listener != null)
            {
                HttpListenerContext? context = null;
                try
                {
                    context = await _listener.GetContextAsync().ConfigureAwait(false);
                }
                catch when (token.IsCancellationRequested || _listener == null)
                {
                    return;
                }
                catch
                {
                    if (_listener == null)
                        return;
                }

                if (context != null)
                    _ = Task.Run(() => HandleContextAsync(context, token), token);
            }
        }

        private async Task HandleContextAsync(HttpListenerContext context, CancellationToken token)
        {
            var path = context.Request.Url?.AbsolutePath ?? "/";

            if (path == "/ws" && context.Request.IsWebSocketRequest)
            {
                await HandleWebSocketAsync(context, token).ConfigureAwait(false);
                return;
            }

            if (path == "/healthz")
            {
                await WriteResponseAsync(context, 200, "text/plain; charset=utf-8", Encoding.UTF8.GetBytes("ok")).ConfigureAwait(false);
                return;
            }

            if (path == "/snapshot")
            {
                await WriteResponseAsync(context, 200, "application/json; charset=utf-8", Encoding.UTF8.GetBytes(_latestSnapshot)).ConfigureAwait(false);
                return;
            }

            if (path == "/app.js")
            {
                await WriteResponseAsync(context, 200, "application/javascript; charset=utf-8", File.ReadAllBytes(ResolveAsset("app.js"))).ConfigureAwait(false);
                return;
            }

            if (path == "/styles.css")
            {
                await WriteResponseAsync(context, 200, "text/css; charset=utf-8", File.ReadAllBytes(ResolveAsset("styles.css"))).ConfigureAwait(false);
                return;
            }

            if (path == "/" || path == "/index.html")
            {
                var html = File.ReadAllText(ResolveAsset("index.html"), Encoding.UTF8)
                    .Replace(
                        "window.PNF_DASHBOARD_CONFIG = window.PNF_DASHBOARD_CONFIG || {};",
                        $"window.PNF_DASHBOARD_CONFIG = {{ wsUrl: \"ws://{Host}:{Port}/ws\" }};");
                await WriteResponseAsync(context, 200, "text/html; charset=utf-8", Encoding.UTF8.GetBytes(html)).ConfigureAwait(false);
                return;
            }

            await WriteResponseAsync(context, 404, "text/plain; charset=utf-8", Encoding.UTF8.GetBytes("not found")).ConfigureAwait(false);
        }

        private async Task HandleWebSocketAsync(HttpListenerContext context, CancellationToken token)
        {
            HttpListenerWebSocketContext wsContext;
            try
            {
                wsContext = await context.AcceptWebSocketAsync(null).ConfigureAwait(false);
            }
            catch
            {
                context.Response.StatusCode = 500;
                context.Response.Close();
                return;
            }

            var id = Guid.NewGuid();
            var socket = wsContext.WebSocket;
            _clients[id] = socket;
            await SendTextAsync(socket, _latestSnapshot, token).ConfigureAwait(false);

            var buffer = new byte[1];
            try
            {
                while (!token.IsCancellationRequested && socket.State == WebSocketState.Open)
                {
                    var result = await socket.ReceiveAsync(new ArraySegment<byte>(buffer), token).ConfigureAwait(false);
                    if (result.MessageType == WebSocketMessageType.Close)
                        break;
                }
            }
            catch
            {
                // Ignore disconnects.
            }
            finally
            {
                _clients.TryRemove(id, out _);
                try
                {
                    if (socket.State == WebSocketState.Open || socket.State == WebSocketState.CloseReceived)
                        await socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "closing", CancellationToken.None).ConfigureAwait(false);
                }
                catch
                {
                    // Ignore cleanup errors.
                }
                socket.Dispose();
            }
        }

        private async Task BroadcastAsync(string message, CancellationToken token)
        {
            foreach (var pair in _clients.ToArray())
            {
                try
                {
                    await SendTextAsync(pair.Value, message, token).ConfigureAwait(false);
                }
                catch
                {
                    _clients.TryRemove(pair.Key, out _);
                    try { pair.Value.Dispose(); } catch { }
                }
            }
        }

        private static async Task SendTextAsync(WebSocket socket, string message, CancellationToken token)
        {
            if (socket.State != WebSocketState.Open)
                return;
            var data = Encoding.UTF8.GetBytes(message);
            await socket.SendAsync(new ArraySegment<byte>(data), WebSocketMessageType.Text, true, token).ConfigureAwait(false);
        }

        private static async Task WriteResponseAsync(HttpListenerContext context, int statusCode, string contentType, byte[] payload)
        {
            context.Response.StatusCode = statusCode;
            context.Response.ContentType = contentType;
            context.Response.Headers["Cache-Control"] = "no-store";
            context.Response.ContentLength64 = payload.Length;
            await context.Response.OutputStream.WriteAsync(payload, 0, payload.Length).ConfigureAwait(false);
            context.Response.Close();
        }

        private object BuildSnapshot(Chart? chart, Indicators? indicators, long sequence)
        {
            if (chart == null)
                return BuildEmptySnapshot();

            var columns = new List<object>();
            for (var i = 0; i < chart.ColumnCount; i++)
            {
                var boxes = new List<object>();
                var boxCount = chart.GetColumnBoxCount(i);
                for (var b = 0; b < boxCount; b++)
                {
                    boxes.Add(new
                    {
                        index = b,
                        price = chart.GetBoxPrice(i, b),
                        type = chart.GetBoxType(i, b).ToString(),
                        marker = chart.GetBoxMarker(i, b) ?? string.Empty
                    });
                }
                columns.Add(new
                {
                    index = i,
                    type = chart.GetColumnType(i).ToString(),
                    box_count = boxCount,
                    highest = chart.GetColumnHighest(i),
                    lowest = chart.GetColumnLowest(i),
                    boxes
                });
            }

            var signals = indicators != null
                ? indicators.GetSignals().Select(signal => (object)new
                {
                    type = signal.Type.ToString(),
                    column_index = signal.ColumnIndex,
                    price = signal.Price
                }).ToList()
                : new List<object>();

            var patterns = indicators != null
                ? indicators.GetPatterns().Select(pattern => (object)new
                {
                    type = pattern.Type.ToString(),
                    start_column = pattern.StartColumn,
                    end_column = pattern.EndColumn,
                    price = pattern.Price,
                    is_bullish = pattern.IsBullish
                }).ToList()
                : new List<object>();

            var supportLevels = indicators != null
                ? indicators.GetSupportLevels().Select(level => (object)new
                {
                    type = "Support",
                    price = level.Price,
                    touches = level.TouchCount
                }).ToList()
                : new List<object>();

            var resistanceLevels = indicators != null
                ? indicators.GetResistanceLevels().Select(level => (object)new
                {
                    type = "Resistance",
                    price = level.Price,
                    touches = level.TouchCount
                }).ToList()
                : new List<object>();

            return new
            {
                type = "dashboard.snapshot",
                version = 1,
                timestamp = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds(),
                sequence,
                payload = new
                {
                    meta = new
                    {
                        binding = "csharp",
                        library_version = Version.String,
                        server_version = "1",
                        stream_state = "running"
                    },
                    chart = new
                    {
                        box_size = chart.BoxSize,
                        column_count = chart.ColumnCount,
                        x_column_count = chart.XColumnCount,
                        o_column_count = chart.OColumnCount,
                        has_bullish_bias = chart.HasBullishBias,
                        has_bearish_bias = chart.HasBearishBias,
                        columns
                    },
                    indicators = new
                    {
                        summary = indicators?.Summary() ?? string.Empty,
                        detailed_summary = indicators?.ToString() ?? string.Empty,
                        bullish_percent = indicators?.BullishPercent ?? 0.0,
                        signal_count = indicators?.SignalCount ?? 0,
                        buy_signal_count = indicators?.BuySignalCount ?? 0,
                        sell_signal_count = indicators?.SellSignalCount ?? 0,
                        pattern_count = indicators?.PatternCount ?? 0,
                        support_level_count = indicators?.SupportLevelCount ?? 0,
                        resistance_level_count = indicators?.ResistanceLevelCount ?? 0,
                        congestion_zone_count = indicators?.CongestionZoneCount ?? 0,
                        signals = signals,
                        patterns = patterns,
                        support_levels = supportLevels,
                        resistance_levels = resistanceLevels,
                        price_objectives = Array.Empty<object>(),
                        congestion_zones = Array.Empty<object>(),
                        series = new
                        {
                            sma_short = indicators?.SmaShortValues() ?? Array.Empty<double>(),
                            sma_medium = indicators?.SmaMediumValues() ?? Array.Empty<double>(),
                            sma_long = indicators?.SmaLongValues() ?? Array.Empty<double>(),
                            bollinger_middle = indicators?.BollingerMiddleValues() ?? Array.Empty<double>(),
                            bollinger_upper = indicators?.BollingerUpperValues() ?? Array.Empty<double>(),
                            bollinger_lower = indicators?.BollingerLowerValues() ?? Array.Empty<double>(),
                            rsi = indicators?.RsiValues() ?? Array.Empty<double>()
                        }
                    }
                }
            };
        }

        private object BuildEmptySnapshot()
        {
            return new
            {
                type = "dashboard.snapshot",
                version = 1,
                timestamp = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds(),
                sequence = 0,
                payload = new
                {
                    meta = new
                    {
                        binding = "csharp",
                        library_version = Version.String,
                        server_version = "1",
                        stream_state = "idle"
                    },
                    chart = new
                    {
                        box_size = 0,
                        column_count = 0,
                        x_column_count = 0,
                        o_column_count = 0,
                        has_bullish_bias = false,
                        has_bearish_bias = false,
                        columns = Array.Empty<object>()
                    },
                    indicators = new
                    {
                        summary = string.Empty,
                        detailed_summary = string.Empty,
                        bullish_percent = 0,
                        signal_count = 0,
                        buy_signal_count = 0,
                        sell_signal_count = 0,
                        pattern_count = 0,
                        support_level_count = 0,
                        resistance_level_count = 0,
                        congestion_zone_count = 0,
                        signals = Array.Empty<object>(),
                        patterns = Array.Empty<object>(),
                        support_levels = Array.Empty<object>(),
                        resistance_levels = Array.Empty<object>(),
                        price_objectives = Array.Empty<object>(),
                        congestion_zones = Array.Empty<object>(),
                        series = new
                        {
                            sma_short = Array.Empty<double>(),
                            sma_medium = Array.Empty<double>(),
                            sma_long = Array.Empty<double>(),
                            bollinger_middle = Array.Empty<double>(),
                            bollinger_upper = Array.Empty<double>(),
                            bollinger_lower = Array.Empty<double>(),
                            rsi = Array.Empty<double>()
                        }
                    }
                }
            };
        }

        private static int ReservePort()
        {
            var listener = new TcpListener(IPAddress.Loopback, 0);
            listener.Start();
            var port = ((IPEndPoint)listener.LocalEndpoint).Port;
            listener.Stop();
            return port;
        }

        private static string ResolveAsset(string fileName)
        {
            var current = Directory.GetCurrentDirectory();
            while (!string.IsNullOrEmpty(current))
            {
                var candidate = Path.Combine(current, "dashboard", "web", fileName);
                if (File.Exists(candidate))
                    return candidate;
                current = Directory.GetParent(current)?.FullName ?? string.Empty;
            }

            throw new FileNotFoundException($"Dashboard asset not found: {fileName}");
        }
    }
}
