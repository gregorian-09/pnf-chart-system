# C++ API Reference

Primary headers (public API):
- `headers/pnf/types.hpp`
- `headers/pnf/box.hpp`
- `headers/pnf/column.hpp`
- `headers/pnf/chart.hpp`
- `headers/pnf/trendline.hpp`
- `headers/pnf/indicators.hpp`
- `headers/pnf/visualization.hpp`
- `headers/pnf/csv_loader.hpp`
- `headers/pnf/version.hpp`

For exhaustive symbol-level coverage generated from source, see:
- [API Symbol Index](api-symbol-index.md)

## Core Model

### Enums
- `BoxType`
- `ColumnType`
- `ConstructionMethod`
- `BoxSizeMethod`
- `TrendLineType`
- `SignalType`
- `PatternType`

### Structs
- `OHLC`
- `Signal`
- `Pattern`
- `SupportResistanceLevel`
- `PriceObjective`
- `TrendLinePoint`
- `ChartConfig`
- `IndicatorConfig`
- `ColumnData`
- `ChartData`
- `IndicatorData`

### Free Helpers
- `pattern_type_to_string(PatternType)`
- `is_bullish_pattern(PatternType)`
- `is_bearish_pattern(PatternType)`

## Chart Layer

### `Box`
- constructor: `Box(price, type, marker = "")`
- `price()`, `type()`, `marker()`
- `set_marker(...)`, `set_type(...)`
- `to_string()`

### `Column`
- constructor: `Column(type = ColumnType::X)`
- `add_box(...)` (overloads)
- `remove_box(...)`, `has_box(...)`
- `get_box(...)`, `get_box_at(...)`
- `get_box_marker(...)`, `set_box_marker(...)`
- `box_count()`, `highest_price()`, `lowest_price()`
- `type()`, `set_type(...)`
- `clear()`, `to_string()`

### `Chart`
- constructor: `Chart(const ChartConfig&)`
- ingestion: `add_data(...)` (OHLC/close overloads), `add_ohlc(...)`
- structure: `column_count()`, `column(i)`, `last_column()`
- stats: `x_column_count()`, `o_column_count()`, `mixed_column_count()`
- index helpers: `x_column_indices()`, `o_column_indices()`, `mixed_column_indices()`
- market state: `all_prices()`, `config()`, `current_box_size()`
- bias/support checks: `has_bullish_bias()`, `has_bearish_bias()`, `should_take_bullish_signals()`, `should_take_bearish_signals()`, `is_above_bullish_support(...)`, `is_below_bearish_resistance(...)`
- lifecycle/export: `clear()`, `to_string()`, `columns()`

## Trendline Layer

### `TrendLine`
- constructor with start point + `box_size`
- `update_end_point(...)`, `is_broken(...)`, `test(...)`, `price_at_column(...)`
- `type()`, `start_point()`, `end_point()`, `is_active()`, `set_active(...)`, `was_touched()`, `touch_count()`
- `to_string()`

### `TrendLineManager`
- constructor: `TrendLineManager(box_size)`
- mutation: `update(...)`, `process_new_column(...)`, `check_break(...)`
- queries: `active_trend_line()`, `all_trend_lines()`, `is_above_bullish_support(...)`, `is_below_bearish_resistance(...)`, `has_bullish_bias()`, `has_bearish_bias()`
- lifecycle: `clear()`, `set_box_size(...)`, `to_string()`

## Indicator Layer

### Indicator Components
- `MovingAverage`
- `BollingerBands`
- `RSI`
- `OnBalanceVolume`
- `BullishPercent`
- `SignalDetector`
- `PatternRecognizer`
- `SupportResistance`
- `PriceObjectiveCalculator`
- `CongestionDetector`

Each component exposes:
- configuration setters (where applicable)
- `calculate`/`detect`/`identify`
- point queries by column
- vector accessors for computed series
- `to_string()`

### `Indicators` Aggregator
- constructors: default + `Indicators(const IndicatorConfig&)`
- configuration: `configure(...)`, `config()`
- execution: `calculate(...)`, `calculate_with_volume(...)`
- accessors for each component pointer (const + mutable)
- exports: `export_data()`, `export_chart_data(...)`
- summary: `summary()`, `to_string()`

## Rendering and Export

### `AsciiRenderer`
- constructor with `RenderConfig`
- `render(chart)`, `render_with_indicators(chart, indicators)`
- `set_config(...)`, `config()`

### `SvgRenderer`
- constructor with `SvgConfig`
- `render(chart)`, `render_with_indicators(chart, indicators)`
- `set_config(...)`, `config()`

### `JsonExporter`
- constructor with `JsonConfig`
- `export_chart(...)`, `export_chart_with_indicators(...)`, `export_indicators(...)`
- `set_config(...)`, `config()`

### `CsvExporter`
- `export_columns(...)`
- `export_boxes(...)`
- `export_signals(...)`
- `export_patterns(...)`
- `export_indicator_values(...)`

### `Visualization` Static Helpers
- `to_ascii(...)`
- `to_svg(...)`
- `to_json(...)`
- `to_csv_columns(...)`
- `to_csv_boxes(...)`
- `to_ascii_with_indicators(...)`
- `to_svg_with_indicators(...)`
- `to_json_with_indicators(...)`

## IO and Version

### `CSVLoader`
- `load(filename)`
- `parse_datetime(date_str, format)`

### `Version`
- `Version::major`
- `Version::minor`
- `Version::patch`
- `Version::string`
