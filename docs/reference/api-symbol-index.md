# API Symbol Index

This file is generated from source and provides exhaustive symbol coverage across the project APIs.

Do not edit manually. Regenerate with:

```bash
python3 tools/generate_api_symbol_index.py
```

- C++ symbols: **232**
- C ABI functions: **107**
- Python symbols: **157**
- Java symbols: **166**
- Rust symbols: **220**
- C# symbols: **190**

## C++ Core

Total symbols: **232**

- `AsciiRenderer`
- `BollingerBands`
- `Box`
- `BoxSizeMethod`
- `BoxType`
- `BullishPercent`
- `CSVLoader`
- `Chart`
- `ChartConfig`
- `ChartData`
- `Column`
- `ColumnData`
- `ColumnType`
- `CongestionDetector`
- `CongestionZone`
- `ConstructionMethod`
- `CsvExporter`
- `IndicatorConfig`
- `IndicatorData`
- `Indicators`
- `JsonConfig`
- `JsonExporter`
- `MovingAverage`
- `OHLC`
- `OnBalanceVolume`
- `Pattern`
- `PatternRecognizer`
- `PatternType`
- `PriceObjective`
- `PriceObjectiveCalculator`
- `RSI`
- `RenderConfig`
- `Signal`
- `SignalDetector`
- `SignalType`
- `SupportResistance`
- `SupportResistanceLevel`
- `SvgConfig`
- `SvgRenderer`
- `TrendLine`
- `TrendLineManager`
- `TrendLinePoint`
- `TrendLineType`
- `Version`
- `Visualization`
- `active_trend_line`
- `add_box`
- `add_data`
- `add_ohlc`
- `all_prices`
- `all_trend_lines`
- `bearish_count`
- `bearish_objectives`
- `bearish_patterns`
- `bearish_targets`
- `bearish_threshold`
- `bollinger`
- `box_count`
- `bullish_count`
- `bullish_objectives`
- `bullish_patterns`
- `bullish_percent`
- `bullish_targets`
- `bullish_threshold`
- `buy_count`
- `buy_signals`
- `calculate`
- `calculate_all`
- `calculate_vertical_count`
- `calculate_with_volume`
- `check_break`
- `clear`
- `column`
- `column_count`
- `columns`
- `config`
- `configure`
- `congestion`
- `current_box_size`
- `current_signal`
- `detect`
- `detect_ascending_triple_top`
- `detect_bear_trap`
- `detect_bearish_catapult`
- `detect_bearish_signal_reversed`
- `detect_bearish_triangle`
- `detect_bull_trap`
- `detect_bullish_catapult`
- `detect_bullish_signal_reversed`
- `detect_bullish_triangle`
- `detect_descending_triple_bottom`
- `detect_double_bottom_breakdown`
- `detect_double_top_breakout`
- `detect_high_pole`
- `detect_long_tail_down`
- `detect_low_pole`
- `detect_quadruple_bottom_breakdown`
- `detect_quadruple_top_breakout`
- `detect_spread_triple_bottom`
- `detect_spread_triple_top`
- `detect_triple_bottom_breakdown`
- `detect_triple_top_breakout`
- `end_point`
- `export_boxes`
- `export_chart`
- `export_chart_data`
- `export_chart_with_indicators`
- `export_columns`
- `export_data`
- `export_indicator_values`
- `export_indicators`
- `export_patterns`
- `export_signals`
- `get_box`
- `get_box_at`
- `get_box_marker`
- `has_bearish_bias`
- `has_box`
- `has_bullish_bias`
- `has_buy_signal`
- `has_pattern`
- `has_sell_signal`
- `has_value`
- `highest_price`
- `identify`
- `is_above_bullish_support`
- `is_above_upper`
- `is_active`
- `is_bearish_alert`
- `is_bearish_pattern`
- `is_below_bearish_resistance`
- `is_below_lower`
- `is_broken`
- `is_bullish_alert`
- `is_bullish_pattern`
- `is_in_congestion`
- `is_near_resistance`
- `is_near_support`
- `is_overbought`
- `is_overbought_custom`
- `is_oversold`
- `is_oversold_custom`
- `largest_zone`
- `last_column`
- `last_signal`
- `latest`
- `latest_pattern`
- `levels`
- `levels_copy`
- `load`
- `lower`
- `lower_band`
- `lower_copy`
- `lowest_price`
- `marker`
- `middle`
- `middle_band`
- `middle_copy`
- `min_columns`
- `mixed_column_count`
- `mixed_column_indices`
- `o_column_count`
- `o_column_indices`
- `objectives`
- `objectives_copy`
- `obv`
- `overbought_threshold`
- `oversold_threshold`
- `parse_datetime`
- `pattern_count`
- `pattern_type_to_string`
- `patterns`
- `patterns_copy`
- `patterns_of_type`
- `period`
- `price`
- `price_at_column`
- `process_new_column`
- `remove_box`
- `render`
- `render_with_indicators`
- `resistance_levels`
- `resistance_prices`
- `rsi`
- `sell_count`
- `sell_signals`
- `set_active`
- `set_box_marker`
- `set_box_size`
- `set_config`
- `set_marker`
- `set_min_columns`
- `set_period`
- `set_std_devs`
- `set_threshold`
- `set_thresholds`
- `set_type`
- `should_take_bearish_signals`
- `should_take_bullish_signals`
- `signals`
- `signals_copy`
- `significant_levels`
- `sma_long`
- `sma_medium`
- `sma_short`
- `start_point`
- `std_devs`
- `summary`
- `support_levels`
- `support_prices`
- `support_resistance`
- `test`
- `threshold`
- `to_csv_boxes`
- `to_csv_columns`
- `to_string`
- `touch_count`
- `trend_line_manager`
- `type`
- `update`
- `update_end_point`
- `upper`
- `upper_band`
- `upper_copy`
- `value`
- `values`
- `values_copy`
- `was_touched`
- `x_column_count`
- `x_column_indices`
- `zones`
- `zones_copy`

