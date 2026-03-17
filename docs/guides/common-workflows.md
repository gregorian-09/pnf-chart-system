# Common Workflows

## Workflow: Batch Build and Full Test Matrix

```bash
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Release -DPNF_BUILD_VIEWER=OFF
cmake --build build-linux -j$(nproc)
ctest --test-dir build-linux --output-on-failure
PYTHONPATH=build-linux/python python3 -m pytest bindings/python/test_pypnf.py -q
(cd bindings/java && mvn test -Dmaven.repo.local=/tmp/m2 -Dnative.library.path=$(cd ../.. && pwd)/build-linux/lib)
(cd bindings/rust && cargo test --release)
(cd bindings/csharp && DOTNET_CLI_HOME=/tmp/dotnet DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet test -c Release)
```

## Workflow: Add New Indicator End-to-End

1. Add implementation in `sources/pnf/indicators.cpp` and declarations in `headers/pnf/indicators.hpp`.
2. Expose in C ABI (`bindings/c/pnf_c.hpp`, `bindings/c/pnf_c.cpp`).
3. Expose in each binding API layer.
4. Add tests in C++ and every binding test suite.
5. Update docs in concepts and binding references.

## Workflow: Diagnose Native Library Loading Errors

1. Confirm native library exists in expected output path.
2. Confirm runtime search path includes that directory.
3. Confirm library file name conventions per platform:
- Linux: `libpnf.so`, `libpnfjni.so`
- Windows: `pnf.dll`, `pnfjni.dll`
4. Rebuild and re-run failing binding tests.
