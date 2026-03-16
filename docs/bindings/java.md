# Java Binding API (`com.pnf`)

Source:
- `bindings/java/pnf_java.cpp`
- `bindings/java/com/pnf/*.java`

The Java binding uses JNI and loads native library `pnfjni`.

## Runtime Contract

- Java classes own native pointers (`long nativePtr`).
- `Chart` and `Indicators` implement `AutoCloseable`.
- Always call `close()` or use try-with-resources.

## Loading Native Library

```java
static { System.loadLibrary("pnfjni"); }
```

Set runtime path for tests/run:
- `-Dnative.library.path=/absolute/path/to/build-linux/lib`

## `Chart` API Groups

Creation:
- `Chart()`
- `Chart(ChartConfig config)`

Mutation:
- `addData(high, low, close, timestamp)`
- `addPrice(price, timestamp)`
- `clear()`

Queries:
- `columnCount()`, `xColumnCount()`, `oColumnCount()`
- `boxSize()`
- `columnType(i)`, `columnBoxCount(i)`, `columnHighest(i)`, `columnLowest(i)`
- `hasBullishBias()`, `hasBearishBias()`

Lifecycle:
- `close()`
- `toString()`

## `Indicators` API Groups

Configuration:
- `configure(config)`
- `setSmaPeriods(...)`
- `setBollingerParams(...)`
- `setRsiParams(...)`

Execution:
- `calculate(chart)`

Retrieval:
- SMA, Bollinger, RSI, OBV values and arrays
- signal APIs (`currentSignal`, `signals`, counts)
- pattern APIs (`patterns`, bullish/bearish subsets)
- support/resistance APIs
- price objective APIs
- congestion APIs
- `summary()`, `toString()`

Lifecycle:
- `close()`

## Error Model

- Invalid indexes typically return default values.
- Access after `close()` should be treated as invalid usage.

## Example

```java
try (Chart chart = new Chart(); Indicators ind = new Indicators()) {
    chart.addPrice(100.0, System.currentTimeMillis());
    chart.addPrice(103.0, System.currentTimeMillis());
    ind.calculate(chart);
    System.out.println(ind.summary());
}
```
