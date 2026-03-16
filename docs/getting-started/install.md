# Install Prerequisites

## Core Build Requirements

- `cmake` 3.16+
- C++17 compiler (`g++`, `clang++`, or MSVC)
- `ninja` or `make`

## Language Runtime Requirements

- Python 3.9+ and `pytest`
- Java 17+ and Maven 3.9+
- Rust stable toolchain (`cargo`)
- .NET SDK 8.0 (LTS)

## Linux (Ubuntu/WSL) Baseline

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build python3 python3-pip openjdk-17-jdk maven cargo
```

Install .NET 8 SDK using Microsoft packages for your distro.

## Verify Toolchain

```bash
cmake --version
g++ --version
python3 --version
mvn -version
cargo --version
dotnet --version
```

## Network Notes

Builds require network for:
- Maven dependency resolution
- Cargo crate index and dependencies
- Pip package installation

If running in restricted environments, pre-seed local caches and set local repositories (`/tmp/m2`, Cargo cache mirror, pip wheelhouse).
