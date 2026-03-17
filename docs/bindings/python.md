# Python Binding API (`pypnf`)

Source:
- `bindings/python/pnf_python.cpp`
- `bindings/python/pypnf_dashboard.py`

For exhaustive generated symbol coverage, see:
- [API Symbol Index](../reference/api-symbol-index.md)

## Runtime Model

- Native core is exposed via pybind11.
- Python objects own native lifetimes.
- `Chart.column(i).get_box_at(j)` style views are valid only while parent objects live.

## Module-Level Functions

- `version()`
- `version_major()`
- `version_minor()`
- `version_patch()`

## Enums

- `BoxType`
- `ColumnType`
- `ConstructionMethod`
- `BoxSizeMethod`
- `SignalType`
- `PatternType`

## Data/Config Classes

- `ChartConfig`
- `IndicatorConfig`
- `OHLC`
- `Signal`
- `Pattern`
- `SupportResistanceLevel`
- `PriceObjective`
- `CongestionZone`

## Core Objects

### `Chart`

- constructors: `Chart()`, `Chart(config)`
- ingest: `add_data(...)`, `add_price(...)`, `add_ohlc(...)`
- columns: `column_count()`, `column_type(i)`, `column_box_count(i)`, `column_high(i)`, `column_low(i)`
- boxes: `box_price(col, box)`, `box_type(col, box)`, `box_marker(col, box)`
- counts/state: `x_column_count()`, `o_column_count()`, `all_prices()`, `current_box_size()`
- bias/trend checks: `has_bullish_bias()`, `has_bearish_bias()`, `is_above_bullish_support(price)`, `is_below_bearish_resistance(price)`
- utility: `clear()`, `to_ascii()`, `to_json()`, `__str__`, `__len__`

### `Box`

- `price()`, `type()`, `marker()`, `__str__`

### `Column`

- `box_count()`, `type()`, `highest_price()`, `lowest_price()`, `get_box_at(index)`, `has_box(price)`, `__str__`

## Indicator Components (direct)

### `MovingAverage`
- `value(column)`, `has_value(column)`, `period()`, `set_period(period)`, `values()`, `values_copy()`, `__str__`

### `BollingerBands`
- `middle(column)`, `upper(column)`, `lower(column)`
- `has_value(column)`, `is_above_upper(column, price)`, `is_below_lower(column, price)`
- `period()`, `std_devs()`, `set_period(period)`, `set_std_devs(std)`
- `middle_band()`, `upper_band()`, `lower_band()`, `middle_copy()`, `upper_copy()`, `lower_copy()`, `__str__`

### `RSI`
- `value(column)`, `has_value(column)`
- `is_overbought(column)`, `is_oversold(column)`
- `is_overbought_custom(column, threshold)`, `is_oversold_custom(column, threshold)`
- `period()`, `overbought_threshold()`, `oversold_threshold()`
- `set_period(period)`, `set_thresholds(overbought, oversold)`
- `values()`, `values_copy()`, `__str__`

### `OnBalanceVolume`
- `value(column)`, `has_value(column)`, `values()`, `values_copy()`, `__str__`

### `BullishPercent`
- `value()`, `is_bullish_alert()`, `is_bearish_alert()`
- `bullish_threshold()`, `bearish_threshold()`
- `set_thresholds(bullish, bearish)`, `__str__`

### `SignalDetector`
- `current_signal()`, `signals()`, `signals_copy()`, `last_signal()`
- `has_buy_signal()`, `has_sell_signal()`
- `buy_signals()`, `sell_signals()`, `buy_count()`, `sell_count()`, `__str__`

### `PatternRecognizer`
- `patterns()`, `patterns_copy()`
- `bullish_patterns()`, `bearish_patterns()`
- `latest_pattern()`, `has_pattern(pattern_type)`, `patterns_of_type(pattern_type)`
- `pattern_count()`, `bullish_count()`, `bearish_count()`, `__str__`

### `SupportResistance`
- `support_levels()`, `resistance_levels()`, `levels_copy()`
- `significant_levels(min_touches=3)`
- `is_near_support(price, tolerance)`, `is_near_resistance(price, tolerance)`
- `support_prices()`, `resistance_prices()`
- `threshold()`, `set_threshold(...)`, `__str__`

### `PriceObjectiveCalculator`
- `objectives()`, `objectives_copy()`, `latest()`
- `bullish_objectives()`, `bearish_objectives()`
- `bullish_targets()`, `bearish_targets()`, `__str__`

### `CongestionDetector`
- `zones()`, `zones_copy()`, `is_in_congestion(column)`, `largest_zone()`
- `min_columns()`, `threshold()`, `set_min_columns(...)`, `set_threshold(...)`, `__str__`

## `Indicators` Aggregator

- constructors: `Indicators()`, `Indicators(config)`
- config/update: `configure(config)`, `config()`, `calculate(chart)`, `calculate_with_volume(chart, ohlc_data)`
- component accessors: `sma_short()`, `sma_medium()`, `sma_long()`, `bollinger()`, `rsi()`, `obv()`, `bullish_percent()`, `signals()`, `patterns()`, `support_resistance()`, `objectives()`, `congestion()`
- export/summary: `export_data()`, `summary()`, `__str__`

## Dashboard Helper (`pypnf_dashboard`)

- `DashboardServer(chart, indicators)`
- `start(host, port)` / `stop()`
- `publish()` / `snapshot_json()`
- `start_auto_publish(interval_ms)` / `stop_auto_publish()`
- `url()`
