# Quick Start Guide

This page is the fast path. For full onboarding, use [Getting Started Overview](getting-started/overview.md).

## Quick Commands

```bash
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Release -DPNF_BUILD_VIEWER=OFF
cmake --build build-linux -j$(nproc)
ctest --test-dir build-linux --output-on-failure
```

## Binding Test Commands

```bash
PYTHONPATH=build-linux/python python3 -m pytest bindings/python/test_pypnf.py -q
(cd bindings/java && mvn test -Dmaven.repo.local=/tmp/m2 -Dnative.library.path=/home/gregorian-rayne/ChartSystem/build-linux/lib)
(cd bindings/rust && cargo test --release)
(cd bindings/csharp && DOTNET_CLI_HOME=/tmp/dotnet DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet test -c Release)
```

## Navigate to Full Docs

- [Install Prerequisites](getting-started/install.md)
- [Build and Test Matrix](getting-started/build-and-test.md)
- [First Chart Walkthrough](getting-started/first-chart.md)
- [Architecture Overview](architecture.md)
- [API Reference Home](index.md#api-reference)
