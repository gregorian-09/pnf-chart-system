//! Safe Rust bindings for the Point and Figure (PnF) chart library.
mod ffi;
mod types;
mod chart;
mod indicators;
mod dashboard;

pub use chart::Chart;
pub use dashboard::{build_snapshot_json, DashboardServer};
pub use indicators::Indicators;
pub use types::*;

use std::ffi::CStr;

/// Returns the library version string.
pub fn version() -> String {
    unsafe {
        let ptr = ffi::pnf_version_string();
        let result = CStr::from_ptr(ptr)
            .to_str()
            .unwrap_or("unknown")
            .to_owned();
        if !ptr.is_null() {
            ffi::pnf_free_string(ptr);
        }
        result
    }
}

/// Returns the library major version.
pub fn version_major() -> i32 {
    unsafe { ffi::pnf_version_major() }
}

/// Returns the library minor version.
pub fn version_minor() -> i32 {
    unsafe { ffi::pnf_version_minor() }
}

/// Returns the library patch version.
pub fn version_patch() -> i32 {
    unsafe { ffi::pnf_version_patch() }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_version() {
        let ver = version();
        assert!(!ver.is_empty());
        println!("PnF Library Version: {}", ver);
    }

    #[test]
    fn test_chart_creation() {
        let chart = Chart::new();
        assert_eq!(chart.column_count(), 0);
    }

    #[test]
    fn test_chart_with_config() {
        let config = ChartConfig {
            method: ConstructionMethod::Close,
            box_size_method: BoxSizeMethod::Traditional,
            box_size: 0.0,
            reversal: 3,
        };
        let chart = Chart::with_config(config);
        assert_eq!(chart.column_count(), 0);
    }

    #[test]
    fn test_chart_add_price() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        chart.add_price(100.0, timestamp);
        chart.add_price(105.0, timestamp + 1);
        chart.add_price(110.0, timestamp + 2);

        assert!(chart.column_count() > 0);
    }

    #[test]
    fn test_chart_add_data() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        assert!(chart.add_data(100.0, 99.0, 99.5, timestamp));
        assert!(chart.add_data(101.0, 100.0, 100.5, timestamp + 1));
        assert!(chart.add_data(102.0, 101.0, 101.5, timestamp + 2));

        assert!(chart.column_count() > 0);
    }

    #[test]
    fn test_chart_properties() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        chart.add_price(100.0, timestamp);
        chart.add_price(102.0, timestamp + 1);
        chart.add_price(101.0, timestamp + 2);

        assert!(chart.box_size() > 0.0);
        assert_eq!(chart.column_count(), chart.x_column_count() + chart.o_column_count());
    }

    #[test]
    fn test_chart_clear() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        chart.add_price(100.0, timestamp);
        chart.add_price(102.0, timestamp + 1);
        assert!(chart.column_count() > 0);

        chart.clear();
        assert_eq!(chart.column_count(), 0);
    }

    #[test]
    fn test_indicators_creation() {
        let indicators = Indicators::new();
        assert_eq!(indicators.pattern_count(), 0);
    }

    #[test]
    fn test_indicators_with_config() {
        let config = IndicatorConfig {
            sma_short_period: 5,
            sma_medium_period: 10,
            sma_long_period: 20,
            bollinger_period: 20,
            bollinger_std_devs: 2.0,
            rsi_period: 14,
            rsi_overbought: 70.0,
            rsi_oversold: 30.0,
            bullish_alert_threshold: 70.0,
            bearish_alert_threshold: 30.0,
            support_resistance_threshold: 0.01,
            congestion_min_columns: 4,
            congestion_price_range: 0.05,
        };
        let indicators = Indicators::with_config(config);
        assert_eq!(indicators.signal_count(), 0);
    }

    #[test]
    fn test_indicators_calculate() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..20 {
            chart.add_price(100.0 + i as f64, timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let bp = indicators.bullish_percent();
        assert!(bp >= 0.0 && bp <= 100.0);
    }

    #[test]
    fn test_indicators_sma() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..30 {
            chart.add_price(100.0 + i as f64, timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let sma_short = indicators.sma_short_values();
        let sma_medium = indicators.sma_medium_values();
        let sma_long = indicators.sma_long_values();

        assert!(!sma_short.is_empty());
        assert!(!sma_medium.is_empty());
        assert!(!sma_long.is_empty());
    }

    #[test]
    fn test_indicators_bollinger() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..30 {
            chart.add_price(100.0 + (i as f64 * 0.5), timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let middle = indicators.bollinger_middle_values();
        let upper = indicators.bollinger_upper_values();
        let lower = indicators.bollinger_lower_values();

        assert!(!middle.is_empty());
        assert!(!upper.is_empty());
        assert!(!lower.is_empty());
    }

    #[test]
    fn test_indicators_signals() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..50 {
            let price = 100.0 + ((i as f64 * 0.5).sin() * 10.0);
            chart.add_price(price, timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let signals = indicators.signals();
        assert!(indicators.signal_count() == signals.len());
    }

    #[test]
    fn test_indicators_patterns() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..100 {
            let price = 100.0 + ((i as f64 * 0.3).sin() * 15.0);
            chart.add_price(price, timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let patterns = indicators.patterns();
        assert_eq!(indicators.pattern_count(), patterns.len());

        let bullish = indicators.bullish_patterns();
        let bearish = indicators.bearish_patterns();
        assert_eq!(
            indicators.bullish_pattern_count() + indicators.bearish_pattern_count(),
            bullish.len() + bearish.len()
        );
    }

    #[test]
    fn test_support_resistance() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..100 {
            let price = 100.0 + ((i as f64 * 0.2).sin() * 10.0);
            chart.add_price(price, timestamp + i);
        }

        let mut indicators = Indicators::new();
        indicators.calculate(&chart);

        let support_levels = indicators.support_levels();
        let resistance_levels = indicators.resistance_levels();

        assert_eq!(indicators.support_level_count(), support_levels.len());
        assert_eq!(
            indicators.resistance_level_count(),
            resistance_levels.len()
        );

        let support_prices = indicators.support_prices();
        let resistance_prices = indicators.resistance_prices();

        assert_eq!(support_levels.len(), support_prices.len());
        assert_eq!(resistance_levels.len(), resistance_prices.len());
    }

    #[test]
    fn test_chart_bias() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        for i in 0..20 {
            chart.add_price(100.0 + i as f64, timestamp + i);
        }

        let has_bullish = chart.has_bullish_bias();
        let has_bearish = chart.has_bearish_bias();

        println!("Bullish bias: {}, Bearish bias: {}", has_bullish, has_bearish);
    }

    #[test]
    fn test_column_info() {
        let mut chart = Chart::new();
        let timestamp = 1640000000;

        chart.add_price(100.0, timestamp);
        chart.add_price(105.0, timestamp + 1);
        chart.add_price(103.0, timestamp + 2);

        if chart.column_count() > 0 {
            let col_type = chart.column_type(0);
            let box_count = chart.column_box_count(0);
            let highest = chart.column_highest(0);
            let lowest = chart.column_lowest(0);

            assert!(matches!(
                col_type,
                ColumnType::X | ColumnType::O | ColumnType::Mixed
            ));
            assert!(box_count > 0);
            assert!(highest >= lowest);
        }
    }
}