## C ABI

Total functions: **107**

- `pnf_chart_add_data`
- `pnf_chart_add_ohlc`
- `pnf_chart_add_price`
- `pnf_chart_box_marker`
- `pnf_chart_box_price`
- `pnf_chart_box_size`
- `pnf_chart_box_type`
- `pnf_chart_clear`
- `pnf_chart_column_box_count`
- `pnf_chart_column_count`
- `pnf_chart_column_highest`
- `pnf_chart_column_lowest`
- `pnf_chart_column_type`
- `pnf_chart_config_default`
- `pnf_chart_create`
- `pnf_chart_create_default`
- `pnf_chart_data_destroy`
- `pnf_chart_destroy`
- `pnf_chart_export_data`
- `pnf_chart_has_bearish_bias`
- `pnf_chart_has_bullish_bias`
- `pnf_chart_is_above_bullish_support`
- `pnf_chart_is_below_bearish_resistance`
- `pnf_chart_o_column_count`
- `pnf_chart_to_ascii`
- `pnf_chart_to_json`
- `pnf_chart_to_string`
- `pnf_chart_x_column_count`
- `pnf_free_double_array`
- `pnf_free_level_array`
- `pnf_free_pattern_array`
- `pnf_free_signal_array`
- `pnf_free_string`
- `pnf_indicator_config_default`
- `pnf_indicator_data_destroy`
- `pnf_indicators_bearish_pattern_count`
- `pnf_indicators_bearish_patterns`
- `pnf_indicators_bollinger_lower`
- `pnf_indicators_bollinger_lower_values`
- `pnf_indicators_bollinger_middle`
- `pnf_indicators_bollinger_middle_values`
- `pnf_indicators_bollinger_upper`
- `pnf_indicators_bollinger_upper_values`
- `pnf_indicators_bullish_pattern_count`
- `pnf_indicators_bullish_patterns`
- `pnf_indicators_bullish_percent`
- `pnf_indicators_buy_signal_count`
- `pnf_indicators_calculate`
- `pnf_indicators_configure`
- `pnf_indicators_congestion_zone_count`
- `pnf_indicators_create`
- `pnf_indicators_create_default`
- `pnf_indicators_create_with_config`
- `pnf_indicators_current_signal`
- `pnf_indicators_destroy`
- `pnf_indicators_export_data`
- `pnf_indicators_is_bearish_alert`
- `pnf_indicators_is_bullish_alert`
- `pnf_indicators_is_in_congestion`
- `pnf_indicators_is_near_resistance`
- `pnf_indicators_is_near_support`
- `pnf_indicators_obv`
- `pnf_indicators_obv_values`
- `pnf_indicators_pattern_at`
- `pnf_indicators_pattern_count`
- `pnf_indicators_patterns`
- `pnf_indicators_resistance_level_count`
- `pnf_indicators_resistance_levels`
- `pnf_indicators_resistance_prices`
- `pnf_indicators_rsi`
- `pnf_indicators_rsi_is_overbought`
- `pnf_indicators_rsi_is_oversold`
- `pnf_indicators_rsi_values`
- `pnf_indicators_sell_signal_count`
- `pnf_indicators_set_bollinger_params`
- `pnf_indicators_set_bullish_percent_thresholds`
- `pnf_indicators_set_congestion_params`
- `pnf_indicators_set_rsi_params`
- `pnf_indicators_set_sma_periods`
- `pnf_indicators_set_support_resistance_threshold`
- `pnf_indicators_signal_at`
- `pnf_indicators_signal_count`
- `pnf_indicators_signals`
- `pnf_indicators_sma_long`
- `pnf_indicators_sma_long_values`
- `pnf_indicators_sma_medium`
- `pnf_indicators_sma_medium_values`
- `pnf_indicators_sma_short`
- `pnf_indicators_sma_short_values`
- `pnf_indicators_summary`
- `pnf_indicators_support_level_count`
- `pnf_indicators_support_levels`
- `pnf_indicators_support_prices`
- `pnf_indicators_to_string`
- `pnf_version_major`
- `pnf_version_minor`
- `pnf_version_patch`
- `pnf_version_string`
- `pnf_viewer_close`
- `pnf_viewer_create`
- `pnf_viewer_destroy`
- `pnf_viewer_is_open`
- `pnf_viewer_poll_events`
- `pnf_viewer_set_chart`
- `pnf_viewer_set_indicators`
- `pnf_viewer_show`
- `pnf_viewer_update`

## Python (`pypnf`)

Total symbols: **157**

### `BollingerBands`

- `__str__`
- `has_value`
- `is_above_upper`
- `is_below_lower`
- `lower`
- `lower_band`
- `lower_copy`
- `middle`
- `middle_band`
- `middle_copy`
- `period`
- `set_period`
- `set_std_devs`
- `std_devs`
- `upper`
- `upper_band`
- `upper_copy`

### `Box`

