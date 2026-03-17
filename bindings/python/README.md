# pypnf

[![PyPI version](https://img.shields.io/pypi/v/pypnf.svg)](https://pypi.org/project/pypnf/)
[![Python versions](https://img.shields.io/pypi/pyversions/pypnf.svg)](https://pypi.org/project/pypnf/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/gregorian-09/pnf-chart-system/blob/master/LICENSE)

Production-ready Python bindings for the PnF (Point and Figure) engine.

Package name is `pypnf`; import name is `pypnf`.

## Why This Package

`pypnf` is built for real analysis workflows, not only chart construction:

| Area | What you get |
| --- | --- |
| Chart Engine | Point-and-Figure charting with `Close` and `HighLow` construction |
| Trend Context | Bullish support / bearish resistance context checks |
| Indicators | SMA, Bollinger Bands, RSI, OBV, Bullish Percent |
| Structural Signals | Buy/sell signals and full PnF pattern detection |
| Market Structure | Support/resistance levels, price objectives, congestion zones |
| Visualization | Localhost real-time dashboard streaming |

## Installation

```bash
pip install pypnf
```

## Quick Start

```python
import pypnf

cfg = pypnf.ChartConfig()
cfg.method = pypnf.ConstructionMethod.HighLow
cfg.box_size_method = pypnf.BoxSizeMethod.Traditional
cfg.box_size = 0.0
cfg.reversal = 3

chart = pypnf.Chart(cfg)

# high, low, close, timestamp
chart.add_data(5000.0, 4950.0, 4985.0, 1700000000)
chart.add_data(5040.0, 4980.0, 5030.0, 1700003600)
chart.add_data(5065.0, 5010.0, 5055.0, 1700007200)

indicators = pypnf.Indicators(pypnf.IndicatorConfig())
indicators.calculate(chart)

print(chart.to_ascii())
print(indicators.summary())
```

## Trendline and Bias Workflow

```python
last_price = 5055.0

print("Bullish bias:", chart.has_bullish_bias())
print("Bearish bias:", chart.has_bearish_bias())
print("Above bullish support:", chart.is_above_bullish_support(last_price))
print("Below bearish resistance:", chart.is_below_bearish_resistance(last_price))
```

These checks are the normal first gate before acting on breakout or breakdown patterns.

## Indicators and Momentum

```python
indicators.calculate(chart)

sma_short = indicators.sma_short()
bands = indicators.bollinger()
rsi = indicators.rsi()
obv = indicators.obv()

col = chart.column_count() - 1
if col >= 0:
    print("SMA short:", sma_short.value(col))
    print("Bollinger upper:", bands.upper(col))
    print("RSI:", rsi.value(col))
    print("OBV:", obv.value(col))
```

## Signals and Pattern Detection

```python
signals = indicators.signals()
patterns = indicators.patterns()

print("Current signal:", signals.current_signal())
print("Buy count:", signals.buy_count())
print("Sell count:", signals.sell_count())

print("Pattern count:", patterns.pattern_count())
print("Bullish patterns:", len(patterns.bullish_patterns()))
print("Bearish patterns:", len(patterns.bearish_patterns()))
```

## Support, Resistance, Objectives, Congestion

```python
sr = indicators.support_resistance()
obj = indicators.objectives()
cong = indicators.congestion()

print("Support levels:", sr.support_levels())
print("Resistance levels:", sr.resistance_levels())
print("Significant levels (>=3 touches):", sr.significant_levels(3))

print("Bullish targets:", obj.bullish_targets())
print("Bearish targets:", obj.bearish_targets())

print("Congestion zones:", cong.zones())
```

## Real-Time Dashboard

```python
from pypnf_dashboard import DashboardServer

server = DashboardServer(chart, indicators)
server.start("127.0.0.1", 8761)
server.publish()
print(server.url())
```

You can call `server.publish()` after each new bar/tick batch to keep the browser in sync.

## API Map

Core:
- `Chart`, `ChartConfig`, `Box`, `Column`

Indicators:
- `Indicators`, `IndicatorConfig`
- `MovingAverage`, `BollingerBands`, `RSI`, `OnBalanceVolume`, `BullishPercent`
- `SignalDetector`, `PatternRecognizer`, `SupportResistance`, `PriceObjectiveCalculator`, `CongestionDetector`

Data:
- `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`, `CongestionZone`

Enums:
- `BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `SignalType`, `PatternType`

## Versioning and Compatibility

- Python package version tracks the same release as the core engine.
- Keep all bindings on the same version when mixing languages in one system.
- See `CHANGELOG.md` for version-by-version behavior changes.

## Troubleshooting

- `ImportError` / native load issues: rebuild and ensure the native library is discoverable.
- Empty indicator values: verify you have enough columns for the configured lookback periods.
- Unexpected chart shape: ensure `HighLow` mode receives real high/low values, not close-only values.

## Documentation and Links

- Python API reference: `docs/bindings/python.md`
- Cross-language API index: `docs/reference/api-symbol-index.md`
- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
