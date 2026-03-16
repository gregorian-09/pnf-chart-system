# Troubleshooting

## Java: `UnsatisfiedLinkError: no pnfjni`

Cause:
- JNI shared library not built or not in `java.library.path`

Resolution:
1. Build core + java JNI target.
2. Pass `-Dnative.library.path=/absolute/path/to/build-linux/lib` to Maven test.
3. Verify file exists: `libpnfjni.so`.

## Maven/CMake Cache Path Mismatch (Windows to WSL)

Cause:
- `CMakeCache.txt` generated from another OS path.

Resolution:
1. Delete stale build directory.
2. Reconfigure from WSL path.

## C# Test Host Requires .NET 6 but Only .NET 8 Installed

Cause:
- Project targets `net6.0` while runtime has only `net8.0`.

Resolution:
1. Retarget project to `net8.0`.
2. Re-run `dotnet test -c Release`.

## Python Cannot Import Module

Cause:
- `pypnf` not on `PYTHONPATH`.

Resolution:
- Run with `PYTHONPATH=build-linux/python`.

## Generic Diagnostics Checklist

1. Verify tool versions.
2. Rebuild from clean build directory.
3. Re-run only failing suite with verbose output.
4. Confirm ABI names and runtime library paths.