- `__str__`
- `box_count`
- `get_box_at`
- `has_box`
- `highest_price`
- `lowest_price`
- `marker`
- `price`
- `type`

### `BullishPercent`

- `__str__`
- `bearish_threshold`
- `bullish_threshold`
- `is_bearish_alert`
- `is_bullish_alert`
- `set_thresholds`
- `value`

### `Chart`

- `__len__`
- `__str__`
- `add_data`
- `add_ohlc`
- `add_price`
- `all_prices`
- `box_marker`
- `box_price`
- `box_type`
- `clear`
- `column_box_count`
- `column_count`
- `column_high`
- `column_low`
- `column_type`
- `current_box_size`
- `has_bearish_bias`
- `has_bullish_bias`
- `is_above_bullish_support`
- `is_below_bearish_resistance`
- `o_column_count`
- `to_ascii`
- `to_json`
- `x_column_count`

### `ChartConfig`


### `ChartData`


### `ColumnData`


### `CongestionDetector`

- `__str__`
- `is_in_congestion`
- `largest_zone`
- `min_columns`
- `set_min_columns`
- `set_threshold`
- `threshold`
- `zones`
- `zones_copy`

### `CongestionZone`


### `CSVLoader`


### `IndicatorConfig`


### `IndicatorData`


### `Indicators`

- `__str__`
- `bollinger`
- `bullish_percent`
- `calculate`
- `calculate_with_volume`
- `config`
- `configure`
- `congestion`
- `export_data`
- `objectives`
- `obv`
- `patterns`
- `rsi`
- `signals`
- `sma_long`
- `sma_medium`
- `sma_short`
- `summary`
- `support_resistance`

### `module`

- `version`
- `version_major`
- `version_minor`
- `version_patch`

### `MovingAverage`

- `__str__`
- `has_value`
- `period`
- `set_period`
- `value`
- `values`
- `values_copy`

### `OHLC`


### `OnBalanceVolume`

- `__str__`
- `has_value`
- `value`
- `values`
- `values_copy`

### `Pattern`

- `is_bullish`

### `PatternRecognizer`

- `__str__`
- `bearish_count`
- `bearish_patterns`
- `bullish_count`
- `bullish_patterns`
- `has_pattern`
- `latest_pattern`
- `pattern_count`
- `patterns`
- `patterns_copy`
- `patterns_of_type`

### `PriceObjective`


### `PriceObjectiveCalculator`

- `__str__`
- `bearish_objectives`
- `bearish_targets`
- `bullish_objectives`
- `bullish_targets`
- `latest`
- `objectives`
- `objectives_copy`

### `RSI`

- `__str__`
- `has_value`
- `is_overbought`
- `is_overbought_custom`
- `is_oversold`
- `is_oversold_custom`
- `overbought_threshold`
- `oversold_threshold`
- `period`
- `set_period`
- `set_thresholds`
- `value`
- `values`
- `values_copy`

### `Signal`


### `SignalDetector`

- `__str__`
- `buy_count`
- `buy_signals`
- `current_signal`
- `has_buy_signal`
- `has_sell_signal`
- `last_signal`
- `sell_count`
- `sell_signals`
- `signals`
- `signals_copy`

### `SupportResistance`

- `__str__`
- `is_near_resistance`
- `is_near_support`
- `levels_copy`
- `resistance_levels`
- `resistance_prices`
- `set_threshold`
- `significant_levels`
- `support_levels`
- `support_prices`
- `threshold`

### `SupportResistanceLevel`


### `Visualization`


## Java (`com.pnf`)

Total symbols: **166**

### `Chart`

- `Chart()`
- `Chart(ChartConfig config)`
- `addData(double high, double low, double close, long timestamp)`
- `addPrice(double price, long timestamp)`
- `boxMarker(int columnIndex, int boxIndex)`
- `boxPrice(int columnIndex, int boxIndex)`
- `boxSize()`
- `boxType(int columnIndex, int boxIndex)`
- `clear()`
- `close()`
- `columnBoxCount(int index)`
- `columnCount()`
- `columnHighest(int index)`
- `columnLowest(int index)`
- `columnType(int index)`
- `hasBearishBias()`
- `hasBullishBias()`
- `oColumnCount()`
- `toAscii()`
- `toString()`
- `xColumnCount()`

### `ChartConfig`

- `Builder()`
- `ChartConfig()`
- `ChartConfig(ConstructionMethod method, BoxSizeMethod boxSizeMethod, double boxSize, int reversal)`
- `boxSize(double boxSize)`
- `boxSizeMethod(BoxSizeMethod boxSizeMethod)`
- `build()`
- `builder()`
- `getDefault()`
- `method(ConstructionMethod method)`
- `reversal(int reversal)`

### `CongestionZone`

- `CongestionZone(int startColumn, int endColumn, double highPrice, double lowPrice, int columnCount)`
- `record CongestionZone(int startColumn, int endColumn, double highPrice, double lowPrice, int columnCount)`
- `toString()`

### `DashboardServer`

- `DashboardServer()`
- `DashboardServer(final Chart chart, final Indicators indicators)`
- `buildSnapshotJson(final Chart chart, final Indicators indicators, final long sequence)`
- `close()`
- `publish()`
- `setChart(final Chart chart)`
- `setIndicators(final Indicators indicators)`
- `snapshotJson()`
- `startAutoPublish()`
- `startAutoPublish(final long intervalMs)`
- `stop()`
- `stopAutoPublish()`
- `url()`

