package com.pnf;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Base64;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Localhost dashboard server for the Java binding.
 */
public class DashboardServer implements AutoCloseable {
    private static final String WS_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    private static final String LIBRARY_VERSION = "0.1.0";

    private final AtomicLong sequence = new AtomicLong();
    private final List<Socket> clients = new CopyOnWriteArrayList<>();
    private final AtomicBoolean running = new AtomicBoolean(false);

    private Chart chart;
    private Indicators indicators;
    private HttpServer httpServer;
    private ExecutorService httpExecutor;
    private Thread wsThread;
    private ServerSocket wsServer;
    private ScheduledExecutorService publisher;
    private String latestSnapshot = emptySnapshot();
    private String host = "127.0.0.1";
    private int port;
    private int wsPort;

    /**
     * Creates a dashboard server with no attached chart or indicators.
     */
    public DashboardServer() {}

    /**
     * Creates a dashboard server bound to the provided chart state.
     *
     * @param chart chart instance used for snapshot generation
     * @param indicators indicator instance used for snapshot generation
     */
    public DashboardServer(final Chart chart, final Indicators indicators) {
        this.chart = chart;
        this.indicators = indicators;
    }

    /**
     * Replaces the chart instance used for generated snapshots.
     *
     * @param chart chart instance to expose through the dashboard
     */
    public void setChart(final Chart chart) {
        this.chart = chart;
    }

    /**
     * Replaces the indicators instance used for generated snapshots.
     *
     * @param indicators indicators instance to expose through the dashboard
     */
    public void setIndicators(final Indicators indicators) {
        this.indicators = indicators;
    }

    /**
     * Starts the dashboard server on localhost using an ephemeral port.
     *
     * @return base dashboard URL
     * @throws IOException if the HTTP or WebSocket server cannot be started
     */
    public String start() throws IOException {
        return start("127.0.0.1", 0);
    }

    /**
     * Starts the dashboard server.
     *
     * @param host host interface to bind to
     * @param port HTTP port to bind to, or {@code 0} for an ephemeral port
     * @return base dashboard URL
     * @throws IOException if the HTTP or WebSocket server cannot be started
     */
    public String start(final String host, final int port) throws IOException {
        if (httpServer != null) {
            return url();
        }
        this.host = host;
        httpServer = HttpServer.create(new InetSocketAddress(host, port), 0);
        this.port = httpServer.getAddress().getPort();
        wsServer = new ServerSocket(0, 50, java.net.InetAddress.getByName(host));
        wsPort = wsServer.getLocalPort();
        running.set(true);

        httpServer.createContext("/", this::handleIndex);
        httpServer.createContext("/index.html", this::handleIndex);
        httpServer.createContext("/app.js", exchange -> serveFile(exchange, "app.js", "application/javascript; charset=utf-8"));
        httpServer.createContext("/styles.css", exchange -> serveFile(exchange, "styles.css", "text/css; charset=utf-8"));
        httpServer.createContext("/healthz", exchange -> send(exchange, 200, "text/plain; charset=utf-8", "ok".getBytes(StandardCharsets.UTF_8)));
        httpServer.createContext("/snapshot", exchange -> send(exchange, 200, "application/json; charset=utf-8", latestSnapshot.getBytes(StandardCharsets.UTF_8)));
        httpExecutor = Executors.newCachedThreadPool();
        httpServer.setExecutor(httpExecutor);
        httpServer.start();

        wsThread = new Thread(this::acceptWsLoop, "pnf-java-dashboard-ws");
        wsThread.setDaemon(true);
        wsThread.start();
        return url();
    }

    /**
     * Returns the base dashboard URL.
     *
     * @return dashboard URL
     */
    public String url() {
        return "http://" + host + ":" + port + "/";
    }

    /**
     * Returns the most recently generated snapshot JSON.
     *
     * @return latest snapshot payload
     */
    public String snapshotJson() {
        return latestSnapshot;
    }

    /**
     * Generates and broadcasts a fresh snapshot to all connected clients.
     *
     * @return generated snapshot JSON
     */
    public String publish() {
        latestSnapshot = buildSnapshotJson(chart, indicators, sequence.incrementAndGet());
        broadcast(latestSnapshot);
        return latestSnapshot;
    }

    /**
     * Starts automatic snapshot publishing using the default interval.
     */
    public void startAutoPublish() {
        startAutoPublish(250);
    }

