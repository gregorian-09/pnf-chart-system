# pypnf

`pypnf` is the official Python binding for the PnF (Point and Figure) chart engine.

It provides:
- High-performance native chart construction (via C++ core).
- Technical indicators (SMA, Bollinger Bands, RSI, OBV, signals, patterns, support/resistance, congestion).
- A lightweight real-time localhost dashboard server (`pypnf_dashboard`).

## Installation

```bash
pip install pypnf-gregorian09
```

## Quick Start

```python
from datetime import datetime
import pypnf

cfg = pypnf.ChartConfig()
cfg.method = pypnf.ConstructionMethod.HighLow
cfg.box_size_method = pypnf.BoxSizeMethod.Traditional
cfg.reversal = 3

chart = pypnf.Chart(cfg)
chart.add_data(5000.0, 4950.0, 4985.0, datetime.utcnow())
chart.add_data(5030.0, 4995.0, 5020.0, datetime.utcnow())

ind = pypnf.Indicators()
ind.calculate(chart)

print(chart.to_ascii())
print(ind.summary())
```

## Real-Time Dashboard

```python
from pypnf_dashboard import DashboardServer

server = DashboardServer(chart, ind)
server.start("127.0.0.1", 8761)
server.publish()
print(server.url())  # http://127.0.0.1:8761/
```

## Main API Surface

- Core:
  - `Chart`, `ChartConfig`
  - `Box`, `Column`
- Indicators:
  - `Indicators`, `IndicatorConfig`
  - `MovingAverage`, `BollingerBands`, `RSI`, `OnBalanceVolume`
  - `SignalDetector`, `PatternRecognizer`, `SupportResistance`, `CongestionDetector`
- Data and enums:
  - `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`, `CongestionZone`
  - `BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `SignalType`, `PatternType`
- Utilities:
  - `version()`, `version_major()`, `version_minor()`, `version_patch()`

Full API reference:
- `docs/bindings/python.md`
- `docs/reference/api-symbol-index.md`

## Data Notes

- Timestamps are Unix or datetime values depending on API overload.
- Prices are floating-point values.
- For high/low construction mode, pass real high/low candle data for best results.

## Troubleshooting

- Linux: ensure native shared libraries can be found at runtime.
- If building from source, install CMake and a C++20-capable toolchain.
- Recreate environment after upgrading package major versions.

## Links

- Source: https://github.com/gregorian-09/pnf-chart-system
- Issue tracker: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
- Documentation: https://github.com/gregorian-09/pnf-chart-system/tree/master/docs