### `IndicatorConfig`

- `Builder()`
- `IndicatorConfig()`
- `IndicatorConfig(int smaShortPeriod, int smaMediumPeriod, int smaLongPeriod)`
- `bearishAlertThreshold(double threshold)`
- `bollingerPeriod(int period)`
- `bollingerStdDevs(double stdDevs)`
- `build()`
- `builder()`
- `bullishAlertThreshold(double threshold)`
- `congestionMinColumns(int columns)`
- `congestionPriceRange(double range)`
- `defaults()`
- `getDefault()`
- `rsiOverbought(double threshold)`
- `rsiOversold(double threshold)`
- `rsiPeriod(int period)`
- `smaLongPeriod(int period)`
- `smaMediumPeriod(int period)`
- `smaShortPeriod(int period)`
- `supportResistanceThreshold(double threshold)`
- `withBearishAlertThreshold(double threshold)`
- `withBollingerPeriod(int period)`
- `withBollingerStdDevs(double stdDevs)`
- `withBullishAlertThreshold(double threshold)`
- `withCongestionMinColumns(int columns)`
- `withCongestionPriceRange(double range)`
- `withRsiOverbought(double threshold)`
- `withRsiOversold(double threshold)`
- `withRsiPeriod(int period)`
- `withSmaLongPeriod(int period)`
- `withSmaMediumPeriod(int period)`
- `withSmaShortPeriod(int period)`
- `withSupportResistanceThreshold(double threshold)`

### `Indicators`

- `Indicators()`
- `Indicators(IndicatorConfig config)`
- `bearishObjectives()`
- `bearishPatternCount()`
- `bearishPatterns()`
- `bearishTargets()`
- `bollingerLower(int column)`
- `bollingerLowerValues()`
- `bollingerMiddle(int column)`
- `bollingerMiddleValues()`
- `bollingerUpper(int column)`
- `bollingerUpperValues()`
- `bullishObjectives()`
- `bullishPatternCount()`
- `bullishPatterns()`
- `bullishPercent()`
- `bullishTargets()`
- `buySignalCount()`
- `buySignals()`
- `calculate(Chart chart)`
- `close()`
- `configure(IndicatorConfig config)`
- `congestionZoneCount()`
- `congestionZones()`
- `currentSignal()`
- `getConfig()`
- `hasBollingerValue(int column)`
- `hasBuySignal()`
- `hasPattern(PatternType type)`
- `hasRsiValue(int column)`
- `hasSellSignal()`
- `isAboveUpperBand(int column, double price)`
- `isBearishAlert()`
- `isBelowLowerBand(int column, double price)`
- `isBullishAlert()`
- `isInCongestion(int column)`
- `isNearResistance(double price, double tolerance)`
- `isNearSupport(double price, double tolerance)`
- `isOverbought(int column)`
- `isOversold(int column)`
- `largestCongestionZone()`
- `levels()`
- `obv(int column)`
- `obvValues()`
- `patternCount()`
- `patterns()`
- `priceObjectives()`
- `resistanceLevelCount()`
- `resistanceLevels()`
- `resistancePrices()`
- `rsi(int column)`
- `rsiValues()`
- `sellSignalCount()`
- `sellSignals()`
- `setBollingerParams(int period, double stdDevs)`
- `setRsiParams(int period, double overbought, double oversold)`
- `setSmaPeriods(int shortPeriod, int mediumPeriod, int longPeriod)`
- `signalCount()`
- `signals()`
- `significantLevels(int minTouches)`
- `sma10(int column)`
- `sma20(int column)`
- `sma5(int column)`
- `smaLong(int column)`
- `smaLongValues()`
- `smaMedium(int column)`
- `smaMediumValues()`
- `smaShort(int column)`
- `smaShortValues()`
- `summary()`
- `supportLevelCount()`
- `supportLevels()`
- `supportPrices()`
- `toString()`

### `Pattern`

- `Pattern(PatternType type, int columnIndex, double price, boolean isBullish)`
- `record Pattern(PatternType type, int columnIndex, double price, boolean isBullish)`
- `toString()`

### `PriceObjective`

- `PriceObjective(double target, int columnIndex, boolean isBullish)`
- `record PriceObjective(double target, int columnIndex, boolean isBullish)`
- `toString()`

### `Signal`

- `Signal(SignalType type, int columnIndex, double price)`
- `record Signal(SignalType type, int columnIndex, double price)`
- `toString()`

### `SupportResistanceLevel`

- `SupportResistanceLevel(LevelType type, double price, int touches, int firstColumn, int lastColumn)`
- `record SupportResistanceLevel(LevelType type, double price, int touches, int firstColumn, int lastColumn)`
- `toString()`

## Rust (`pnf`)

Total symbols: **220**

### `chart`

- `add_data(&mut self, high: f64, low: f64, close: f64, timestamp: i64)`
- `add_ohlc(&mut self, ohlc: &OHLC)`
- `add_price(&mut self, price: f64, timestamp: i64)`
- `box_marker(&self, col_index: usize, box_index: usize)`
- `box_price(&self, col_index: usize, box_index: usize)`
- `box_size(&self)`
- `box_type(&self, col_index: usize, box_index: usize)`
- `clear(&mut self)`
- `column_box_count(&self, index: usize)`
- `column_count(&self)`
- `column_highest(&self, index: usize)`
- `column_lowest(&self, index: usize)`
- `column_type(&self, index: usize)`
- `has_bearish_bias(&self)`
- `has_bullish_bias(&self)`
- `is_above_bullish_support(&self, price: f64)`
- `is_below_bearish_resistance(&self, price: f64)`
- `new()`
- `o_column_count(&self)`
- `struct Chart`
- `to_ascii(&self)`
- `to_json(&self)`
- `to_string(&self)`
- `with_config(config: ChartConfig)`
- `x_column_count(&self)`

