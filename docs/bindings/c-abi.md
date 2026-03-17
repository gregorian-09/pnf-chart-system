# C ABI Reference

The C ABI is the stable interoperability boundary used by all non-C++ bindings.

Source:
- `bindings/c/pnf_c.hpp`
- `bindings/c/pnf_c.cpp`

For exhaustive generated function coverage, see:
- [API Symbol Index](../reference/api-symbol-index.md)

## ABI Contract

- Opaque handles: `PnfChart*`, `PnfIndicators*`, `PnfViewer*`
- C-compatible enums and POD structs
- Explicit allocation/free ownership for strings and arrays
- No global mutable state required by callers

## Enum Types

- `PnfBoxType`
- `PnfColumnType`
- `PnfConstructionMethod`
- `PnfBoxSizeMethod`
- `PnfSignalType`
- `PnfPatternType`

## Config and Data Structs

- `PnfChartConfig`
- `PnfIndicatorConfig`
- `PnfOHLC`
- `PnfSignal`
- `PnfPattern`
- `PnfSupportResistanceLevel`
- `PnfColumnData`
- `PnfDoubleArray`
- `PnfSignalArray`
- `PnfPatternArray`
- `PnfLevelArray`
- `PnfChartData`
- `PnfIndicatorData`

## API Families

### Version
- `pnf_version_string`
- `pnf_version_major`
- `pnf_version_minor`
- `pnf_version_patch`

### Default Config Builders
- `pnf_chart_config_default`
- `pnf_indicator_config_default`

### Chart Lifecycle
- `pnf_chart_create`
- `pnf_chart_create_default`
- `pnf_chart_destroy`

### Chart Input
- `pnf_chart_add_data`
- `pnf_chart_add_price`
- `pnf_chart_add_ohlc`

### Chart Query
- `pnf_chart_column_count`
- `pnf_chart_x_column_count`
- `pnf_chart_o_column_count`
- `pnf_chart_box_size`
- `pnf_chart_column_type`
- `pnf_chart_column_box_count`
- `pnf_chart_column_highest`
- `pnf_chart_column_lowest`
- `pnf_chart_box_price`
- `pnf_chart_box_type`
- `pnf_chart_box_marker`
- `pnf_chart_has_bullish_bias`
- `pnf_chart_has_bearish_bias`
- `pnf_chart_is_above_bullish_support`
- `pnf_chart_is_below_bearish_resistance`

### Chart Utility and Export
- `pnf_chart_clear`
- `pnf_chart_to_string`
- `pnf_chart_to_ascii`
- `pnf_chart_to_json`
- `pnf_chart_export_data`
- `pnf_chart_data_destroy`

### Indicators Lifecycle
- `pnf_indicators_create`
- `pnf_indicators_create_default`
- `pnf_indicators_create_with_config`
- `pnf_indicators_destroy`

### Indicators Configuration
- `pnf_indicators_configure`
- `pnf_indicators_set_sma_periods`
- `pnf_indicators_set_bollinger_params`
- `pnf_indicators_set_rsi_params`
- `pnf_indicators_set_bullish_percent_thresholds`
- `pnf_indicators_set_support_resistance_threshold`
- `pnf_indicators_set_congestion_params`

### Indicators Calculation
- `pnf_indicators_calculate`

### Indicators: SMA
- `pnf_indicators_sma_short`
- `pnf_indicators_sma_medium`
- `pnf_indicators_sma_long`
- `pnf_indicators_sma_short_values`
- `pnf_indicators_sma_medium_values`
- `pnf_indicators_sma_long_values`

### Indicators: Bollinger Bands
- `pnf_indicators_bollinger_middle`
- `pnf_indicators_bollinger_upper`
- `pnf_indicators_bollinger_lower`
- `pnf_indicators_bollinger_middle_values`
- `pnf_indicators_bollinger_upper_values`
- `pnf_indicators_bollinger_lower_values`

### Indicators: RSI and OBV
- `pnf_indicators_rsi`
- `pnf_indicators_rsi_is_overbought`
- `pnf_indicators_rsi_is_oversold`
- `pnf_indicators_rsi_values`
- `pnf_indicators_obv`
- `pnf_indicators_obv_values`

### Indicators: Bullish Percent and Signals
- `pnf_indicators_bullish_percent`
- `pnf_indicators_is_bullish_alert`
- `pnf_indicators_is_bearish_alert`
- `pnf_indicators_current_signal`
- `pnf_indicators_signal_count`
- `pnf_indicators_signal_at`
- `pnf_indicators_signals`
- `pnf_indicators_buy_signal_count`
- `pnf_indicators_sell_signal_count`

### Indicators: Patterns
- `pnf_indicators_pattern_count`
- `pnf_indicators_bullish_pattern_count`
- `pnf_indicators_bearish_pattern_count`
- `pnf_indicators_pattern_at`
- `pnf_indicators_patterns`
- `pnf_indicators_bullish_patterns`
- `pnf_indicators_bearish_patterns`

### Indicators: Support/Resistance
- `pnf_indicators_support_level_count`
- `pnf_indicators_resistance_level_count`
- `pnf_indicators_is_near_support`
- `pnf_indicators_is_near_resistance`
- `pnf_indicators_support_levels`
- `pnf_indicators_resistance_levels`
- `pnf_indicators_support_prices`
- `pnf_indicators_resistance_prices`

### Indicators: Congestion and Export
- `pnf_indicators_is_in_congestion`
- `pnf_indicators_congestion_zone_count`
- `pnf_indicators_export_data`
- `pnf_indicator_data_destroy`
- `pnf_indicators_summary`
- `pnf_indicators_to_string`

### Memory Release Helpers
- `pnf_free_string`
- `pnf_free_double_array`
- `pnf_free_signal_array`
- `pnf_free_pattern_array`
- `pnf_free_level_array`

### Interactive Viewer
- `pnf_viewer_create`
- `pnf_viewer_destroy`
- `pnf_viewer_set_chart`
- `pnf_viewer_set_indicators`
- `pnf_viewer_update`
- `pnf_viewer_is_open`
- `pnf_viewer_show`
- `pnf_viewer_close`
- `pnf_viewer_poll_events`

## Memory Ownership Rules

Caller must release memory returned by ABI allocation functions. Use only matching free helpers from this ABI.

## Threading

The ABI does not provide built-in synchronization for mutating a single chart/indicator handle from multiple threads.
