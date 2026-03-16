# Rust Binding API (`pnf` crate)

Source:
- `bindings/rust/src/lib.rs`
- `bindings/rust/src/chart.rs`
- `bindings/rust/src/indicators.rs`
- `bindings/rust/src/types.rs`

The Rust crate wraps the C ABI and provides safe ergonomic wrappers over FFI calls.

## Crate Layers

```mermaid
flowchart LR
  R[Rust safe API] --> F[ffi.rs extern C]
  F --> C[C ABI]
  C --> CPP[C++ Core]
```

## Top-Level Version API

- `version()`
- `version_major()`
- `version_minor()`
- `version_patch()`

## `Chart` API

Creation:
- `Chart::new()`
- `Chart::with_config(config)`

Mutation:
- `add_data(...)`
- `add_price(...)`
- `add_ohlc(&OHLC)`
- `clear()`

Queries:
- `column_count()`, `x_column_count()`, `o_column_count()`
- `box_size()`
- `column_type(i)`, `column_box_count(i)`, `column_highest(i)`, `column_lowest(i)`
- `box_price(col, box)`, `box_type(col, box)`, `box_marker(col, box)`
- `has_bullish_bias()`, `has_bearish_bias()`
- `is_above_bullish_support(price)`, `is_below_bearish_resistance(price)`
- `to_string()`, `to_ascii()`, `to_json()`

## `Indicators` API

Configuration:
- `new()`, `with_config(config)`, `configure(config)`
- `set_sma_periods`, `set_bollinger_params`, `set_rsi_params`
- threshold/config setters for alerts and congestion

Execution:
- `calculate(&Chart)`

Retrieval:
- SMA/Bollinger/RSI/OBV scalar + vectors
- signal and pattern counts and lists
- support/resistance counts and levels
- congestion checks
- `summary()`, `to_string()`

## Ownership and Safety

- Wrappers own native handles.
- `Drop` releases underlying C handles.
- Arrays/strings from C ABI are copied into Rust-owned types before free.

## Example

```rust
use pnf::{Chart, Indicators, ChartConfig, BoxSizeMethod};

let mut cfg = ChartConfig::new();
cfg.box_size_method = BoxSizeMethod::Fixed;
cfg.box_size = 1.0;

let mut chart = Chart::with_config(cfg);
chart.add_price(100.0, 1);
chart.add_price(103.0, 2);

let mut ind = Indicators::new();
ind.calculate(&chart);
println!("{:?}", ind.summary());
```