### `dashboard`

- `build_snapshot_json(chart: &Chart, indicators: Option<&Indicators>, sequence: u64)`
- `new()`
- `publish(&self, chart: &Chart, indicators: Option<&Indicators>)`
- `snapshot_json(&self)`
- `start(&mut self, host: &str, port: u16)`
- `stop(&mut self)`
- `struct DashboardServer`
- `url(&self)`

### `ffi`

- `enum PnfBoxSizeMethod`
- `enum PnfBoxType`
- `enum PnfColumnType`
- `enum PnfConstructionMethod`
- `enum PnfPatternType`
- `enum PnfSignalType`
- `pnf_chart_add_data(chart: *mut PnfChart, high: f64, low: f64, close: f64, timestamp: i64,)`
- `pnf_chart_add_ohlc(chart: *mut PnfChart, ohlc: *const PnfOHLC)`
- `pnf_chart_add_price(chart: *mut PnfChart, price: f64, timestamp: i64)`
- `pnf_chart_box_marker(chart: *const PnfChart, col_index: usize, box_index: usize,)`
- `pnf_chart_box_price(chart: *const PnfChart, col_index: usize, box_index: usize,)`
- `pnf_chart_box_size(chart: *const PnfChart)`
- `pnf_chart_box_type(chart: *const PnfChart, col_index: usize, box_index: usize,)`
- `pnf_chart_clear(chart: *mut PnfChart)`
- `pnf_chart_column_box_count(chart: *const PnfChart, index: usize)`
- `pnf_chart_column_count(chart: *const PnfChart)`
- `pnf_chart_column_highest(chart: *const PnfChart, index: usize)`
- `pnf_chart_column_lowest(chart: *const PnfChart, index: usize)`
- `pnf_chart_column_type(chart: *const PnfChart, index: usize)`
- `pnf_chart_config_default()`
- `pnf_chart_create(config: *const PnfChartConfig)`
- `pnf_chart_create_default()`
- `pnf_chart_destroy(chart: *mut PnfChart)`
- `pnf_chart_has_bearish_bias(chart: *const PnfChart)`
- `pnf_chart_has_bullish_bias(chart: *const PnfChart)`
- `pnf_chart_is_above_bullish_support(chart: *const PnfChart, price: f64)`
- `pnf_chart_is_below_bearish_resistance(chart: *const PnfChart, price: f64)`
- `pnf_chart_o_column_count(chart: *const PnfChart)`
- `pnf_chart_to_ascii(chart: *const PnfChart)`
- `pnf_chart_to_json(chart: *const PnfChart)`
- `pnf_chart_to_string(chart: *const PnfChart)`
- `pnf_chart_x_column_count(chart: *const PnfChart)`
- `pnf_free_double_array(arr: PnfDoubleArray)`
- `pnf_free_level_array(arr: PnfLevelArray)`
- `pnf_free_pattern_array(arr: PnfPatternArray)`
- `pnf_free_signal_array(arr: PnfSignalArray)`
- `pnf_free_string(s: *const c_char)`
- `pnf_indicator_config_default()`
- `pnf_indicators_bearish_pattern_count(ind: *const PnfIndicators)`
- `pnf_indicators_bearish_patterns(ind: *const PnfIndicators)`
- `pnf_indicators_bollinger_lower(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_bollinger_lower_values(ind: *const PnfIndicators)`
- `pnf_indicators_bollinger_middle(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_bollinger_middle_values(ind: *const PnfIndicators)`
- `pnf_indicators_bollinger_upper(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_bollinger_upper_values(ind: *const PnfIndicators)`
- `pnf_indicators_bullish_pattern_count(ind: *const PnfIndicators)`
- `pnf_indicators_bullish_patterns(ind: *const PnfIndicators)`
- `pnf_indicators_bullish_percent(ind: *const PnfIndicators)`
- `pnf_indicators_buy_signal_count(ind: *const PnfIndicators)`
- `pnf_indicators_calculate(indicators: *mut PnfIndicators, chart: *const PnfChart)`
- `pnf_indicators_configure(ind: *mut PnfIndicators, config: *const PnfIndicatorConfig)`
- `pnf_indicators_congestion_zone_count(ind: *const PnfIndicators)`
- `pnf_indicators_create()`
- `pnf_indicators_create_default()`
- `pnf_indicators_create_with_config(config: *const PnfIndicatorConfig)`
- `pnf_indicators_current_signal(ind: *const PnfIndicators)`
- `pnf_indicators_destroy(indicators: *mut PnfIndicators)`
- `pnf_indicators_is_bearish_alert(ind: *const PnfIndicators)`
- `pnf_indicators_is_bullish_alert(ind: *const PnfIndicators)`
- `pnf_indicators_is_in_congestion(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_is_near_resistance(ind: *const PnfIndicators, price: f64, tolerance: f64,)`
- `pnf_indicators_is_near_support(ind: *const PnfIndicators, price: f64, tolerance: f64,)`
- `pnf_indicators_obv(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_obv_values(ind: *const PnfIndicators)`
- `pnf_indicators_pattern_at(ind: *const PnfIndicators, index: usize)`
- `pnf_indicators_pattern_count(ind: *const PnfIndicators)`
- `pnf_indicators_patterns(ind: *const PnfIndicators)`
- `pnf_indicators_resistance_level_count(ind: *const PnfIndicators)`
- `pnf_indicators_resistance_levels(ind: *const PnfIndicators)`
- `pnf_indicators_resistance_prices(ind: *const PnfIndicators)`
- `pnf_indicators_rsi(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_rsi_is_overbought(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_rsi_is_oversold(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_rsi_values(ind: *const PnfIndicators)`
- `pnf_indicators_sell_signal_count(ind: *const PnfIndicators)`
- `pnf_indicators_set_bollinger_params(ind: *mut PnfIndicators, period: c_int, std_devs: f64,)`
- `pnf_indicators_set_bullish_percent_thresholds(ind: *mut PnfIndicators, bullish: f64, bearish: f64,)`
- `pnf_indicators_set_congestion_params(ind: *mut PnfIndicators, min_columns: c_int, price_range: f64,)`
- `pnf_indicators_set_rsi_params(ind: *mut PnfIndicators, period: c_int, overbought: f64, oversold: f64,)`
- `pnf_indicators_set_sma_periods(ind: *mut PnfIndicators, short_period: c_int, medium_period: c_int, long_period: c_int,)`
- `pnf_indicators_set_support_resistance_threshold(ind: *mut PnfIndicators, threshold: f64)`
- `pnf_indicators_signal_at(ind: *const PnfIndicators, index: usize)`
- `pnf_indicators_signal_count(ind: *const PnfIndicators)`
- `pnf_indicators_signals(ind: *const PnfIndicators)`
- `pnf_indicators_sma_long(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_sma_long_values(ind: *const PnfIndicators)`
- `pnf_indicators_sma_medium(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_sma_medium_values(ind: *const PnfIndicators)`
- `pnf_indicators_sma_short(ind: *const PnfIndicators, column: c_int)`
- `pnf_indicators_sma_short_values(ind: *const PnfIndicators)`
- `pnf_indicators_summary(ind: *const PnfIndicators)`
- `pnf_indicators_support_level_count(ind: *const PnfIndicators)`
- `pnf_indicators_support_levels(ind: *const PnfIndicators)`
- `pnf_indicators_support_prices(ind: *const PnfIndicators)`
- `pnf_indicators_to_string(ind: *const PnfIndicators)`
- `pnf_version_major()`
- `pnf_version_minor()`
- `pnf_version_patch()`
- `pnf_version_string()`
- `struct PnfChart`
- `struct PnfChartConfig`
- `struct PnfColumnData`
- `struct PnfDoubleArray`
- `struct PnfIndicatorConfig`
- `struct PnfIndicators`
- `struct PnfLevelArray`
- `struct PnfOHLC`
- `struct PnfPattern`
- `struct PnfPatternArray`
- `struct PnfSignal`
- `struct PnfSignalArray`
- `struct PnfSupportResistanceLevel`
- `struct PnfViewer`