    /**
     * Starts automatic snapshot publishing on a fixed interval.
     *
     * @param intervalMs publish interval in milliseconds
     */
    public void startAutoPublish(final long intervalMs) {
        stopAutoPublish();
        publisher = Executors.newSingleThreadScheduledExecutor();
        publisher.scheduleAtFixedRate(() -> {
            if (running.get() && chart != null) {
                publish();
            }
        }, intervalMs, intervalMs, TimeUnit.MILLISECONDS);
    }

    /**
     * Stops automatic snapshot publishing if it is running.
     */
    public void stopAutoPublish() {
        if (publisher != null) {
            publisher.shutdownNow();
            publisher = null;
        }
    }

    /**
     * Stops the dashboard server and closes all connected clients.
     */
    public void stop() {
        stopAutoPublish();
        running.set(false);
        if (httpServer != null) {
            httpServer.stop(0);
            httpServer = null;
        }
        if (httpExecutor != null) {
            httpExecutor.shutdownNow();
            httpExecutor = null;
        }
        if (wsServer != null) {
            try {
                wsServer.close();
            } catch (IOException ignored) {
            }
            wsServer = null;
        }
        for (Socket socket : new ArrayList<>(clients)) {
            try {
                socket.close();
            } catch (IOException ignored) {
            }
        }
        clients.clear();
    }

    @Override
    public void close() {
        stop();
    }

    /**
     * Builds a dashboard snapshot JSON document from the current binding state.
     *
     * @param chart chart instance to serialize
     * @param indicators indicators instance to serialize
     * @param sequence snapshot sequence number
     * @return serialized dashboard snapshot JSON
     */
    public static String buildSnapshotJson(final Chart chart, final Indicators indicators, final long sequence) {
        if (chart == null) {
            return emptySnapshot();
        }
        final StringBuilder columns = new StringBuilder();
        columns.append('[');
        for (int i = 0; i < chart.columnCount(); i++) {
            if (i > 0) {
                columns.append(',');
            }
            final StringBuilder boxes = new StringBuilder();
            boxes.append('[');
            final int boxCount = chart.columnBoxCount(i);
            for (int b = 0; b < boxCount; b++) {
                if (b > 0) {
                    boxes.append(',');
                }
                boxes.append('{')
                        .append("\"index\":").append(b).append(',')
                        .append("\"price\":").append(chart.boxPrice(i, b)).append(',')
                        .append("\"type\":\"").append(escape(chart.boxType(i, b).name())).append("\",")
                        .append("\"marker\":\"").append(escape(chart.boxMarker(i, b))).append("\"")
                        .append('}');
            }
            boxes.append(']');
            columns.append('{')
                    .append("\"index\":").append(i).append(',')
                    .append("\"type\":\"").append(escape(chart.columnType(i).name())).append("\",")
                    .append("\"box_count\":").append(boxCount).append(',')
                    .append("\"highest\":").append(chart.columnHighest(i)).append(',')
                    .append("\"lowest\":").append(chart.columnLowest(i)).append(',')
                    .append("\"boxes\":").append(boxes)
                    .append('}');
        }
        columns.append(']');

        final String summary = indicators != null ? indicators.summary() : "";
        final String detailedSummary = indicators != null ? indicators.toString() : "";
        final double bullishPercent = indicators != null ? indicators.bullishPercent() : 0.0;
        final int signalCount = indicators != null ? indicators.signalCount() : 0;
        final int buySignalCount = indicators != null ? indicators.buySignalCount() : 0;
        final int sellSignalCount = indicators != null ? indicators.sellSignalCount() : 0;
        final int patternCount = indicators != null ? indicators.patternCount() : 0;
        final int supportLevelCount = indicators != null ? indicators.supportLevelCount() : 0;
        final int resistanceLevelCount = indicators != null ? indicators.resistanceLevelCount() : 0;
        final int congestionZoneCount = indicators != null ? indicators.congestionZoneCount() : 0;
        final String signalsJson = signalsJson(indicators);
        final String patternsJson = patternsJson(indicators);
        final String supportLevelsJson = supportLevelsJson(indicators != null ? indicators.supportLevels() : null);
        final String resistanceLevelsJson = supportLevelsJson(indicators != null ? indicators.resistanceLevels() : null);
        final String priceObjectivesJson = priceObjectivesJson(indicators);
        final String congestionZonesJson = congestionZonesJson(indicators);
        final String seriesJson = seriesJson(indicators);

        return "{" +
                "\"type\":\"dashboard.snapshot\"," +
                "\"version\":1," +
                "\"timestamp\":" + Instant.now().toEpochMilli() + ',' +
                "\"sequence\":" + sequence + ',' +
                "\"payload\":{" +
                "\"meta\":{" +
                "\"binding\":\"java\"," +
                "\"library_version\":\"" + escape(LIBRARY_VERSION) + "\"," +
                "\"server_version\":\"1\"," +
                "\"stream_state\":\"running\"}," +
                "\"chart\":{" +
                "\"box_size\":" + chart.boxSize() + ',' +
                "\"column_count\":" + chart.columnCount() + ',' +
                "\"x_column_count\":" + chart.xColumnCount() + ',' +
                "\"o_column_count\":" + chart.oColumnCount() + ',' +
                "\"has_bullish_bias\":" + chart.hasBullishBias() + ',' +
                "\"has_bearish_bias\":" + chart.hasBearishBias() + ',' +
                "\"columns\":" + columns + "}," +
                "\"indicators\":{" +
                "\"summary\":\"" + escape(summary) + "\"," +
                "\"detailed_summary\":\"" + escape(detailedSummary) + "\"," +
                "\"bullish_percent\":" + bullishPercent + ',' +
                "\"signal_count\":" + signalCount + ',' +
                "\"buy_signal_count\":" + buySignalCount + ',' +
                "\"sell_signal_count\":" + sellSignalCount + ',' +
                "\"pattern_count\":" + patternCount + ',' +
                "\"support_level_count\":" + supportLevelCount + ',' +
                "\"resistance_level_count\":" + resistanceLevelCount + ',' +
                "\"congestion_zone_count\":" + congestionZoneCount + ',' +
                "\"signals\":" + signalsJson + ',' +
                "\"patterns\":" + patternsJson + ',' +
                "\"support_levels\":" + supportLevelsJson + ',' +
                "\"resistance_levels\":" + resistanceLevelsJson + ',' +
                "\"price_objectives\":" + priceObjectivesJson + ',' +
                "\"congestion_zones\":" + congestionZonesJson + ',' +
                "\"series\":" + seriesJson +
                "}}}";
    }

