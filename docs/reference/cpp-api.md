# C++ API Reference

This page documents the core public API declared under `headers/pnf/`.

## Header Map

- `headers/pnf/types.hpp`
- `headers/pnf/box.hpp`
- `headers/pnf/column.hpp`
- `headers/pnf/chart.hpp`
- `headers/pnf/indicators.hpp`
- `headers/pnf/trendline.hpp`
- `headers/pnf/visualization.hpp`
- `headers/pnf/csv_loader.hpp`
- `headers/pnf/version.hpp`

## Primary Types

### `ChartConfig`

- `method`
- `box_size_method`
- `box_size`
- `reversal`

### `Chart`

Creation and data input:
- `Chart(const ChartConfig&)`
- `add_data(high, low, close, time)`
- `add_data(price, time)`
- `add_ohlc(const OHLC&)`

Inspection:
- `column_count()`, `x_column_count()`, `o_column_count()`, `mixed_column_count()`
- `column(index)`, `last_column()`
- `all_prices()`
- `current_box_size()`
- `has_bullish_bias()`, `has_bearish_bias()`
- `should_take_bullish_signals()`, `should_take_bearish_signals()`
- `is_above_bullish_support(price)`, `is_below_bearish_resistance(price)`

Lifecycle/utility:
- `clear()`
- `to_string()`

### `Indicators`

Configuration and execution:
- constructor with `IndicatorConfig`
- `configure(const IndicatorConfig&)`
- `calculate(const Chart&)`
- `calculate_with_volume(const Chart&, const std::vector<OHLC>&)`

Retrieval areas:
- moving averages
- Bollinger bands
- RSI
- OBV
- signals
- patterns
- support/resistance
- price objectives
- congestion zones
- data export and summary

## Data Types (`types.hpp`)

- Enums: `BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `TrendLineType`, `SignalType`, `PatternType`
- Structs: `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`, `TrendLinePoint`
- Helpers: `pattern_type_to_string`, `is_bullish_pattern`, `is_bearish_pattern`

## Error and Boundary Behavior

- API prefers safe defaults for invalid column lookups in most read methods.
- Mutating calls return `bool` success/failure.
- Caller owns object lifetimes directly in C++ (RAII recommended).

## Reference by Binding

- [C ABI](../bindings/c-abi.md)
- [Python API](../bindings/python.md)
- [Java API](../bindings/java.md)
- [Rust API](../bindings/rust.md)
- [C# API](../bindings/csharp.md)
