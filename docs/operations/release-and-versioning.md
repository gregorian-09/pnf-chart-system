# Release and Versioning

## Policy

- Canonical project version is stored in root `VERSION`.
- Version scheme is strict semantic versioning: `MAJOR.MINOR.PATCH`.
- All publishable artifacts must share the exact same version:
  - C++ core (`CMakeLists.txt`, `headers/pnf/version.hpp`)
  - Python (`bindings/python/setup.py` via root `VERSION`)
  - Java (`bindings/java/pom.xml`)
  - Rust (`bindings/rust/Cargo.toml`)
  - C# (`bindings/csharp/PnF.csproj`)
- Public change history is tracked in root `CHANGELOG.md` using Keep a Changelog format.

## Required Tooling

- `tools/bump_version.py`
- `tools/check_versions.py`
- `tools/generate_api_symbol_index.py`
- `tools/check_docs_coverage.py`

## Version Bump Workflow

1. Bump all versions in one command.

```bash
python3 tools/bump_version.py 0.2.0
```

2. Update `CHANGELOG.md`:
- move completed entries from `Unreleased` into the new version section with date
- keep new `Unreleased` section for next cycle

3. Regenerate API index and validate documentation/version consistency.

```bash
python3 tools/generate_api_symbol_index.py
python3 tools/check_docs_coverage.py
python3 tools/check_versions.py
```

## Release Readiness Checklist

1. Core C++ build/tests pass.
2. Python, Java, Rust, and C# binding builds/tests pass.
3. C ABI changes reviewed for compatibility impact.
4. `CHANGELOG.md` updated and accurate.
5. `python3 tools/check_versions.py` passes.
6. `python3 tools/check_docs_coverage.py` passes.
7. Tag matches `VERSION` (for example `v0.2.0`).

## Tagging

```bash
git tag v0.2.0
git push origin v0.2.0
```

## Package Registry Targets

- Python: PyPI
- Java: Maven Central
- Rust: crates.io
- C#: NuGet

Registry-specific publishing runbooks remain in `internal/`.
