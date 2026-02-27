use pnf::{Chart, Indicators};

fn main() {
    println!("PnF Library Version: {}", pnf::version());

    let mut chart = Chart::new();

    let timestamp = 1640000000;
    for i in 0..50 {
        let price = 100.0 + ((i as f64 * 0.5).sin() * 10.0);
        chart.add_price(price, timestamp + i);
    }

    println!("\nChart Information:");
    println!("  Total columns: {}", chart.column_count());
    println!("  X columns: {}", chart.x_column_count());
    println!("  O columns: {}", chart.o_column_count());
    println!("  Box size: {:.2}", chart.box_size());
    println!("  Bullish bias: {}", chart.has_bullish_bias());
    println!("  Bearish bias: {}", chart.has_bearish_bias());

    let mut indicators = Indicators::new();
    indicators.calculate(&chart);

    println!("\nIndicators:");
    println!("  Bullish percent: {:.2}%", indicators.bullish_percent());
    println!("  Bullish alert: {}", indicators.is_bullish_alert());
    println!("  Bearish alert: {}", indicators.is_bearish_alert());

    println!("\nSignals:");
    println!("  Total signals: {}", indicators.signal_count());
    println!("  Buy signals: {}", indicators.buy_signal_count());
    println!("  Sell signals: {}", indicators.sell_signal_count());

    let signals = indicators.signals();
    for (i, signal) in signals.iter().take(5).enumerate() {
        println!("  Signal {}: {:?} at column {}, price {:.2}",
                 i + 1, signal.signal_type, signal.column_index, signal.price);
    }

    println!("\nPatterns:");
    println!("  Total patterns: {}", indicators.pattern_count());
    println!("  Bullish patterns: {}", indicators.bullish_pattern_count());
    println!("  Bearish patterns: {}", indicators.bearish_pattern_count());

    let patterns = indicators.patterns();
    for (i, pattern) in patterns.iter().take(5).enumerate() {
        println!("  Pattern {}: {:?} at columns {}-{}, price {:.2}",
                 i + 1, pattern.pattern_type, pattern.start_column,
                 pattern.end_column, pattern.price);
    }

    println!("\nSupport/Resistance:");
    println!("  Support levels: {}", indicators.support_level_count());
    println!("  Resistance levels: {}", indicators.resistance_level_count());

    let support_prices = indicators.support_prices();
    if !support_prices.is_empty() {
        println!("  Support prices: {:?}", &support_prices[..support_prices.len().min(5)]);
    }

    let resistance_prices = indicators.resistance_prices();
    if !resistance_prices.is_empty() {
        println!("  Resistance prices: {:?}", &resistance_prices[..resistance_prices.len().min(5)]);
    }
}
