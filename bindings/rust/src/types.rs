//! Public data types shared by the safe Rust API.
use crate::ffi;
use std::ffi::CStr;
use std::os::raw::c_int;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Marker used for each filled box in a column.
pub enum BoxType {
    X,
    O,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Direction of a Point and Figure column.
pub enum ColumnType {
    X,
    O,
    Mixed,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Input method used when constructing columns from price data.
pub enum ConstructionMethod {
    Close,
    HighLow,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Strategy used to determine chart box size.
pub enum BoxSizeMethod {
    Fixed,
    Traditional,
    Percentage,
    Points,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Direction of a generated signal.
pub enum SignalType {
    None,
    Buy,
    Sell,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// Supported Point and Figure pattern classifications.
pub enum PatternType {
    None,
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

#[derive(Debug, Clone, Copy)]
/// Configuration used when constructing a [`Chart`](crate::Chart).
pub struct ChartConfig {
    pub method: ConstructionMethod,
    pub box_size_method: BoxSizeMethod,
    pub box_size: f64,
    pub reversal: i32,
}

impl Default for ChartConfig {
    fn default() -> Self {
        let config = unsafe { ffi::pnf_chart_config_default() };
        Self::from_ffi(config)
    }
}

impl ChartConfig {
    /// Creates a default configuration.
    pub fn new() -> Self {
        Self::default()
    }

    pub(crate) fn to_ffi(&self) -> ffi::PnfChartConfig {
        ffi::PnfChartConfig {
            method: self.method.to_ffi(),
            box_size_method: self.box_size_method.to_ffi(),
            box_size: self.box_size,
            reversal: self.reversal as c_int,
        }
    }

    pub(crate) fn from_ffi(config: ffi::PnfChartConfig) -> Self {
        Self {
            method: ConstructionMethod::from_ffi(config.method),
            box_size_method: BoxSizeMethod::from_ffi(config.box_size_method),
            box_size: config.box_size,
            reversal: config.reversal,
        }
    }
}

#[derive(Debug, Clone, Copy)]
/// Configuration used when constructing [`Indicators`](crate::Indicators).
pub struct IndicatorConfig {
    pub sma_short_period: i32,
    pub sma_medium_period: i32,
    pub sma_long_period: i32,
    pub bollinger_period: i32,
    pub bollinger_std_devs: f64,
    pub rsi_period: i32,
    pub rsi_overbought: f64,
    pub rsi_oversold: f64,
    pub bullish_alert_threshold: f64,
    pub bearish_alert_threshold: f64,
    pub support_resistance_threshold: f64,
    pub congestion_min_columns: i32,
    pub congestion_price_range: f64,
}

impl Default for IndicatorConfig {
    fn default() -> Self {
        let config = unsafe { ffi::pnf_indicator_config_default() };
        Self::from_ffi(config)
    }
}

impl IndicatorConfig {
    /// Creates a default configuration.
    pub fn new() -> Self {
        Self::default()
    }

    pub(crate) fn to_ffi(&self) -> ffi::PnfIndicatorConfig {
        ffi::PnfIndicatorConfig {
            sma_short_period: self.sma_short_period as c_int,
            sma_medium_period: self.sma_medium_period as c_int,
            sma_long_period: self.sma_long_period as c_int,
            bollinger_period: self.bollinger_period as c_int,
            bollinger_std_devs: self.bollinger_std_devs,
            rsi_period: self.rsi_period as c_int,
            rsi_overbought: self.rsi_overbought,
            rsi_oversold: self.rsi_oversold,
            bullish_alert_threshold: self.bullish_alert_threshold,
            bearish_alert_threshold: self.bearish_alert_threshold,
            support_resistance_threshold: self.support_resistance_threshold,
            congestion_min_columns: self.congestion_min_columns as c_int,
            congestion_price_range: self.congestion_price_range,
        }
    }

