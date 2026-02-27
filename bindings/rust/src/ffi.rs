//! ffi module for Rust bindings.
use std::os::raw::{c_char, c_int};

#[repr(C)]
/// PnfChart binding API.
pub struct PnfChart {
    _private: [u8; 0],
}

#[repr(C)]
/// PnfIndicators binding API.
pub struct PnfIndicators {
    _private: [u8; 0],
}

#[repr(C)]
#[allow(dead_code)]
/// PnfViewer binding API.
pub struct PnfViewer {
    _private: [u8; 0],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[allow(dead_code)]
/// PnfBoxType binding API.
pub enum PnfBoxType {
    X = 0,
    O = 1,
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[allow(dead_code)]
/// PnfColumnType binding API.
pub enum PnfColumnType {
    X = 0,
    O = 1,
    Mixed = 2,
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// PnfConstructionMethod binding API.
pub enum PnfConstructionMethod {
    Close = 0,
    HighLow = 1,
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// PnfBoxSizeMethod binding API.
pub enum PnfBoxSizeMethod {
    Fixed = 0,
    Traditional = 1,
    Percentage = 2,
    Points = 3,
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[allow(dead_code)]
/// PnfSignalType binding API.
pub enum PnfSignalType {
    None = 0,
    Buy = 1,
    Sell = 2,
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[allow(dead_code)]
/// PnfPatternType binding API.
pub enum PnfPatternType {
    None = 0,
    DoubleTopBreakout,
    DoubleBottomBreakdown,
    TripleTopBreakout,
    TripleBottomBreakdown,
    QuadrupleTopBreakout,
    QuadrupleBottomBreakdown,
    AscendingTripleTop,
    DescendingTripleBottom,
    BullishCatapult,
    BearishCatapult,
    BullishSignalReversed,
    BearishSignalReversed,
    BullishTriangle,
    BearishTriangle,
    LongTailDown,
    HighPole,
    LowPole,
    BullTrap,
    BearTrap,
    SpreadTripleTop,
    SpreadTripleBottom,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfChartConfig binding API.
pub struct PnfChartConfig {
    pub method: PnfConstructionMethod,
    pub box_size_method: PnfBoxSizeMethod,
    pub box_size: f64,
    pub reversal: c_int,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfIndicatorConfig binding API.
pub struct PnfIndicatorConfig {
    pub sma_short_period: c_int,
    pub sma_medium_period: c_int,
    pub sma_long_period: c_int,
    pub bollinger_period: c_int,
    pub bollinger_std_devs: f64,
    pub rsi_period: c_int,
    pub rsi_overbought: f64,
    pub rsi_oversold: f64,
    pub bullish_alert_threshold: f64,
    pub bearish_alert_threshold: f64,
    pub support_resistance_threshold: f64,
    pub congestion_min_columns: c_int,
    pub congestion_price_range: f64,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfOHLC binding API.
pub struct PnfOHLC {
    pub timestamp: i64,
    pub open: f64,
    pub high: f64,
    pub low: f64,
    pub close: f64,
    pub volume: f64,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfSignal binding API.
pub struct PnfSignal {
    pub signal_type: PnfSignalType,
    pub column_index: c_int,
    pub price: f64,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfPattern binding API.
pub struct PnfPattern {
    pub pattern_type: PnfPatternType,
    pub start_column: c_int,
    pub end_column: c_int,
    pub price: f64,
    pub is_bullish: bool,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
/// PnfSupportResistanceLevel binding API.
pub struct PnfSupportResistanceLevel {
    pub price: f64,
    pub touch_count: c_int,
    pub is_support: bool,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
#[allow(dead_code)]
/// PnfColumnData binding API.
pub struct PnfColumnData {
    pub index: c_int,
    pub column_type: PnfColumnType,
    pub high: f64,
    pub low: f64,
    pub box_count: c_int,
}

#[repr(C)]
#[derive(Debug, Clone)]
/// PnfDoubleArray binding API.
pub struct PnfDoubleArray {
    pub data: *mut f64,
    pub length: usize,
}

#[repr(C)]
#[derive(Debug, Clone)]
/// PnfSignalArray binding API.
pub struct PnfSignalArray {
    pub data: *mut PnfSignal,
    pub length: usize,
}

#[repr(C)]
#[derive(Debug, Clone)]
/// PnfPatternArray binding API.
pub struct PnfPatternArray {
    pub data: *mut PnfPattern,
    pub length: usize,
}

#[repr(C)]
#[derive(Debug, Clone)]
/// PnfLevelArray binding API.
pub struct PnfLevelArray {
    pub data: *mut PnfSupportResistanceLevel,
    pub length: usize,
}

#[link(name = "pnf")]
extern "C" {
    /// pnf_version_string binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_version_string() -> *const c_char;
    /// pnf_version_major binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_version_major() -> c_int;
    /// pnf_version_minor binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_version_minor() -> c_int;
    /// pnf_version_patch binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_version_patch() -> c_int;

    /// pnf_chart_config_default binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_config_default() -> PnfChartConfig;
    /// pnf_indicator_config_default binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_indicator_config_default() -> PnfIndicatorConfig;

    /// pnf_chart_create binding API.
    ///
    /// # Arguments
    /// * `config` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_create(config: *const PnfChartConfig) -> *mut PnfChart;
    /// pnf_chart_create_default binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_create_default() -> *mut PnfChart;
    /// pnf_chart_destroy binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    pub fn pnf_chart_destroy(chart: *mut PnfChart);

    /// pnf_chart_add_data binding API.
    pub fn pnf_chart_add_data(
        chart: *mut PnfChart,
        high: f64,
        low: f64,
        close: f64,
        timestamp: i64,
    ) -> bool;
    /// pnf_chart_add_price binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `price` - 
    /// * `timestamp` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_add_price(chart: *mut PnfChart, price: f64, timestamp: i64) -> bool;
    /// pnf_chart_add_ohlc binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `ohlc` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_add_ohlc(chart: *mut PnfChart, ohlc: *const PnfOHLC) -> bool;

    /// pnf_chart_column_count binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_column_count(chart: *const PnfChart) -> usize;
    /// pnf_chart_x_column_count binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_x_column_count(chart: *const PnfChart) -> usize;
    /// pnf_chart_o_column_count binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_o_column_count(chart: *const PnfChart) -> usize;
    /// pnf_chart_box_size binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_box_size(chart: *const PnfChart) -> f64;

    /// pnf_chart_column_type binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_column_type(chart: *const PnfChart, index: usize) -> PnfColumnType;
    /// pnf_chart_column_box_count binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_column_box_count(chart: *const PnfChart, index: usize) -> usize;
    /// pnf_chart_column_highest binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_column_highest(chart: *const PnfChart, index: usize) -> f64;
    /// pnf_chart_column_lowest binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_column_lowest(chart: *const PnfChart, index: usize) -> f64;

    /// pnf_chart_box_price binding API.
    pub fn pnf_chart_box_price(
        chart: *const PnfChart,
        col_index: usize,
        box_index: usize,
    ) -> f64;
    /// pnf_chart_box_type binding API.
    pub fn pnf_chart_box_type(
        chart: *const PnfChart,
        col_index: usize,
        box_index: usize,
    ) -> PnfBoxType;
    /// pnf_chart_box_marker binding API.
    pub fn pnf_chart_box_marker(
        chart: *const PnfChart,
        col_index: usize,
        box_index: usize,
    ) -> *const c_char;

    /// pnf_chart_has_bullish_bias binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_has_bullish_bias(chart: *const PnfChart) -> bool;
    /// pnf_chart_has_bearish_bias binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_has_bearish_bias(chart: *const PnfChart) -> bool;
    /// pnf_chart_is_above_bullish_support binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `price` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_is_above_bullish_support(chart: *const PnfChart, price: f64) -> bool;
    /// pnf_chart_is_below_bearish_resistance binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    /// * `price` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_is_below_bearish_resistance(chart: *const PnfChart, price: f64) -> bool;

    /// pnf_chart_clear binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    pub fn pnf_chart_clear(chart: *mut PnfChart);
    /// pnf_chart_to_string binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_to_string(chart: *const PnfChart) -> *const c_char;
    /// pnf_chart_to_ascii binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_to_ascii(chart: *const PnfChart) -> *const c_char;
    /// pnf_chart_to_json binding API.
    ///
    /// # Arguments
    /// * `chart` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_chart_to_json(chart: *const PnfChart) -> *const c_char;

    #[allow(dead_code)]
    /// pnf_indicators_create binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_create() -> *mut PnfIndicators;
    /// pnf_indicators_create_default binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_create_default() -> *mut PnfIndicators;
    /// pnf_indicators_create_with_config binding API.
    ///
    /// # Arguments
    /// * `config` - 
    pub fn pnf_indicators_create_with_config(config: *const PnfIndicatorConfig)
        -> *mut PnfIndicators;
    /// pnf_indicators_destroy binding API.
    pub fn pnf_indicators_destroy(indicators: *mut PnfIndicators);

    /// pnf_indicators_configure binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `config` - 
    pub fn pnf_indicators_configure(ind: *mut PnfIndicators, config: *const PnfIndicatorConfig);
    /// pnf_indicators_set_sma_periods binding API.
    pub fn pnf_indicators_set_sma_periods(
        ind: *mut PnfIndicators,
        short_period: c_int,
        medium_period: c_int,
        long_period: c_int,
    );
    /// pnf_indicators_set_bollinger_params binding API.
    pub fn pnf_indicators_set_bollinger_params(
        ind: *mut PnfIndicators,
        period: c_int,
        std_devs: f64,
    );
    /// pnf_indicators_set_rsi_params binding API.
    pub fn pnf_indicators_set_rsi_params(
        ind: *mut PnfIndicators,
        period: c_int,
        overbought: f64,
        oversold: f64,
    );
    /// pnf_indicators_set_bullish_percent_thresholds binding API.
    pub fn pnf_indicators_set_bullish_percent_thresholds(
        ind: *mut PnfIndicators,
        bullish: f64,
        bearish: f64,
    );
    /// pnf_indicators_set_support_resistance_threshold binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `threshold` - 
    pub fn pnf_indicators_set_support_resistance_threshold(ind: *mut PnfIndicators, threshold: f64);
    /// pnf_indicators_set_congestion_params binding API.
    pub fn pnf_indicators_set_congestion_params(
        ind: *mut PnfIndicators,
        min_columns: c_int,
        price_range: f64,
    );

    /// pnf_indicators_calculate binding API.
    ///
    /// # Arguments
    /// * `indicators` - 
    /// * `chart` - 
    pub fn pnf_indicators_calculate(indicators: *mut PnfIndicators, chart: *const PnfChart);

    /// pnf_indicators_sma_short binding API.
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_short(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_sma_medium binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_medium(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_sma_long binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_long(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_sma_short_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_short_values(ind: *const PnfIndicators) -> PnfDoubleArray;
    /// pnf_indicators_sma_medium_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_medium_values(ind: *const PnfIndicators) -> PnfDoubleArray;
    /// pnf_indicators_sma_long_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sma_long_values(ind: *const PnfIndicators) -> PnfDoubleArray;

    /// pnf_indicators_bollinger_middle binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_middle(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_bollinger_upper binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_upper(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_bollinger_lower binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_lower(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_bollinger_middle_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_middle_values(ind: *const PnfIndicators) -> PnfDoubleArray;
    /// pnf_indicators_bollinger_upper_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_upper_values(ind: *const PnfIndicators) -> PnfDoubleArray;
    /// pnf_indicators_bollinger_lower_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bollinger_lower_values(ind: *const PnfIndicators) -> PnfDoubleArray;

    /// pnf_indicators_rsi binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_rsi(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_rsi_is_overbought binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_rsi_is_overbought(ind: *const PnfIndicators, column: c_int) -> bool;
    /// pnf_indicators_rsi_is_oversold binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_rsi_is_oversold(ind: *const PnfIndicators, column: c_int) -> bool;
    /// pnf_indicators_rsi_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_rsi_values(ind: *const PnfIndicators) -> PnfDoubleArray;

    /// pnf_indicators_obv binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_obv(ind: *const PnfIndicators, column: c_int) -> f64;
    /// pnf_indicators_obv_values binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_obv_values(ind: *const PnfIndicators) -> PnfDoubleArray;

    /// pnf_indicators_bullish_percent binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bullish_percent(ind: *const PnfIndicators) -> f64;
    /// pnf_indicators_is_bullish_alert binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_is_bullish_alert(ind: *const PnfIndicators) -> bool;
    /// pnf_indicators_is_bearish_alert binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_is_bearish_alert(ind: *const PnfIndicators) -> bool;

    /// pnf_indicators_current_signal binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_current_signal(ind: *const PnfIndicators) -> PnfSignalType;
    /// pnf_indicators_signal_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_signal_count(ind: *const PnfIndicators) -> usize;
    #[allow(dead_code)]
    /// pnf_indicators_signal_at binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_signal_at(ind: *const PnfIndicators, index: usize) -> PnfSignal;
    /// pnf_indicators_signals binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_signals(ind: *const PnfIndicators) -> PnfSignalArray;
    /// pnf_indicators_buy_signal_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_buy_signal_count(ind: *const PnfIndicators) -> c_int;
    /// pnf_indicators_sell_signal_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_sell_signal_count(ind: *const PnfIndicators) -> c_int;

    /// pnf_indicators_pattern_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_pattern_count(ind: *const PnfIndicators) -> usize;
    /// pnf_indicators_bullish_pattern_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bullish_pattern_count(ind: *const PnfIndicators) -> usize;
    /// pnf_indicators_bearish_pattern_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bearish_pattern_count(ind: *const PnfIndicators) -> usize;
    #[allow(dead_code)]
    /// pnf_indicators_pattern_at binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `index` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_pattern_at(ind: *const PnfIndicators, index: usize) -> PnfPattern;
    /// pnf_indicators_patterns binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_patterns(ind: *const PnfIndicators) -> PnfPatternArray;
    /// pnf_indicators_bullish_patterns binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bullish_patterns(ind: *const PnfIndicators) -> PnfPatternArray;
    /// pnf_indicators_bearish_patterns binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_bearish_patterns(ind: *const PnfIndicators) -> PnfPatternArray;

    /// pnf_indicators_support_level_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_support_level_count(ind: *const PnfIndicators) -> usize;
    /// pnf_indicators_resistance_level_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_resistance_level_count(ind: *const PnfIndicators) -> usize;
    /// pnf_indicators_is_near_support binding API.
    pub fn pnf_indicators_is_near_support(
        ind: *const PnfIndicators,
        price: f64,
        tolerance: f64,
    ) -> bool;
    /// pnf_indicators_is_near_resistance binding API.
    pub fn pnf_indicators_is_near_resistance(
        ind: *const PnfIndicators,
        price: f64,
        tolerance: f64,
    ) -> bool;
    /// pnf_indicators_support_levels binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_support_levels(ind: *const PnfIndicators) -> PnfLevelArray;
    /// pnf_indicators_resistance_levels binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_resistance_levels(ind: *const PnfIndicators) -> PnfLevelArray;
    /// pnf_indicators_support_prices binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_support_prices(ind: *const PnfIndicators) -> PnfDoubleArray;
    /// pnf_indicators_resistance_prices binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_resistance_prices(ind: *const PnfIndicators) -> PnfDoubleArray;

    /// pnf_indicators_is_in_congestion binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    /// * `column` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_is_in_congestion(ind: *const PnfIndicators, column: c_int) -> bool;
    /// pnf_indicators_congestion_zone_count binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_congestion_zone_count(ind: *const PnfIndicators) -> usize;

    /// pnf_indicators_summary binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_summary(ind: *const PnfIndicators) -> *const c_char;
    /// pnf_indicators_to_string binding API.
    ///
    /// # Arguments
    /// * `ind` - 
    ///
    /// # Returns
    /// 
    pub fn pnf_indicators_to_string(ind: *const PnfIndicators) -> *const c_char;

    /// pnf_free_string binding API.
    ///
    /// # Arguments
    /// * `s` - 
    pub fn pnf_free_string(s: *const c_char);
    /// pnf_free_double_array binding API.
    pub fn pnf_free_double_array(arr: PnfDoubleArray);
    /// pnf_free_signal_array binding API.
    pub fn pnf_free_signal_array(arr: PnfSignalArray);
    /// pnf_free_pattern_array binding API.
    ///
    /// # Arguments
    /// * `arr` - 
    pub fn pnf_free_pattern_array(arr: PnfPatternArray);
    /// pnf_free_level_array binding API.
    pub fn pnf_free_level_array(arr: PnfLevelArray);
}
