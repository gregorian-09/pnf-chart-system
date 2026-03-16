# Python Binding API (`pypnf`)

Source:
- `bindings/python/pnf_python.cpp`

The Python API wraps the C++ core via pybind11 and exposes Python-native classes and enums.

## Module Composition

- Enums: `BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `SignalType`, `PatternType`
- Config/data structs: `ChartConfig`, `IndicatorConfig`, `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`
- Core classes: `Chart`, `Indicators`
- Indicator helper classes: `MovingAverage`, `BollingerBands`, `RSI`, `OnBalanceVolume`, `BullishPercent`, `SignalDetector`, `PatternRecognizer`, `SupportResistance`

## Lifecycle and Ownership

- Python owns wrapper objects.
- Native object lifetime tracks Python object lifetime.
- `Column`/`Box` views are only valid while parent `Chart` remains alive.

## `Chart` API Surface

Mutations:
- `add_data(high, low, close, timestamp)`
- `add_price(price, timestamp)`
- `add_ohlc(ohlc)`
- `clear()`

Structure queries:
- `column_count()`, `x_column_count()`, `o_column_count()`
- `column_type(i)`, `column_box_count(i)`, `column_high(i)`, `column_low(i)`
- `all_prices()`

Market state/export:
- `current_box_size()`
- `has_bullish_bias()`, `has_bearish_bias()`
- `is_above_bullish_support(price)`, `is_below_bearish_resistance(price)`
- `to_ascii()`, `to_json()`, `str(chart)`

## `Indicators` API Surface

Configuration/execution:
- `configure(config)`
- `set_sma_periods(...)`
- `set_bollinger_params(...)`
- `set_rsi_params(...)`
- `set_bullish_percent_thresholds(...)`
- `set_support_resistance_threshold(...)`
- `set_congestion_params(...)`
- `calculate(chart)`

Retrieval:
- SMA/Bollinger/RSI/OBV point values and vectors
- signal access and counts
- pattern access and counts
- support/resistance levels and prices
- objectives and congestion checks
- `summary()`, `str(indicators)`

## Error Semantics

- Out-of-range lookups generally return defaults rather than raising.
- Empty charts produce empty vectors and default numeric values.

## Example

```python
import pypnf
from datetime import datetime

cfg = pypnf.ChartConfig()
cfg.box_size_method = pypnf.BoxSizeMethod.Fixed
cfg.box_size = 1.0

chart = pypnf.Chart(cfg)
chart.add_price(100.0, datetime.now())
chart.add_price(103.0, datetime.now())

ind = pypnf.Indicators()
ind.calculate(chart)
print(ind.summary())
```
