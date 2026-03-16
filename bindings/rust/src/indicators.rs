//! Safe wrapper around native indicator calculations and result accessors.
use crate::chart::Chart;
use crate::ffi;
use crate::types::*;
use std::slice;

/// Technical indicators for Point & Figure charts.
pub struct Indicators {
    ptr: *mut ffi::PnfIndicators,
}

impl Indicators {
    /// Creates new indicators with default configuration.
    pub fn new() -> Self {
        let ptr = unsafe { ffi::pnf_indicators_create_default() };
        Self { ptr }
    }

    /// Creates new indicators with the specified configuration.
    pub fn with_config(config: IndicatorConfig) -> Self {
        let ffi_config = config.to_ffi();
        let ptr = unsafe { ffi::pnf_indicators_create_with_config(&ffi_config) };
        Self { ptr }
    }

    /// Configures the indicators with new settings.
    pub fn configure(&mut self, config: IndicatorConfig) {
        let ffi_config = config.to_ffi();
        unsafe { ffi::pnf_indicators_configure(self.ptr, &ffi_config) }
    }

    /// Sets the periods for simple moving averages.
    pub fn set_sma_periods(&mut self, short: i32, medium: i32, long: i32) {
        unsafe { ffi::pnf_indicators_set_sma_periods(self.ptr, short, medium, long) }
    }

    /// Sets the parameters for Bollinger Bands.
    pub fn set_bollinger_params(&mut self, period: i32, std_devs: f64) {
        unsafe { ffi::pnf_indicators_set_bollinger_params(self.ptr, period, std_devs) }
    }

    /// Sets the parameters for RSI.
    pub fn set_rsi_params(&mut self, period: i32, overbought: f64, oversold: f64) {
        unsafe { ffi::pnf_indicators_set_rsi_params(self.ptr, period, overbought, oversold) }
    }

    /// Sets the bullish percent alert thresholds.
    pub fn set_bullish_percent_thresholds(&mut self, bullish: f64, bearish: f64) {
        unsafe { ffi::pnf_indicators_set_bullish_percent_thresholds(self.ptr, bullish, bearish) }
    }

    /// Sets the threshold for merging support/resistance levels.
    pub fn set_support_resistance_threshold(&mut self, threshold: f64) {
        unsafe { ffi::pnf_indicators_set_support_resistance_threshold(self.ptr, threshold) }
    }

    /// Sets the parameters for congestion zone detection.
    pub fn set_congestion_params(&mut self, min_columns: i32, price_range: f64) {
        unsafe { ffi::pnf_indicators_set_congestion_params(self.ptr, min_columns, price_range) }
    }

    /// Calculates all indicators for the given chart.
    pub fn calculate(&mut self, chart: &Chart) {
        unsafe { ffi::pnf_indicators_calculate(self.ptr, chart.as_ptr()) }
    }

