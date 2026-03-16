# Compatibility Matrix

## Core

| Component | Baseline |
|---|---|
| C++ Standard | C++17 |
| CMake | 3.16+ |
| Linux | Ubuntu/WSL supported |
| Windows | Supported through toolchain/runtime parity |

## Bindings

| Binding | Runtime | Build Tool |
|---|---|---|
| Python | 3.9+ | CMake + pybind11 |
| Java | JDK 17+ | Maven + JNI |
| Rust | stable | Cargo + C FFI |
| C# | .NET 8 LTS | dotnet CLI + P/Invoke |

## Operational Notes

- Keep runtime and target framework aligned (`net8.0` for C#).
- Keep Java runtime architecture aligned with compiled JNI binary.
- For WSL environments, avoid reusing Windows-generated CMake caches.
