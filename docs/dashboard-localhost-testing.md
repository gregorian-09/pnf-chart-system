# Localhost Dashboard Testing

The dashboard is a shared browser UI served by each binding on `127.0.0.1`.

## Routes

Each binding-local dashboard server exposes:

- `GET /`
- `GET /healthz`
- `GET /snapshot`
- `WS /ws`

## Smoke Checklist

1. Start the dashboard server.
2. Confirm `GET /healthz` returns `200 OK`.
3. Confirm `GET /snapshot` returns a `dashboard.snapshot` JSON payload.
4. Open `GET /` in a browser.
5. Confirm the page connects and renders chart + indicator panels.
6. Publish another snapshot and confirm the UI updates.

## Recommended Local Commands

### Python

Use `PYTHONPATH=build-linux/python:bindings/python` during local repo testing so both `pypnf` and `pypnf_dashboard` are importable.

### Java

Compile against `bindings/java/com/pnf/*.java` and set `-Djava.library.path=build-linux/lib` so JNI can load `libpnfjni.so`.

### Rust

Use `cargo check` or `cargo run` from `bindings/rust`.

### C#

Use `dotnet build` from `bindings/csharp`. In network-restricted environments, restore may fail if NuGet test packages are not already cached.