    private static String emptySnapshot() {
        return "{" +
                "\"type\":\"dashboard.snapshot\"," +
                "\"version\":1," +
                "\"timestamp\":" + Instant.now().toEpochMilli() + ',' +
                "\"sequence\":0," +
                "\"payload\":{" +
                "\"meta\":{\"binding\":\"java\",\"library_version\":\"" + escape(LIBRARY_VERSION) + "\",\"server_version\":\"1\",\"stream_state\":\"idle\"}," +
                "\"chart\":{\"box_size\":0,\"column_count\":0,\"x_column_count\":0,\"o_column_count\":0,\"has_bullish_bias\":false,\"has_bearish_bias\":false,\"columns\":[]}," +
                "\"indicators\":{\"summary\":\"\",\"detailed_summary\":\"\",\"bullish_percent\":0,\"signal_count\":0,\"buy_signal_count\":0,\"sell_signal_count\":0,\"pattern_count\":0,\"support_level_count\":0,\"resistance_level_count\":0,\"congestion_zone_count\":0,\"signals\":[],\"patterns\":[],\"support_levels\":[],\"resistance_levels\":[],\"price_objectives\":[],\"congestion_zones\":[],\"series\":{\"sma_short\":[],\"sma_medium\":[],\"sma_long\":[],\"bollinger_middle\":[],\"bollinger_upper\":[],\"bollinger_lower\":[],\"rsi\":[]}}}}}";
    }

    private static String seriesJson(final Indicators indicators) {
        if (indicators == null) {
            return "{\"sma_short\":[],\"sma_medium\":[],\"sma_long\":[],\"bollinger_middle\":[],\"bollinger_upper\":[],\"bollinger_lower\":[],\"rsi\":[]}";
        }
        return '{' +
                "\"sma_short\":" + doubleArrayJson(indicators.smaShortValues()) + ',' +
                "\"sma_medium\":" + doubleArrayJson(indicators.smaMediumValues()) + ',' +
                "\"sma_long\":" + doubleArrayJson(indicators.smaLongValues()) + ',' +
                "\"bollinger_middle\":" + doubleArrayJson(indicators.bollingerMiddleValues()) + ',' +
                "\"bollinger_upper\":" + doubleArrayJson(indicators.bollingerUpperValues()) + ',' +
                "\"bollinger_lower\":" + doubleArrayJson(indicators.bollingerLowerValues()) + ',' +
                "\"rsi\":" + doubleArrayJson(indicators.rsiValues()) +
                '}';
    }

