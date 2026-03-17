# C# Binding API (`PnF`)

Source:
- `bindings/csharp/Chart.cs`
- `bindings/csharp/Indicators.cs`
- `bindings/csharp/Types.cs`
- `bindings/csharp/NativeMethods.cs`
- `bindings/csharp/Version.cs`
- `bindings/csharp/DashboardServer.cs`

For exhaustive generated symbol coverage, see:
- [API Symbol Index](../reference/api-symbol-index.md)

## Runtime and Ownership

- Target: `.NET 8`
- Native interop via P/Invoke (`NativeMethods`)
- `Chart`, `Indicators`, and `DashboardServer` implement disposal patterns

## Public Types (`Types.cs`)

### Enums
- `BoxType`
- `ColumnType`
- `ConstructionMethod`
- `BoxSizeMethod`
- `SignalType`
- `PatternType`

### Structs
- `ChartConfig`
- `IndicatorConfig`
- `OHLC`
- `Signal`
- `Pattern`
- `SupportResistanceLevel`
- `PnfDoubleArray`
- `PnfSignalArray`
- `PnfPatternArray`
- `PnfLevelArray`

## `Chart`

- constructors: `Chart()`, `Chart(ChartConfig)`
- ingest: `AddData(...)`, `AddPrice(...)`, `AddOHLC(...)`
- properties: `ColumnCount`, `XColumnCount`, `OColumnCount`, `BoxSize`, `HasBullishBias`, `HasBearishBias`
- column/box queries: `GetColumnType`, `GetColumnBoxCount`, `GetColumnHighest`, `GetColumnLowest`, `GetBoxPrice`, `GetBoxType`, `GetBoxMarker`
- trend checks: `IsAboveBullishSupport(price)`, `IsBelowBearishResistance(price)`
- utility/export: `Clear()`, `ToString()`, `ToAscii()`, `ToJson()`, `Dispose()`

## `Indicators`

### Configuration and Execution
- `Configure(config)`
- `SetSmaPeriods(...)`
- `SetBollingerParams(...)`
- `SetRsiParams(...)`
- `SetBullishPercentThresholds(...)`
- `SetSupportResistanceThreshold(...)`
- `SetCongestionParams(...)`
- `Calculate(chart)`

### Values and Collections
- SMA: `SmaShort`, `SmaMedium`, `SmaLong`, plus `SmaShortValues`, `SmaMediumValues`, `SmaLongValues`
- Bollinger: `BollingerMiddle`, `BollingerUpper`, `BollingerLower`, plus `Bollinger*Values`
- RSI/OBV: `Rsi`, `RsiIsOverbought`, `RsiIsOversold`, `RsiValues`, `Obv`, `ObvValues`
- signals: `CurrentSignal`, `SignalCount`, `BuySignalCount`, `SellSignalCount`, `GetSignals`
- patterns: `PatternCount`, `BullishPatternCount`, `BearishPatternCount`, `GetPatterns`, `GetBullishPatterns`, `GetBearishPatterns`
- levels: `SupportLevelCount`, `ResistanceLevelCount`, `IsNearSupport`, `IsNearResistance`, `GetSupportLevels`, `GetResistanceLevels`, `GetSupportPrices`, `GetResistancePrices`
- congestion: `CongestionZoneCount`, `IsInCongestion`
- summary: `BullishPercent`, `IsBullishAlert`, `IsBearishAlert`, `Summary()`, `ToString()`, `Dispose()`

## `Version`

- `Version.String`
- `Version.Major`
- `Version.Minor`
- `Version.Patch`
- `Version.Full`

## `DashboardServer`

- assignment: `SetChart(...)`, `SetIndicators(...)`
- server lifecycle: `Start(host, port)`, `Stop()`, `Dispose()`
- publish flow: `Publish()`, `SnapshotJson()`
- auto-publish: `StartAutoPublish(intervalMs)`, `StopAutoPublish()`
- status/properties: `IsRunning`, `Host`, `Port`, `Url()`, `Chart`, `Indicators`

## Low-Level Interop (`NativeMethods`)

`NativeMethods` exposes the full C ABI surface one-to-one for advanced callers. Most application code should use `Chart` and `Indicators` wrappers instead.