### `indicators`

- `bearish_pattern_count(&self)`
- `bearish_patterns(&self)`
- `bollinger_lower(&self, column: i32)`
- `bollinger_lower_values(&self)`
- `bollinger_middle(&self, column: i32)`
- `bollinger_middle_values(&self)`
- `bollinger_upper(&self, column: i32)`
- `bollinger_upper_values(&self)`
- `bullish_pattern_count(&self)`
- `bullish_patterns(&self)`
- `bullish_percent(&self)`
- `buy_signal_count(&self)`
- `calculate(&mut self, chart: &Chart)`
- `configure(&mut self, config: IndicatorConfig)`
- `congestion_zone_count(&self)`
- `current_signal(&self)`
- `is_bearish_alert(&self)`
- `is_bullish_alert(&self)`
- `is_in_congestion(&self, column: i32)`
- `is_near_resistance(&self, price: f64, tolerance: f64)`
- `is_near_support(&self, price: f64, tolerance: f64)`
- `new()`
- `obv(&self, column: i32)`
- `obv_values(&self)`
- `pattern_count(&self)`
- `patterns(&self)`
- `resistance_level_count(&self)`
- `resistance_levels(&self)`
- `resistance_prices(&self)`
- `rsi(&self, column: i32)`
- `rsi_is_overbought(&self, column: i32)`
- `rsi_is_oversold(&self, column: i32)`
- `rsi_values(&self)`
- `sell_signal_count(&self)`
- `set_bollinger_params(&mut self, period: i32, std_devs: f64)`
- `set_bullish_percent_thresholds(&mut self, bullish: f64, bearish: f64)`
- `set_congestion_params(&mut self, min_columns: i32, price_range: f64)`
- `set_rsi_params(&mut self, period: i32, overbought: f64, oversold: f64)`
- `set_sma_periods(&mut self, short: i32, medium: i32, long: i32)`
- `set_support_resistance_threshold(&mut self, threshold: f64)`
- `signal_count(&self)`
- `signals(&self)`
- `sma_long(&self, column: i32)`
- `sma_long_values(&self)`
- `sma_medium(&self, column: i32)`
- `sma_medium_values(&self)`
- `sma_short(&self, column: i32)`
- `sma_short_values(&self)`
- `struct Indicators`
- `summary(&self)`
- `support_level_count(&self)`
- `support_levels(&self)`
- `support_prices(&self)`
- `to_string(&self)`
- `with_config(config: IndicatorConfig)`