    pub(crate) fn from_ffi(config: ffi::PnfIndicatorConfig) -> Self {
        Self {
            sma_short_period: config.sma_short_period,
            sma_medium_period: config.sma_medium_period,
            sma_long_period: config.sma_long_period,
            bollinger_period: config.bollinger_period,
            bollinger_std_devs: config.bollinger_std_devs,
            rsi_period: config.rsi_period,
            rsi_overbought: config.rsi_overbought,
            rsi_oversold: config.rsi_oversold,
            bullish_alert_threshold: config.bullish_alert_threshold,
            bearish_alert_threshold: config.bearish_alert_threshold,
            support_resistance_threshold: config.support_resistance_threshold,
            congestion_min_columns: config.congestion_min_columns,
            congestion_price_range: config.congestion_price_range,
        }
    }
}

#[derive(Debug, Clone, Copy)]
/// OHLC candle input used by `Chart::add_ohlc`.
pub struct OHLC {
    pub timestamp: i64,
    pub open: f64,
    pub high: f64,
    pub low: f64,
    pub close: f64,
    pub volume: f64,
}

impl OHLC {
    pub(crate) fn to_ffi(&self) -> ffi::PnfOHLC {
        ffi::PnfOHLC {
            timestamp: self.timestamp,
            open: self.open,
            high: self.high,
            low: self.low,
            close: self.close,
            volume: self.volume,
        }
    }
}

#[derive(Debug, Clone, Copy)]
/// Signal event emitted by indicator analysis.
pub struct Signal {
    pub signal_type: SignalType,
    pub column_index: i32,
    pub price: f64,
}

impl Signal {
    pub(crate) fn from_ffi(signal: ffi::PnfSignal) -> Self {
        Self {
            signal_type: SignalType::from_ffi(signal.signal_type),
            column_index: signal.column_index,
            price: signal.price,
        }
    }
}

#[derive(Debug, Clone, Copy)]
/// Pattern event detected by indicator analysis.
pub struct Pattern {
    pub pattern_type: PatternType,
    pub start_column: i32,
    pub end_column: i32,
    pub price: f64,
    pub is_bullish: bool,
}

impl Pattern {
    pub(crate) fn from_ffi(pattern: ffi::PnfPattern) -> Self {
        Self {
            pattern_type: PatternType::from_ffi(pattern.pattern_type),
            start_column: pattern.start_column,
            end_column: pattern.end_column,
            price: pattern.price,
            is_bullish: pattern.is_bullish,
        }
    }
}

#[derive(Debug, Clone, Copy)]
/// Support or resistance level with touch statistics.
pub struct SupportResistanceLevel {
    pub price: f64,
    pub touch_count: i32,
    pub is_support: bool,
}

impl SupportResistanceLevel {
    pub(crate) fn from_ffi(level: ffi::PnfSupportResistanceLevel) -> Self {
        Self {
            price: level.price,
            touch_count: level.touch_count,
            is_support: level.is_support,
        }
    }
}

/// Column data for export.
#[derive(Debug, Clone, Copy)]
/// Snapshot of a chart column returned by native APIs.
pub struct ColumnData {
    pub index: i32,
    pub column_type: ColumnType,
    pub high: f64,
    pub low: f64,
    pub box_count: i32,
}

impl ColumnData {
    #[allow(dead_code)]
    pub(crate) fn from_ffi(data: ffi::PnfColumnData) -> Self {
        Self {
            index: data.index,
            column_type: ColumnType::from_ffi(data.column_type),
            high: data.high,
            low: data.low,
            box_count: data.box_count,
        }
    }
}

impl BoxType {
    #[allow(dead_code)]
    pub(crate) fn to_ffi(self) -> ffi::PnfBoxType {
        match self {
            BoxType::X => ffi::PnfBoxType::X,
            BoxType::O => ffi::PnfBoxType::O,
        }
    }

    pub(crate) fn from_ffi(bt: ffi::PnfBoxType) -> Self {
        match bt {
            ffi::PnfBoxType::X => BoxType::X,
            ffi::PnfBoxType::O => BoxType::O,
        }
    }
}

impl ColumnType {
    #[allow(dead_code)]
    pub(crate) fn to_ffi(self) -> ffi::PnfColumnType {
        match self {
            ColumnType::X => ffi::PnfColumnType::X,
            ColumnType::O => ffi::PnfColumnType::O,
            ColumnType::Mixed => ffi::PnfColumnType::Mixed,
        }
    }

    pub(crate) fn from_ffi(ct: ffi::PnfColumnType) -> Self {
        match ct {
            ffi::PnfColumnType::X => ColumnType::X,
            ffi::PnfColumnType::O => ColumnType::O,
            ffi::PnfColumnType::Mixed => ColumnType::Mixed,
        }
    }
}

impl ConstructionMethod {
    pub(crate) fn to_ffi(self) -> ffi::PnfConstructionMethod {
        match self {
            ConstructionMethod::Close => ffi::PnfConstructionMethod::Close,
            ConstructionMethod::HighLow => ffi::PnfConstructionMethod::HighLow,
        }
    }