    /// Returns the short-term SMA value for a specific column.
    pub fn sma_short(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_sma_short(self.ptr, column) }
    }

    /// Returns the medium-term SMA value for a specific column.
    pub fn sma_medium(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_sma_medium(self.ptr, column) }
    }

    /// Returns the long-term SMA value for a specific column.
    pub fn sma_long(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_sma_long(self.ptr, column) }
    }

    /// Returns all short-term SMA values.
    pub fn sma_short_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_sma_short_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns all medium-term SMA values.
    pub fn sma_medium_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_sma_medium_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns all long-term SMA values.
    pub fn sma_long_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_sma_long_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns the Bollinger Bands middle value for a specific column.
    pub fn bollinger_middle(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_bollinger_middle(self.ptr, column) }
    }

    /// Returns the Bollinger Bands upper value for a specific column.
    pub fn bollinger_upper(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_bollinger_upper(self.ptr, column) }
    }

    /// Returns the Bollinger Bands lower value for a specific column.
    pub fn bollinger_lower(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_bollinger_lower(self.ptr, column) }
    }

    /// Returns all Bollinger Bands middle values.
    pub fn bollinger_middle_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_bollinger_middle_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns all Bollinger Bands upper values.
    pub fn bollinger_upper_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_bollinger_upper_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns all Bollinger Bands lower values.
    pub fn bollinger_lower_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_bollinger_lower_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns the RSI value for a specific column.
    pub fn rsi(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_rsi(self.ptr, column) }
    }

    /// Checks if RSI is in overbought territory for a specific column.
    pub fn rsi_is_overbought(&self, column: i32) -> bool {
        unsafe { ffi::pnf_indicators_rsi_is_overbought(self.ptr, column) }
    }

    /// Checks if RSI is in oversold territory for a specific column.
    pub fn rsi_is_oversold(&self, column: i32) -> bool {
        unsafe { ffi::pnf_indicators_rsi_is_oversold(self.ptr, column) }
    }

    /// Returns all RSI values.
    pub fn rsi_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_rsi_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns the OBV value for a specific column.
    pub fn obv(&self, column: i32) -> f64 {
        unsafe { ffi::pnf_indicators_obv(self.ptr, column) }
    }

    /// Returns all OBV values.
    pub fn obv_values(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_obv_values(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns the bullish percent value.
    pub fn bullish_percent(&self) -> f64 {
        unsafe { ffi::pnf_indicators_bullish_percent(self.ptr) }
    }

    /// Checks if there is a bullish alert.
    pub fn is_bullish_alert(&self) -> bool {
        unsafe { ffi::pnf_indicators_is_bullish_alert(self.ptr) }
    }

    /// Checks if there is a bearish alert.
    pub fn is_bearish_alert(&self) -> bool {
        unsafe { ffi::pnf_indicators_is_bearish_alert(self.ptr) }
    }

    /// Returns the current signal type.
    pub fn current_signal(&self) -> SignalType {
        let st = unsafe { ffi::pnf_indicators_current_signal(self.ptr) };
        SignalType::from_ffi(st)
    }

    /// Returns the total number of signals.
    pub fn signal_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_signal_count(self.ptr) }
    }

    /// Returns all detected signals.
    pub fn signals(&self) -> Vec<Signal> {
        let arr = unsafe { ffi::pnf_indicators_signals(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.iter().map(|&s| Signal::from_ffi(s)).collect();
        unsafe { ffi::pnf_free_signal_array(arr) };
        result
    }

    /// Returns the number of buy signals.
    pub fn buy_signal_count(&self) -> i32 {
        unsafe { ffi::pnf_indicators_buy_signal_count(self.ptr) }
    }

    /// Returns the number of sell signals.
    pub fn sell_signal_count(&self) -> i32 {
        unsafe { ffi::pnf_indicators_sell_signal_count(self.ptr) }
    }

    /// Returns the total number of patterns.
    pub fn pattern_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_pattern_count(self.ptr) }
    }

    /// Returns the number of bullish patterns.
    pub fn bullish_pattern_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_bullish_pattern_count(self.ptr) }
    }

    /// Returns the number of bearish patterns.
    pub fn bearish_pattern_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_bearish_pattern_count(self.ptr) }
    }

    /// Returns all detected patterns.
    pub fn patterns(&self) -> Vec<Pattern> {
        let arr = unsafe { ffi::pnf_indicators_patterns(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.iter().map(|&p| Pattern::from_ffi(p)).collect();
        unsafe { ffi::pnf_free_pattern_array(arr) };
        result
    }

    /// Returns all bullish patterns.
    pub fn bullish_patterns(&self) -> Vec<Pattern> {
        let arr = unsafe { ffi::pnf_indicators_bullish_patterns(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.iter().map(|&p| Pattern::from_ffi(p)).collect();
        unsafe { ffi::pnf_free_pattern_array(arr) };
        result
    }

    /// Returns all bearish patterns.
    pub fn bearish_patterns(&self) -> Vec<Pattern> {
        let arr = unsafe { ffi::pnf_indicators_bearish_patterns(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.iter().map(|&p| Pattern::from_ffi(p)).collect();
        unsafe { ffi::pnf_free_pattern_array(arr) };
        result
    }

    /// Returns the number of support levels.
    pub fn support_level_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_support_level_count(self.ptr) }
    }

    /// Returns the number of resistance levels.
    pub fn resistance_level_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_resistance_level_count(self.ptr) }
    }

    /// Checks if the price is near a support level.
    pub fn is_near_support(&self, price: f64, tolerance: f64) -> bool {
        unsafe { ffi::pnf_indicators_is_near_support(self.ptr, price, tolerance) }
    }

    /// Checks if the price is near a resistance level.
    pub fn is_near_resistance(&self, price: f64, tolerance: f64) -> bool {
        unsafe { ffi::pnf_indicators_is_near_resistance(self.ptr, price, tolerance) }
    }

    /// Returns all support levels.
    pub fn support_levels(&self) -> Vec<SupportResistanceLevel> {
        let arr = unsafe { ffi::pnf_indicators_support_levels(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice
            .iter()
            .map(|&l| SupportResistanceLevel::from_ffi(l))
            .collect();
        unsafe { ffi::pnf_free_level_array(arr) };
        result
    }

    /// Returns all resistance levels.
    pub fn resistance_levels(&self) -> Vec<SupportResistanceLevel> {
        let arr = unsafe { ffi::pnf_indicators_resistance_levels(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice
            .iter()
            .map(|&l| SupportResistanceLevel::from_ffi(l))
            .collect();
        unsafe { ffi::pnf_free_level_array(arr) };
        result
    }

    /// Returns all support prices.
    pub fn support_prices(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_support_prices(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Returns all resistance prices.
    pub fn resistance_prices(&self) -> Vec<f64> {
        let arr = unsafe { ffi::pnf_indicators_resistance_prices(self.ptr) };
        let slice = unsafe { slice::from_raw_parts(arr.data, arr.length) };
        let result = slice.to_vec();
        unsafe { ffi::pnf_free_double_array(arr) };
        result
    }

    /// Checks if a column is in a congestion zone.
    pub fn is_in_congestion(&self, column: i32) -> bool {
        unsafe { ffi::pnf_indicators_is_in_congestion(self.ptr, column) }
    }

    /// Returns the number of congestion zones.
    pub fn congestion_zone_count(&self) -> usize {
        unsafe { ffi::pnf_indicators_congestion_zone_count(self.ptr) }
    }

    /// Returns a summary of all indicators.
    pub fn summary(&self) -> Option<String> {
        let ptr = unsafe { ffi::pnf_indicators_summary(self.ptr) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }

    /// Returns a string representation of all indicators.
    pub fn to_string(&self) -> Option<String> {
        let ptr = unsafe { ffi::pnf_indicators_to_string(self.ptr) };
        let result = cstr_to_string(ptr);
        if !ptr.is_null() {
            unsafe { ffi::pnf_free_string(ptr) };
        }
        result
    }
}

impl Default for Indicators {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for Indicators {
    fn drop(&mut self) {
        unsafe {
            ffi::pnf_indicators_destroy(self.ptr);
        }
    }
}

unsafe impl Send for Indicators {}
unsafe impl Sync for Indicators {}
