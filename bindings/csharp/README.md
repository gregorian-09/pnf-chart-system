# PnF.ChartSystem

[![NuGet Version](https://img.shields.io/nuget/v/PnF.ChartSystem.svg)](https://www.nuget.org/packages/PnF.ChartSystem)
[![NuGet Downloads](https://img.shields.io/nuget/dt/PnF.ChartSystem.svg)](https://www.nuget.org/packages/PnF.ChartSystem)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/gregorian-09/pnf-chart-system/blob/master/LICENSE)

`.NET 8` binding for the Point-and-Figure (PnF) engine, including charting, indicators, structure analysis, and real-time dashboard streaming.

## What This Package Covers

| Area | Coverage |
| --- | --- |
| Chart Engine | Point-and-Figure column/box construction |
| Trend Context | Bullish and bearish bias checks |
| Indicators | SMA, Bollinger Bands, RSI, OBV, Bullish Percent |
| Signals and Patterns | Buy/sell signals and named PnF patterns |
| Market Structure | Support/resistance and congestion analytics |
| Visualization | Localhost dashboard server with live snapshots |

## Install

```bash
dotnet add package PnF.ChartSystem
```

## Quick Start

```csharp
using PnF;

var chartConfig = ChartConfig.Default();
chartConfig.Method = ConstructionMethod.HighLow;
chartConfig.BoxSizeMethod = BoxSizeMethod.Traditional;
chartConfig.Reversal = 3;

using var chart = new Chart(chartConfig);
chart.AddData(5000.0, 4950.0, 4985.0, 1700000000);
chart.AddData(5040.0, 4980.0, 5030.0, 1700003600);
chart.AddData(5065.0, 5010.0, 5055.0, 1700007200);

using var indicators = new Indicators(IndicatorConfig.Default());
indicators.Calculate(chart);

Console.WriteLine(chart.ToAscii());
Console.WriteLine(indicators.Summary());
```

## Trendline and Bias Checks

```csharp
double lastPrice = 5055.0;

Console.WriteLine($"Bullish bias: {chart.HasBullishBias}");
Console.WriteLine($"Bearish bias: {chart.HasBearishBias}");
Console.WriteLine($"Above bullish support: {chart.IsAboveBullishSupport(lastPrice)}");
Console.WriteLine($"Below bearish resistance: {chart.IsBelowBearishResistance(lastPrice)}");
```

## Indicators and Momentum

```csharp
int lastColumn = Math.Max(0, chart.ColumnCount - 1);

Console.WriteLine($"SMA short: {indicators.SmaShort(lastColumn)}");
Console.WriteLine($"SMA medium: {indicators.SmaMedium(lastColumn)}");
Console.WriteLine($"SMA long: {indicators.SmaLong(lastColumn)}");

Console.WriteLine($"Bollinger upper: {indicators.BollingerUpper(lastColumn)}");
Console.WriteLine($"RSI: {indicators.Rsi(lastColumn)}");
Console.WriteLine($"OBV: {indicators.Obv(lastColumn)}");
Console.WriteLine($"Bullish percent: {indicators.BullishPercent}");
```

## Signals and Patterns

```csharp
Console.WriteLine($"Current signal: {indicators.CurrentSignal}");
Console.WriteLine($"Signal count: {indicators.SignalCount}");
Console.WriteLine($"Buy signals: {indicators.BuySignalCount}");
Console.WriteLine($"Sell signals: {indicators.SellSignalCount}");

Console.WriteLine($"Pattern count: {indicators.PatternCount}");
Console.WriteLine($"Bullish patterns: {indicators.BullishPatternCount}");
Console.WriteLine($"Bearish patterns: {indicators.BearishPatternCount}");

foreach (var pattern in indicators.GetPatterns())
{
    Console.WriteLine($"{pattern.Type} {pattern.StartColumn}->{pattern.EndColumn} @ {pattern.Price:F2}");
}
```

## Support, Resistance, and Congestion

```csharp
var support = indicators.GetSupportLevels();
var resistance = indicators.GetResistanceLevels();

Console.WriteLine($"Support levels: {support.Count}");
Console.WriteLine($"Resistance levels: {resistance.Count}");
Console.WriteLine($"Near support: {indicators.IsNearSupport(5050.0, 1.0)}");
Console.WriteLine($"Near resistance: {indicators.IsNearResistance(5050.0, 1.0)}");
Console.WriteLine($"Congestion zones: {indicators.CongestionZoneCount}");
```

## Real-Time Dashboard

```csharp
using var server = new DashboardServer(chart, indicators);
server.Start("127.0.0.1", 8764);
server.Publish();
Console.WriteLine(server.Url());
```

## API Overview

Core:
- `Chart`, `ChartConfig`

Indicators:
- `Indicators`, `IndicatorConfig`
- SMA/Bollinger/RSI/OBV methods and vector accessors
- signal/pattern/support-resistance/congestion methods

Data models:
- `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`

Dashboard:
- `DashboardServer`

Version:
- `Version.String`, `Version.Major`, `Version.Minor`, `Version.Patch`, `Version.Full`

## Native Runtime Notes

- Package relies on native `pnf` binaries.
- Ensure native assets are present for your runtime identifier.
- Validate native load in CI before publishing production builds.

## Versioning

- NuGet package version follows the core engine release.
- Keep versions synchronized across all bindings in mixed-language deployments.
- Review `CHANGELOG.md` for behavior changes.

## Troubleshooting

- `DllNotFoundException`: missing native asset or runtime mismatch.
- Empty indicator vectors: insufficient chart history for configured lookbacks.
- Cross-service mismatch: compare config and package versions first.

## Documentation and Links

- C# binding reference: `docs/bindings/csharp.md`
- API symbol index: `docs/reference/api-symbol-index.md`
- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
