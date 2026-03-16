# Installation Guide for Rust Bindings

## Prerequisites

### 1. Install Rust

If you don't have Rust installed, install it using rustup:

#### Windows
```powershell
# Download and run rustup-init.exe from https://rustup.rs/
# Or use PowerShell:
Invoke-WebRequest -Uri https://win.rustup.rs -OutFile rustup-init.exe
.\rustup-init.exe
```

#### Linux/macOS
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

After installation, restart your terminal or run:
```bash
source $HOME/.cargo/env
```

Verify installation:
```bash
cargo --version
rustc --version
```

### 2. Build the C Library

The Rust bindings require the PnF C shared library to be built first:

```bash
cd path/to/ChartSystem
mkdir -p build && cd build
cmake ..
cmake --build . --config Release
```

This will create:
- Windows: `build/bin/Release/pnf.dll`
- Linux: `build/lib/libpnf.so`
- macOS: `build/lib/libpnf.dylib`

## Building the Rust Bindings

### Option 1: Using CMake (Recommended)

```bash
cd path/to/ChartSystem/build
cmake .. -DPNF_BUILD_RUST=ON
cmake --build . --target pnf_rust
```

### Option 2: Using Cargo Directly

```bash
cd path/to/ChartSystem/bindings/rust
cargo build --release
```

## Running Tests

```bash
cd path/to/ChartSystem/bindings/rust

# Run all tests
cargo test

# Run tests with output
cargo test -- --nocapture

# Run specific test
cargo test test_chart_creation
```

## Running Examples

```bash
cd path/to/ChartSystem/bindings/rust

# Build and run the basic example
cargo run --example basic --release
```

## Using in Your Project

### Method 1: Local Path Dependency

In your `Cargo.toml`:

```toml
[dependencies]
pnf = { path = "../path/to/ChartSystem/bindings/rust" }
```

### Method 2: After Publishing to crates.io

```toml
[dependencies]
pnf = "0.1.0"
```

## Environment Setup

### Windows

Ensure the PnF DLL is in your PATH or in the same directory as your executable:

```powershell
$env:PATH += ";C:\path\to\ChartSystem\build\bin\Release"
```

Or copy `pnf.dll` to your project's target directory:

```powershell
Copy-Item "C:\path\to\ChartSystem\build\bin\Release\pnf.dll" -Destination "target\release\"
```

### Linux

Add the library path to LD_LIBRARY_PATH:

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/ChartSystem/build/lib
```

Or install system-wide:

```bash
sudo cp /path/to/ChartSystem/build/lib/libpnf.so /usr/local/lib/
sudo ldconfig
```

### macOS

Add the library path to DYLD_LIBRARY_PATH:

```bash
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/path/to/ChartSystem/build/lib
```

Or install system-wide:

```bash
sudo cp /path/to/ChartSystem/build/lib/libpnf.dylib /usr/local/lib/
```

## Troubleshooting

### "cannot find -lpnf" Error

The linker cannot find the PnF shared library. Make sure:
1. The C library has been built (`pnf.dll`, `libpnf.so`, or `libpnf.dylib` exists)
2. The library is in the search path (see Environment Setup above)
3. The `build.rs` file points to the correct library directory

### "undefined reference" Errors

The C library may be outdated. Rebuild it:

```bash
cd path/to/ChartSystem/build
cmake --build . --target pnf_shared --config Release
```

### Runtime "library not found" Errors

The shared library is not in the system's dynamic library search path:
- Windows: Add DLL location to PATH
- Linux: Add to LD_LIBRARY_PATH or install to /usr/local/lib
- macOS: Add to DYLD_LIBRARY_PATH or install to /usr/local/lib

## Development

### Enable Clippy Lints

```bash
cargo clippy
```

### Format Code

```bash
cargo fmt
```

### Generate Documentation

```bash
cargo doc --open
```

### Run Benchmarks (if added)

```bash
cargo bench
```
