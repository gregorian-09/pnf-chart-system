# pnf (Rust)

`pnf` is the official Rust binding for the PnF (Point and Figure) chart engine.

It exposes:
- Chart construction with P&F box/reversal rules.
- Indicator calculations (SMA, Bollinger, RSI, OBV, signals, patterns, support/resistance, congestion).
- JSON/ASCII exports.
- A real-time localhost dashboard server.

## Installation

```toml
[dependencies]
pnf = "0.1.0"
```

## Quick Start

```rust
use pnf::{Chart, Indicators};

fn main() {
    let mut chart = Chart::new();
    chart.add_price(5000.0, 1_700_000_000);
    chart.add_price(5030.0, 1_700_000_001);

    let mut indicators = Indicators::new();
    indicators.calculate(&chart);

    println!("{}", chart.to_ascii());
    println!("{}", indicators.summary());
}
```

## Real-Time Dashboard

```rust
use pnf::DashboardServer;

let mut server = DashboardServer::new();
server.start("127.0.0.1", 8763).unwrap();
server.publish(&chart, &indicators).unwrap();
println!("{}", server.url());
```

## API Highlights

- Core:
  - `Chart`
  - `ChartConfig`
- Indicators:
  - `Indicators`
  - `IndicatorConfig`
- Data types:
  - `OHLC`, `Signal`, `Pattern`, `SupportResistanceLevel`
  - enums (`BoxType`, `ColumnType`, `ConstructionMethod`, `BoxSizeMethod`, `SignalType`, `PatternType`)
- Dashboard:
  - `DashboardServer`
  - `build_snapshot_json`
- Version:
  - `version()`, `version_major()`, `version_minor()`, `version_patch()`

## Documentation

- Crate docs: https://docs.rs/pnf
- Project docs: https://github.com/gregorian-09/pnf-chart-system/tree/master/docs
- Rust binding reference: https://github.com/gregorian-09/pnf-chart-system/blob/master/docs/bindings/rust.md

## Compatibility

- Rust edition: 2021
- Runtime: stable Rust toolchain
- Native backend: bundled C ABI bridge from the same project version

## Troubleshooting

- If docs.rs build fails, verify optional dependencies/features are docs-safe.
- Keep `pnf` crate version aligned with the same released core version.
- If linking errors occur in local workspace development, rebuild the core project first.

## Links

- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