### `lib`

- `version()`
- `version_major()`
- `version_minor()`
- `version_patch()`

### `types`

- `enum BoxSizeMethod`
- `enum BoxType`
- `enum ColumnType`
- `enum ConstructionMethod`
- `enum PatternType`
- `enum SignalType`
- `new()`
- `struct ChartConfig`
- `struct ColumnData`
- `struct IndicatorConfig`
- `struct OHLC`
- `struct Pattern`
- `struct Signal`
- `struct SupportResistanceLevel`

## C# (`PnF`)

Total symbols: **190**

### `Chart`

- `AddData(double high, double low, double close, long timestamp)`
- `AddOHLC(OHLC ohlc)`
- `AddPrice(double price, long timestamp)`
- `Chart()`
- `Chart(ChartConfig config)`
- `Clear()`
- `Dispose()`
- `GetBoxMarker(int columnIndex, int boxIndex)`
- `GetBoxPrice(int columnIndex, int boxIndex)`
- `GetBoxType(int columnIndex, int boxIndex)`
- `GetColumnBoxCount(int index)`
- `GetColumnHighest(int index)`
- `GetColumnLowest(int index)`
- `GetColumnType(int index)`
- `IsAboveBullishSupport(double price)`
- `IsBelowBearishResistance(double price)`
- `ToAscii()`
- `ToJson()`
- `ToString()`

### `DashboardServer`

- `Chart (property)`
- `DashboardServer()`
- `DashboardServer(Chart chart, Indicators indicators)`
- `Dispose()`
- `Host (property)`
- `Indicators (property)`
- `Port (property)`
- `Publish()`
- `SetChart(Chart chart)`
- `SetIndicators(Indicators indicators)`
- `SnapshotJson()`
- `Start(string host = "127.0.0.1", int port = 0)`
- `StartAutoPublish(int intervalMs = 250)`
- `Stop()`
- `StopAutoPublish()`
- `Url()`

### `Indicators`

- `BollingerLower(int column)`
- `BollingerLowerValues()`
- `BollingerMiddle(int column)`
- `BollingerMiddleValues()`
- `BollingerUpper(int column)`
- `BollingerUpperValues()`
- `Calculate(Chart chart)`
- `Configure(IndicatorConfig config)`
- `Dispose()`
- `GetBearishPatterns()`
- `GetBullishPatterns()`
- `GetPatterns()`
- `GetResistanceLevels()`
- `GetResistancePrices()`
- `GetSignals()`
- `GetSupportLevels()`
- `GetSupportPrices()`
- `Indicators()`
- `Indicators(IndicatorConfig config)`
- `IsInCongestion(int column)`
- `IsNearResistance(double price, double tolerance)`
- `IsNearSupport(double price, double tolerance)`
- `Obv(int column)`
- `ObvValues()`
- `Rsi(int column)`
- `RsiIsOverbought(int column)`
- `RsiIsOversold(int column)`
- `RsiValues()`
- `SetBollingerParams(int period, double stdDevs)`
- `SetBullishPercentThresholds(double bullish, double bearish)`
- `SetCongestionParams(int minColumns, double priceRange)`
- `SetRsiParams(int period, double overbought, double oversold)`
- `SetSmaPeriods(int shortPeriod, int mediumPeriod, int longPeriod)`
- `SetSupportResistanceThreshold(double threshold)`
- `SmaLong(int column)`
- `SmaLongValues()`
- `SmaMedium(int column)`
- `SmaMediumValues()`
- `SmaShort(int column)`
- `SmaShortValues()`
- `Summary()`
- `ToString()`

### `NativeMethods`