    private static String doubleArrayJson(final double[] values) {
        if (values == null || values.length == 0) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        for (int i = 0; i < values.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            json.append(values[i]);
        }
        json.append(']');
        return json.toString();
    }

    private static String signalsJson(final Indicators indicators) {
        if (indicators == null) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        final Signal[] signals = indicators.signals();
        for (int i = 0; i < signals.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            final Signal signal = signals[i];
            json.append('{')
                    .append("\"type\":\"").append(escape(signal.type().name())).append("\",")
                    .append("\"column_index\":").append(signal.columnIndex()).append(',')
                    .append("\"price\":").append(signal.price())
                    .append('}');
        }
        json.append(']');
        return json.toString();
    }

    private static String patternsJson(final Indicators indicators) {
        if (indicators == null) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        final Pattern[] patterns = indicators.patterns();
        for (int i = 0; i < patterns.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            final Pattern pattern = patterns[i];
            json.append('{')
                    .append("\"type\":\"").append(escape(pattern.type().name())).append("\",")
                    .append("\"start_column\":").append(pattern.columnIndex()).append(',')
                    .append("\"end_column\":").append(pattern.columnIndex()).append(',')
                    .append("\"price\":").append(pattern.price()).append(',')
                    .append("\"is_bullish\":").append(pattern.isBullish())
                    .append('}');
        }
        json.append(']');
        return json.toString();
    }

    private static String supportLevelsJson(final SupportResistanceLevel[] levels) {
        if (levels == null) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        for (int i = 0; i < levels.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            final SupportResistanceLevel level = levels[i];
            json.append('{')
                    .append("\"type\":\"").append(escape(level.type().name())).append("\",")
                    .append("\"price\":").append(level.price()).append(',')
                    .append("\"touches\":").append(level.touches()).append(',')
                    .append("\"first_column\":").append(level.firstColumn()).append(',')
                    .append("\"last_column\":").append(level.lastColumn())
                    .append('}');
        }
        json.append(']');
        return json.toString();
    }

    private static String priceObjectivesJson(final Indicators indicators) {
        if (indicators == null) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        final PriceObjective[] objectives = indicators.priceObjectives();
        for (int i = 0; i < objectives.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            final PriceObjective objective = objectives[i];
            json.append('{')
                    .append("\"target\":").append(objective.target()).append(',')
                    .append("\"column_index\":").append(objective.columnIndex()).append(',')
                    .append("\"is_bullish\":").append(objective.isBullish())
                    .append('}');
        }
        json.append(']');
        return json.toString();
    }

    private static String congestionZonesJson(final Indicators indicators) {
        if (indicators == null) {
            return "[]";
        }
        final StringBuilder json = new StringBuilder("[");
        final CongestionZone[] zones = indicators.congestionZones();
        for (int i = 0; i < zones.length; i++) {
            if (i > 0) {
                json.append(',');
            }
            final CongestionZone zone = zones[i];
            json.append('{')
                    .append("\"start_column\":").append(zone.startColumn()).append(',')
                    .append("\"end_column\":").append(zone.endColumn()).append(',')
                    .append("\"high_price\":").append(zone.highPrice()).append(',')
                    .append("\"low_price\":").append(zone.lowPrice()).append(',')
                    .append("\"column_count\":").append(zone.columnCount())
                    .append('}');
        }
        json.append(']');
        return json.toString();
    }

