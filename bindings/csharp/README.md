# GregorianRayne.PnF

`GregorianRayne.PnF` is the official .NET binding for the PnF (Point and Figure) chart engine.

It includes:
- Fast chart construction through native interop.
- Full indicator stack (SMA, Bollinger Bands, RSI, OBV, signals, patterns, support/resistance, congestion).
- Real-time localhost dashboard integration.

## Installation

```bash
dotnet add package GregorianRayne.PnF
```

## Quick Start

```csharp
using PnF;

var chart = new Chart();
chart.AddPrice(5000.0, DateTimeOffset.UtcNow.ToUnixTimeSeconds());
chart.AddPrice(5030.0, DateTimeOffset.UtcNow.ToUnixTimeSeconds() + 1);

var indicators = new Indicators();
indicators.Calculate(chart);

Console.WriteLine(chart.ToAscii());
Console.WriteLine(indicators.Summary());
```

## Real-Time Dashboard

```csharp
var server = new DashboardServer(chart, indicators);
server.Start("127.0.0.1", 8764);
server.Publish();
Console.WriteLine(server.Url());
```

## API Overview

- Core:
  - `Chart`
  - `ChartConfig`
- Indicators:
  - `Indicators`
  - `IndicatorConfig`
- Data:
  - `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`
- Dashboard:
  - `DashboardServer`
- Version:
  - `Version.String`, `Version.Major`, `Version.Minor`, `Version.Patch`

## Native Runtime Notes

- The package relies on native binaries (`pnf_native`).
- Ensure deployment includes native assets for your OS/architecture.
- In CI/CD, validate native loading with at least one smoke test before publish.

## Documentation

- C# reference: `docs/bindings/csharp.md`
- Full API symbol index: `docs/reference/api-symbol-index.md`
- Project docs home: `docs/index.md`

## Troubleshooting

- `DllNotFoundException`: missing native runtime asset or incorrect runtime identifier.
- Mismatch errors: ensure package version aligns with the same core version.

## Links

- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
