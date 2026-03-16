# Release and Versioning

## Versioning Model

- Use semantic versioning: `MAJOR.MINOR.PATCH`
- Align binding package versions with core release tag when possible

## Release Readiness Checklist

1. Core C++ tests pass.
2. All binding test suites pass.
3. ABI-sensitive changes reviewed (C ABI function signatures and structs).
4. Changelog updated.
5. Tag prepared and signed if required.

## Tagging

```bash
git tag v1.0.0
git push origin v1.0.0
```

## Package Registry Targets

- Python: PyPI
- Java: Maven Central
- Rust: crates.io
- C#: NuGet

Public registry-specific operational runbooks remain in `internal/`.
