# PnF Rust Bindings

Safe Rust bindings for the Point and Figure Chart Library.

## Requirements

- Rust 1.56 or higher
- The PnF C library must be built first

## Building

First, build the C library:

```bash
cd ../..
mkdir -p build && cd build
cmake ..
cmake --build . --config Release
```

Then build the Rust bindings:

```bash
cd ../bindings/rust
cargo build --release
```

## Running Tests

```bash
cargo test
```

## Usage

Add to your `Cargo.toml`:

```toml
[dependencies]
pnf = { path = "path/to/bindings/rust" }
```

Example usage:

```rust
use pnf::{Chart, Indicators, ChartConfig, ConstructionMethod, BoxSizeMethod};

fn main() {
    let mut chart = Chart::new();

    let timestamp = 1640000000;
    chart.add_price(100.0, timestamp);
    chart.add_price(102.0, timestamp + 1);
    chart.add_price(104.0, timestamp + 2);

    println!("Columns: {}", chart.column_count());
    println!("Box size: {}", chart.box_size());

    let mut indicators = Indicators::new();
    indicators.calculate(&chart);

    println!("Bullish percent: {:.2}%", indicators.bullish_percent());
    println!("Signals: {}", indicators.signal_count());
    println!("Patterns: {}", indicators.pattern_count());
}
```

## Features

- Safe Rust API wrapping the C library
- Full support for:
  - Chart creation and manipulation
  - Technical indicators (SMA, Bollinger Bands, RSI, OBV)
  - Signal detection
  - Pattern recognition
  - Support/Resistance levels
  - Congestion zones
- Automatic memory management
- Thread-safe (Send + Sync)

## License

MIT