    private static String escape(final String value) {
        return Objects.toString(value, "")
                .replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    private void handleIndex(final HttpExchange exchange) throws IOException {
        String html = Files.readString(resolveAsset("index.html"), StandardCharsets.UTF_8);
        final String wsUrl = "ws://" + requestHost(exchange) + ":" + wsPort + "/ws";
        html = html.replace(
                "window.PNF_DASHBOARD_CONFIG = window.PNF_DASHBOARD_CONFIG || {};",
                "window.PNF_DASHBOARD_CONFIG = { wsUrl: \"" + escape(wsUrl) + "\" };"
        );
        send(exchange, 200, "text/html; charset=utf-8", html.getBytes(StandardCharsets.UTF_8));
    }

    private String requestHost(final HttpExchange exchange) {
        final String hostHeader = exchange.getRequestHeaders().getFirst("Host");
        if (hostHeader != null && !hostHeader.isBlank()) {
            final String trimmed = hostHeader.trim();
            if (trimmed.startsWith("[")) {
                final int closing = trimmed.indexOf(']');
                if (closing > 0) {
                    return trimmed.substring(0, closing + 1);
                }
            }
            final int colon = trimmed.indexOf(':');
            return colon > 0 ? trimmed.substring(0, colon) : trimmed;
        }
        if ("0.0.0.0".equals(host) || "::".equals(host)) {
            return "127.0.0.1";
        }
        return host;
    }

    private void serveFile(final HttpExchange exchange, final String name, final String contentType) throws IOException {
        send(exchange, 200, contentType, Files.readAllBytes(resolveAsset(name)));
    }

    private void send(final HttpExchange exchange, final int status, final String contentType, final byte[] body) throws IOException {
        exchange.getResponseHeaders().add("Content-Type", contentType);
        exchange.getResponseHeaders().add("Cache-Control", "no-store");
        exchange.sendResponseHeaders(status, body.length);
        try (OutputStream os = exchange.getResponseBody()) {
            os.write(body);
        }
    }

    private void acceptWsLoop() {
        while (running.get()) {
            try {
                final Socket socket = wsServer.accept();
                final Thread thread = new Thread(() -> handleWsClient(socket), "pnf-java-dashboard-client");
                thread.setDaemon(true);
                thread.start();
            } catch (IOException ignored) {
                if (!running.get()) {
                    return;
                }
            }
        }
    }

    private void handleWsClient(final Socket socket) {
        try (socket) {
            final BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            String line;
            String key = null;
            while ((line = reader.readLine()) != null && !line.isEmpty()) {
                final String lower = line.toLowerCase();
                if (lower.startsWith("sec-websocket-key:")) {
                    key = line.substring(line.indexOf(':') + 1).trim();
                }
            }
            if (key == null) {
                return;
            }
            final String accept = Base64.getEncoder().encodeToString(
                    MessageDigest.getInstance("SHA-1").digest((key + WS_GUID).getBytes(StandardCharsets.UTF_8))
            );
            socket.getOutputStream().write((
                    "HTTP/1.1 101 Switching Protocols\r\n" +
                    "Upgrade: websocket\r\n" +
                    "Connection: Upgrade\r\n" +
                    "Sec-WebSocket-Accept: " + accept + "\r\n\r\n"
            ).getBytes(StandardCharsets.UTF_8));
            clients.add(socket);
            sendWsText(socket, latestSnapshot);
            while (running.get() && !socket.isClosed()) {
                try {
                    Thread.sleep(1000L);
                } catch (InterruptedException ignored) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        } catch (Exception ignored) {
        } finally {
            clients.remove(socket);
        }
    }

    private void broadcast(final String message) {
        for (Socket socket : new ArrayList<>(clients)) {
            try {
                sendWsText(socket, message);
            } catch (IOException e) {
                clients.remove(socket);
                try {
                    socket.close();
                } catch (IOException ignored) {
                }
            }
        }
    }

    private static void sendWsText(final Socket socket, final String message) throws IOException {
        final byte[] payload = message.getBytes(StandardCharsets.UTF_8);
        final OutputStream os = socket.getOutputStream();
        os.write(0x81);
        if (payload.length < 126) {
            os.write(payload.length);
        } else if (payload.length < 65536) {
            os.write(126);
            os.write((payload.length >> 8) & 0xFF);
            os.write(payload.length & 0xFF);
        } else {
            os.write(127);
            for (int shift = 56; shift >= 0; shift -= 8) {
                os.write((payload.length >> shift) & 0xFF);
            }
        }
        os.write(payload);
        os.flush();
    }

    private static Path resolveAsset(final String name) throws IOException {
        Path current = Path.of("").toAbsolutePath();
        while (current != null) {
            final Path candidate = current.resolve("dashboard").resolve("web").resolve(name);
            if (Files.exists(candidate)) {
                return candidate;
            }
            current = current.getParent();
        }
        throw new IOException("Dashboard asset not found: " + name);
    }
}
