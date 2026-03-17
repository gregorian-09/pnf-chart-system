# Rust Binding API (`pnf` crate)

Source:
- `bindings/rust/src/lib.rs`
- `bindings/rust/src/types.rs`
- `bindings/rust/src/chart.rs`
- `bindings/rust/src/indicators.rs`
- `bindings/rust/src/dashboard.rs`

For exhaustive generated symbol coverage, see:
- [API Symbol Index](../reference/api-symbol-index.md)

## Crate Exports

- `Chart`
- `Indicators`
- `DashboardServer`
- `build_snapshot_json`
- all public types from `types.rs`

## Top-Level Version API

- `version()`
- `version_major()`
- `version_minor()`
- `version_patch()`

## Public Types (`types.rs`)

### Enums
- `BoxType`
- `ColumnType`
- `ConstructionMethod`
- `BoxSizeMethod`
- `SignalType`
- `PatternType`

### Structs
- `ChartConfig`
- `IndicatorConfig`
- `OHLC`
- `Signal`
- `Pattern`
- `SupportResistanceLevel`
- `ColumnData`

## `Chart`

- constructors: `new()`, `with_config(config)`
- ingest: `add_data(...)`, `add_price(...)`, `add_ohlc(&OHLC)`
- counts/state: `column_count()`, `x_column_count()`, `o_column_count()`, `box_size()`
- column/box queries: `column_type(i)`, `column_box_count(i)`, `column_highest(i)`, `column_lowest(i)`, `box_price(col, box)`, `box_type(col, box)`, `box_marker(col, box)`
- trend/bias checks: `has_bullish_bias()`, `has_bearish_bias()`, `is_above_bullish_support(price)`, `is_below_bearish_resistance(price)`
- utility/export: `clear()`, `to_string()`, `to_ascii()`, `to_json()`

## `Indicators`

### Lifecycle and Configuration
- `new()`
- `with_config(config)`
- `configure(config)`
- `set_sma_periods(short, medium, long)`
- `set_bollinger_params(period, std_devs)`
- `set_rsi_params(period, overbought, oversold)`
- `set_bullish_percent_thresholds(bullish, bearish)`
- `set_support_resistance_threshold(threshold)`
- `set_congestion_params(min_columns, price_range)`
- `calculate(&Chart)`

### Indicator Values
- SMA: `sma_short`, `sma_medium`, `sma_long`, and `*_values`
- Bollinger: `bollinger_middle`, `bollinger_upper`, `bollinger_lower`, and `*_values`
- RSI: `rsi`, `rsi_is_overbought`, `rsi_is_oversold`, `rsi_values`
- OBV: `obv`, `obv_values`

### Signals, Patterns, Levels
- alerts and current state: `bullish_percent`, `is_bullish_alert`, `is_bearish_alert`, `current_signal`
- signals: `signal_count`, `signals`, `buy_signal_count`, `sell_signal_count`
- patterns: `pattern_count`, `bullish_pattern_count`, `bearish_pattern_count`, `patterns`, `bullish_patterns`, `bearish_patterns`
- support/resistance: `support_level_count`, `resistance_level_count`, `is_near_support`, `is_near_resistance`, `support_levels`, `resistance_levels`, `support_prices`, `resistance_prices`
- congestion: `is_in_congestion`, `congestion_zone_count`

### Summary
- `summary()`
- `to_string()`

## Dashboard (`dashboard.rs`)

### `DashboardServer`
- `new()`
- `start(host, port)`
- `url()`
- `snapshot_json()`
- `publish(chart, indicators)`
- `stop()`

### Snapshot Builder
- `build_snapshot_json(chart, indicators, sequence)`
