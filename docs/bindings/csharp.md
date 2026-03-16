# C# Binding API (`PnF`)

Source:
- `bindings/csharp/Chart.cs`
- `bindings/csharp/Indicators.cs`
- `bindings/csharp/Types.cs`
- `bindings/csharp/NativeMethods.cs`

C# binding uses P/Invoke over the C ABI.

## Runtime and Framework

- Target framework: `.NET 8` (LTS)
- Native library resolution must find `libpnf.so`/`pnf.dll`

## Lifecycle and Disposal

- `Chart` and `Indicators` are native-backed and implement disposal semantics.
- Use `using` blocks or call `Dispose()` explicitly.
- Do not access methods after disposal.

## `Chart` API Groups

Creation:
- `Chart()`
- `Chart(ChartConfig config)`

Mutation:
- `AddData(high, low, close, timestamp)`
- `AddPrice(price, timestamp)`
- `AddOHLC(OHLC)`
- `Clear()`

Counts/properties:
- `ColumnCount`, `XColumnCount`, `OColumnCount`, `BoxSize`

Column/box reads:
- `GetColumnType(index)`
- `GetColumnBoxCount(index)`
- `GetColumnHighest(index)`
- `GetColumnLowest(index)`
- `GetBoxPrice(columnIndex, boxIndex)`
- `GetBoxType(columnIndex, boxIndex)`
- `GetBoxMarker(columnIndex, boxIndex)`

Context/export:
- `HasBullishBias`, `HasBearishBias`
- `IsAboveBullishSupport(price)`, `IsBelowBearishResistance(price)`
- `ToAscii()`, `ToJson()`, `ToString()`

## `Indicators` API Groups

Configuration:
- `Configure(IndicatorConfig)`
- `SetSmaPeriods(...)`
- `SetBollingerParams(...)`
- `SetRsiParams(...)`
- threshold setters for bullish percent, S/R, congestion

Execution:
- `Calculate(Chart)`

Retrieval:
- SMA/Bollinger/RSI/OBV values and arrays
- `BullishPercent`, alert booleans
- `CurrentSignal`, signal counts, `GetSignals()`
- pattern counts and getters (`GetPatterns`, bullish/bearish variants)
- support/resistance counts, prices, and level lists
- congestion APIs
- `Summary()`, `ToString()`

## Error Behavior

- Invalid index reads generally return default values.
- Native interop failures should be treated as fatal integration errors.

## Example

```csharp
using var chart = new Chart();
chart.AddPrice(100.0, DateTimeOffset.UtcNow.ToUnixTimeSeconds());
chart.AddPrice(103.0, DateTimeOffset.UtcNow.ToUnixTimeSeconds());

using var indicators = new Indicators();
indicators.Calculate(chart);
Console.WriteLine(indicators.Summary());
```