- `pnf_chart_add_data(IntPtr chart, double high, double low, double close, long timestamp)`
- `pnf_chart_add_ohlc(IntPtr chart, ref OHLC ohlc)`
- `pnf_chart_add_price(IntPtr chart, double price, long timestamp)`
- `pnf_chart_box_marker(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex)`
- `pnf_chart_box_price(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex)`
- `pnf_chart_box_size(IntPtr chart)`
- `pnf_chart_box_type(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex)`
- `pnf_chart_clear(IntPtr chart)`
- `pnf_chart_column_box_count(IntPtr chart, UIntPtr index)`
- `pnf_chart_column_count(IntPtr chart)`
- `pnf_chart_column_highest(IntPtr chart, UIntPtr index)`
- `pnf_chart_column_lowest(IntPtr chart, UIntPtr index)`
- `pnf_chart_column_type(IntPtr chart, UIntPtr index)`
- `pnf_chart_config_default()`
- `pnf_chart_create(ref ChartConfig config)`
- `pnf_chart_create_default()`
- `pnf_chart_destroy(IntPtr chart)`
- `pnf_chart_has_bearish_bias(IntPtr chart)`
- `pnf_chart_has_bullish_bias(IntPtr chart)`
- `pnf_chart_is_above_bullish_support(IntPtr chart, double price)`
- `pnf_chart_is_below_bearish_resistance(IntPtr chart, double price)`
- `pnf_chart_o_column_count(IntPtr chart)`
- `pnf_chart_to_ascii(IntPtr chart)`
- `pnf_chart_to_json(IntPtr chart)`
- `pnf_chart_to_string(IntPtr chart)`
- `pnf_chart_x_column_count(IntPtr chart)`
- `pnf_free_double_array(PnfDoubleArray arr)`
- `pnf_free_level_array(PnfLevelArray arr)`
- `pnf_free_pattern_array(PnfPatternArray arr)`
- `pnf_free_signal_array(PnfSignalArray arr)`
- `pnf_free_string(IntPtr str)`
- `pnf_indicator_config_default()`
- `pnf_indicators_bearish_pattern_count(IntPtr indicators)`
- `pnf_indicators_bearish_patterns(IntPtr indicators)`
- `pnf_indicators_bollinger_lower(IntPtr indicators, int column)`
- `pnf_indicators_bollinger_lower_values(IntPtr indicators)`
- `pnf_indicators_bollinger_middle(IntPtr indicators, int column)`
- `pnf_indicators_bollinger_middle_values(IntPtr indicators)`
- `pnf_indicators_bollinger_upper(IntPtr indicators, int column)`
- `pnf_indicators_bollinger_upper_values(IntPtr indicators)`
- `pnf_indicators_bullish_pattern_count(IntPtr indicators)`
- `pnf_indicators_bullish_patterns(IntPtr indicators)`
- `pnf_indicators_bullish_percent(IntPtr indicators)`
- `pnf_indicators_buy_signal_count(IntPtr indicators)`
- `pnf_indicators_calculate(IntPtr indicators, IntPtr chart)`
- `pnf_indicators_configure(IntPtr indicators, ref IndicatorConfig config)`
- `pnf_indicators_congestion_zone_count(IntPtr indicators)`
- `pnf_indicators_create()`
- `pnf_indicators_create_default()`
- `pnf_indicators_create_with_config(ref IndicatorConfig config)`
- `pnf_indicators_current_signal(IntPtr indicators)`
- `pnf_indicators_destroy(IntPtr indicators)`
- `pnf_indicators_is_bearish_alert(IntPtr indicators)`
- `pnf_indicators_is_bullish_alert(IntPtr indicators)`
- `pnf_indicators_is_in_congestion(IntPtr indicators, int column)`
- `pnf_indicators_is_near_resistance(IntPtr indicators, double price, double tolerance)`
- `pnf_indicators_is_near_support(IntPtr indicators, double price, double tolerance)`
- `pnf_indicators_obv(IntPtr indicators, int column)`
- `pnf_indicators_obv_values(IntPtr indicators)`
- `pnf_indicators_pattern_at(IntPtr indicators, UIntPtr index)`
- `pnf_indicators_pattern_count(IntPtr indicators)`
- `pnf_indicators_patterns(IntPtr indicators)`
- `pnf_indicators_resistance_level_count(IntPtr indicators)`
- `pnf_indicators_resistance_levels(IntPtr indicators)`
- `pnf_indicators_resistance_prices(IntPtr indicators)`
- `pnf_indicators_rsi(IntPtr indicators, int column)`
- `pnf_indicators_rsi_is_overbought(IntPtr indicators, int column)`
- `pnf_indicators_rsi_is_oversold(IntPtr indicators, int column)`
- `pnf_indicators_rsi_values(IntPtr indicators)`
- `pnf_indicators_sell_signal_count(IntPtr indicators)`
- `pnf_indicators_set_bollinger_params(IntPtr indicators, int period, double stdDevs)`
- `pnf_indicators_set_bullish_percent_thresholds(IntPtr indicators, double bullish, double bearish)`
- `pnf_indicators_set_congestion_params(IntPtr indicators, int minColumns, double priceRange)`
- `pnf_indicators_set_rsi_params(IntPtr indicators, int period, double overbought, double oversold)`
- `pnf_indicators_set_sma_periods(IntPtr indicators, int shortPeriod, int mediumPeriod, int longPeriod)`
- `pnf_indicators_set_support_resistance_threshold(IntPtr indicators, double threshold)`
- `pnf_indicators_signal_at(IntPtr indicators, UIntPtr index)`
- `pnf_indicators_signal_count(IntPtr indicators)`
- `pnf_indicators_signals(IntPtr indicators)`
- `pnf_indicators_sma_long(IntPtr indicators, int column)`
- `pnf_indicators_sma_long_values(IntPtr indicators)`
- `pnf_indicators_sma_medium(IntPtr indicators, int column)`
- `pnf_indicators_sma_medium_values(IntPtr indicators)`
- `pnf_indicators_sma_short(IntPtr indicators, int column)`
- `pnf_indicators_sma_short_values(IntPtr indicators)`
- `pnf_indicators_summary(IntPtr indicators)`
- `pnf_indicators_support_level_count(IntPtr indicators)`
- `pnf_indicators_support_levels(IntPtr indicators)`
- `pnf_indicators_support_prices(IntPtr indicators)`
- `pnf_indicators_to_string(IntPtr indicators)`
- `pnf_version_major()`
- `pnf_version_minor()`
- `pnf_version_patch()`
- `pnf_version_string()`

### `Types`

- `ColumnIndex (property)`
- `Default()`
- `EndColumn (property)`
- `IsBullish (property)`
- `IsSupport (property)`
- `Price (property)`
- `StartColumn (property)`
- `ToString()`
- `TouchCount (property)`
- `Type (property)`
- `enum BoxSizeMethod`
- `enum BoxType`
- `enum ColumnType`
- `enum ConstructionMethod`
- `enum PatternType`
- `enum SignalType`
- `struct ChartConfig`
- `struct IndicatorConfig`
- `struct OHLC`
