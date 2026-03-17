# pnf

[![crates.io](https://img.shields.io/crates/v/pnf.svg)](https://crates.io/crates/pnf)
[![docs.rs](https://docs.rs/pnf/badge.svg)](https://docs.rs/pnf)
[![License](https://img.shields.io/crates/l/pnf.svg)](https://github.com/gregorian-09/pnf-chart-system/blob/master/LICENSE)

Rust bindings for the Point-and-Figure (PnF) analysis engine.

The crate is designed for production chart analytics, not just drawing columns.

## Features

- PnF chart construction with configurable method, box-size mode, and reversal.
- Structural trend checks (bullish support / bearish resistance context).
- Indicator stack: SMA, Bollinger Bands, RSI, OBV, Bullish Percent.
- Signal and pattern extraction (buy/sell and named PnF patterns).
- Support/resistance and congestion detection.
- Real-time localhost dashboard snapshot publishing.

## Installation

```toml
[dependencies]
pnf = "0.1.0"
```

## Quick Start

```rust,no_run
use pnf::{BoxSizeMethod, Chart, ChartConfig, ConstructionMethod, IndicatorConfig, Indicators};

fn main() {
    let config = ChartConfig {
        method: ConstructionMethod::HighLow,
        box_size_method: BoxSizeMethod::Traditional,
        box_size: 0.0,
        reversal: 3,
    };

    let mut chart = Chart::with_config(config);
    chart.add_data(5000.0, 4950.0, 4985.0, 1_700_000_000);
    chart.add_data(5040.0, 4980.0, 5030.0, 1_700_000_360);
    chart.add_data(5065.0, 5010.0, 5055.0, 1_700_000_720);

    let mut indicators = Indicators::with_config(IndicatorConfig::default());
    indicators.calculate(&chart);

    println!("{}", chart.to_ascii().unwrap_or_default());
    println!("{}", indicators.summary().unwrap_or_default());
}
```

## Trendline and Bias Checks

```rust,ignore
let last_price = 5055.0;

println!("bullish_bias={}", chart.has_bullish_bias());
println!("bearish_bias={}", chart.has_bearish_bias());
println!(
    "above_bullish_support={}",
    chart.is_above_bullish_support(last_price)
);
println!(
    "below_bearish_resistance={}",
    chart.is_below_bearish_resistance(last_price)
);
```

These are the standard structural checks before acting on breakouts/breakdowns.

## Indicators and Momentum

```rust,ignore
let last_col = chart.column_count().saturating_sub(1) as i32;

println!("sma_short={}", indicators.sma_short(last_col));
println!("bollinger_upper={}", indicators.bollinger_upper(last_col));
println!("rsi={}", indicators.rsi(last_col));
println!("obv={}", indicators.obv(last_col));
println!("bullish_percent={}", indicators.bullish_percent());
```

## Signals and Pattern Analysis

```rust,ignore
println!("current_signal={:?}", indicators.current_signal());
println!("signal_count={}", indicators.signal_count());
println!("buy_signal_count={}", indicators.buy_signal_count());
println!("sell_signal_count={}", indicators.sell_signal_count());

println!("pattern_count={}", indicators.pattern_count());
println!("bullish_pattern_count={}", indicators.bullish_pattern_count());
println!("bearish_pattern_count={}", indicators.bearish_pattern_count());

for pattern in indicators.patterns() {
    println!("pattern={:?} start={} end={} price={}", pattern.pattern_type, pattern.start_column, pattern.end_column, pattern.price);
}
```

## Support, Resistance, and Congestion

```rust,ignore
println!("support_levels={}", indicators.support_level_count());
println!("resistance_levels={}", indicators.resistance_level_count());
println!("near_support={}", indicators.is_near_support(5050.0, 1.0));
println!("near_resistance={}", indicators.is_near_resistance(5050.0, 1.0));
println!("congestion_zones={}", indicators.congestion_zone_count());
```

## Real-Time Dashboard

```rust,ignore
use pnf::DashboardServer;

let mut server = DashboardServer::new();
let _url = server.start("127.0.0.1", 8763).expect("dashboard start failed");

// Publish with indicators
server.publish(&chart, Some(&indicators));

// Or publish chart-only snapshots
server.publish(&chart, None);

println!("{}", server.url());
```

## API Surface

Core:
- `Chart`, `ChartConfig`

Indicators and analysis:
- `Indicators`, `IndicatorConfig`
- SMA/Bollinger/RSI/OBV accessors and vectors
- signal/pattern/support-resistance/congestion queries

Data types:
- `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`
- `BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `SignalType`, `PatternType`

Dashboard:
- `DashboardServer`, `build_snapshot_json`

Version API:
- `version()`, `version_major()`, `version_minor()`, `version_patch()`

## Versioning

- Crate version follows the core engine release.
- Use matching versions across Python/Java/C# bindings when deploying mixed-language systems.
- All notable changes are listed in `CHANGELOG.md`.

## Troubleshooting

- Linker/runtime issues: rebuild native artifacts and verify toolchain compatibility.
- Empty indicator vectors: ensure enough columns exist for configured periods.
- Behavior drift between services: align package versions and chart config defaults.

## Documentation and Links

- API docs: https://docs.rs/pnf
- Rust binding reference: `docs/bindings/rust.md`
- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