    pub(crate) fn from_ffi(cm: ffi::PnfConstructionMethod) -> Self {
        match cm {
            ffi::PnfConstructionMethod::Close => ConstructionMethod::Close,
            ffi::PnfConstructionMethod::HighLow => ConstructionMethod::HighLow,
        }
    }
}

impl BoxSizeMethod {
    pub(crate) fn to_ffi(self) -> ffi::PnfBoxSizeMethod {
        match self {
            BoxSizeMethod::Fixed => ffi::PnfBoxSizeMethod::Fixed,
            BoxSizeMethod::Traditional => ffi::PnfBoxSizeMethod::Traditional,
            BoxSizeMethod::Percentage => ffi::PnfBoxSizeMethod::Percentage,
            BoxSizeMethod::Points => ffi::PnfBoxSizeMethod::Points,
        }
    }

    pub(crate) fn from_ffi(bsm: ffi::PnfBoxSizeMethod) -> Self {
        match bsm {
            ffi::PnfBoxSizeMethod::Fixed => BoxSizeMethod::Fixed,
            ffi::PnfBoxSizeMethod::Traditional => BoxSizeMethod::Traditional,
            ffi::PnfBoxSizeMethod::Percentage => BoxSizeMethod::Percentage,
            ffi::PnfBoxSizeMethod::Points => BoxSizeMethod::Points,
        }
    }
}

impl SignalType {
    #[allow(dead_code)]
    pub(crate) fn to_ffi(self) -> ffi::PnfSignalType {
        match self {
            SignalType::None => ffi::PnfSignalType::None,
            SignalType::Buy => ffi::PnfSignalType::Buy,
            SignalType::Sell => ffi::PnfSignalType::Sell,
        }
    }

    pub(crate) fn from_ffi(st: ffi::PnfSignalType) -> Self {
        match st {
            ffi::PnfSignalType::None => SignalType::None,
            ffi::PnfSignalType::Buy => SignalType::Buy,
            ffi::PnfSignalType::Sell => SignalType::Sell,
        }
    }
}

impl PatternType {
    pub(crate) fn from_ffi(pt: ffi::PnfPatternType) -> Self {
        match pt {
            ffi::PnfPatternType::None => PatternType::None,
            ffi::PnfPatternType::DoubleTopBreakout => PatternType::DoubleTopBreakout,
            ffi::PnfPatternType::DoubleBottomBreakdown => PatternType::DoubleBottomBreakdown,
            ffi::PnfPatternType::TripleTopBreakout => PatternType::TripleTopBreakout,
            ffi::PnfPatternType::TripleBottomBreakdown => PatternType::TripleBottomBreakdown,
            ffi::PnfPatternType::QuadrupleTopBreakout => PatternType::QuadrupleTopBreakout,
            ffi::PnfPatternType::QuadrupleBottomBreakdown => PatternType::QuadrupleBottomBreakdown,
            ffi::PnfPatternType::AscendingTripleTop => PatternType::AscendingTripleTop,
            ffi::PnfPatternType::DescendingTripleBottom => PatternType::DescendingTripleBottom,
            ffi::PnfPatternType::BullishCatapult => PatternType::BullishCatapult,
            ffi::PnfPatternType::BearishCatapult => PatternType::BearishCatapult,
            ffi::PnfPatternType::BullishSignalReversed => PatternType::BullishSignalReversed,
            ffi::PnfPatternType::BearishSignalReversed => PatternType::BearishSignalReversed,
            ffi::PnfPatternType::BullishTriangle => PatternType::BullishTriangle,
            ffi::PnfPatternType::BearishTriangle => PatternType::BearishTriangle,
            ffi::PnfPatternType::LongTailDown => PatternType::LongTailDown,
            ffi::PnfPatternType::HighPole => PatternType::HighPole,
            ffi::PnfPatternType::LowPole => PatternType::LowPole,
            ffi::PnfPatternType::BullTrap => PatternType::BullTrap,
            ffi::PnfPatternType::BearTrap => PatternType::BearTrap,
            ffi::PnfPatternType::SpreadTripleTop => PatternType::SpreadTripleTop,
            ffi::PnfPatternType::SpreadTripleBottom => PatternType::SpreadTripleBottom,
        }
    }
}

pub(crate) fn cstr_to_string(ptr: *const std::os::raw::c_char) -> Option<String> {
    if ptr.is_null() {
        None
    } else {
        unsafe { CStr::from_ptr(ptr).to_str().ok().map(|s| s.to_owned()) }
    }
}
