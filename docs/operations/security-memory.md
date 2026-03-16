# Security and Memory Ownership

## C++ Core

- RAII ownership for native objects.
- Avoid shared mutable access to a `Chart` across threads.

## C ABI

Ownership is explicit:
- Create/destroy handles with matching functions.
- Free allocated strings/arrays with matching `pnf_free_*` APIs.

Incorrect free behavior can cause leaks or invalid frees.

## Binding Safety Responsibilities

- Python: object lifetime tied to Python wrappers.
- Java: `close()` native-backed objects (`AutoCloseable`).
- Rust: ensure `Drop` semantics map to matching C destroy/free calls.
- C#: implement and use `IDisposable` correctly.

## Secure Release Hygiene

- Scope registry tokens to minimal permissions.
- Use OIDC/trusted publishing where available.
- Rotate secrets periodically.
