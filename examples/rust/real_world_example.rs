use std::fs::File;
use std::io::{BufRead, BufReader, Write};
use std::path::Path;

extern crate pnf;
use pnf::*;

#[derive(Debug)]
struct OHLCBar {
    timestamp: String,
    open: f64,
    high: f64,
    low: f64,
    close: f64,
}

fn load_csv(filename: &str) -> Result<Vec<OHLCBar>, Box<dyn std::error::Error>> {
    let file = File::open(filename)?;
    let reader = BufReader::new(file);
    let mut data = Vec::new();

    for (i, line) in reader.lines().enumerate() {
        if i == 0 {
            continue;
        }

        let line = line?;
        let fields: Vec<&str> = line.split(',').collect();

        if fields.len() >= 6 {
            let bar = OHLCBar {
                timestamp: fields[0].to_string(),
                open: fields[2].parse()?,
                high: fields[3].parse()?,
                low: fields[4].parse()?,
                close: fields[5].parse()?,
            };
            data.push(bar);
        }
    }

    println!("Loaded {} bars from {}", data.len(), filename);
    Ok(data)
}

fn process_dataset(csv_file: &str, output_prefix: &str) -> Result<(), Box<dyn std::error::Error>> {
    println!("\n{}", "=".repeat(60));
    println!("Processing: {}", csv_file);
    println!("{}", "=".repeat(60));

    let data = load_csv(csv_file)?;
    if data.is_empty() {
        eprintln!("No data loaded, skipping...");
        return Ok(());
    }

    let (box_size_method, box_size) = if csv_file.contains("GBPUSD") || csv_file.contains("EURUSD") {
        (BoxSizeMethod::Points, 0.0020)
    } else {
        (BoxSizeMethod::Traditional, 0.0)
    };

    let config = ChartConfig {
        method: ConstructionMethod::HighLow,
        box_size_method,
        box_size,
        reversal: 3,
    };

    let mut chart = Chart::with_config(config);

    let mut timestamp: i64 = 1640000000;
    for bar in &data {
        chart.add_data(bar.high, bar.low, bar.close, timestamp);
        timestamp += 1;
    }

    println!("\nChart Statistics:");
    println!("  Total columns: {}", chart.column_count());
    println!("  X columns: {}", chart.x_column_count());
    println!("  O columns: {}", chart.o_column_count());
    println!("  Box size: {}", chart.box_size());
    println!("  Has bullish bias: {}", if chart.has_bullish_bias() { "Yes" } else { "No" });
    println!("  Has bearish bias: {}", if chart.has_bearish_bias() { "Yes" } else { "No" });

    let ind_config = IndicatorConfig {
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

    let mut indicators = Indicators::with_config(ind_config);
    indicators.calculate(&chart);

    println!("\nIndicator Results:");
    println!("  Bullish Percent: {:.2}%", indicators.bullish_percent());
    println!("  Signals: {}", indicators.signal_count());
    println!("  Patterns: {}", indicators.pattern_count());
    println!("  Buy signals: {}", indicators.buy_signal_count());
    println!("  Sell signals: {}", indicators.sell_signal_count());
    println!("  Support levels: {}", indicators.support_level_count());
    println!("  Resistance levels: {}", indicators.resistance_level_count());

    let signals = indicators.signals();
    println!("\nRecent Signals:");
    for signal in signals.iter().take(5) {
        let signal_type = match signal.signal_type {
            SignalType::Buy => "BUY",
            SignalType::Sell => "SELL",
            SignalType::None => "NONE",
        };
        println!("  Column {}: {} (Price: {:.5})", signal.column_index, signal_type, signal.price);
    }

    let patterns = indicators.patterns();
    println!("\nRecent Patterns:");
    for pattern in patterns.iter().take(5) {
        let sentiment = if pattern.is_bullish { "BULLISH" } else { "BEARISH" };
        println!("  Columns {}-{}: {:?} ({}, Price: {:.5})",
                 pattern.start_column, pattern.end_column,
                 pattern.pattern_type, sentiment, pattern.price);
    }

    println!("\nGenerating visualizations...");

    let output_dir = Path::new(output_prefix).parent().unwrap();
    std::fs::create_dir_all(output_dir)?;

    if let Some(ascii_str) = chart.to_ascii() {
        let mut file = File::create(format!("{}_ascii.txt", output_prefix))?;
        file.write_all(ascii_str.as_bytes())?;
        println!("  ASCII chart saved to: {}_ascii.txt", output_prefix);
    }

    if let Some(json_str) = chart.to_json() {
        let mut file = File::create(format!("{}_chart.json", output_prefix))?;
        file.write_all(json_str.as_bytes())?;
        println!("  JSON chart saved to: {}_chart.json", output_prefix);
    }

    println!("\nProcessing complete!");

    Ok(())
}

fn main() {
    println!("PnF Chart Library - Real World Data Example (Rust)");
    println!("Library Version: {}", pnf::version());

    if let Err(e) = process_dataset("../../fixtures/GBPUSD_PERIOD_M1.csv", "output/gbpusd_m1") {
        eprintln!("Error processing GBPUSD: {}", e);
    }

    if let Err(e) = process_dataset("../../fixtures/Boom_500_Index_PERIOD_H1.csv", "output/boom500_h1") {
        eprintln!("Error processing Boom 500: {}", e);
    }

    if let Err(e) = process_dataset("../../fixtures/Volatility_75_Index_PERIOD_D1.csv", "output/volatility75_d1") {
        eprintln!("Error processing Volatility 75: {}", e);
    }

    println!("\n{}", "=".repeat(60));
    println!("All datasets processed successfully!");
    println!("{}", "=".repeat(60));
}
