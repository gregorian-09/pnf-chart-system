# Java Binding API (`com.pnf`)

Source:
- `bindings/java/pnf_java.cpp`
- `bindings/java/com/pnf/*.java`

For exhaustive generated symbol coverage, see:
- [API Symbol Index](../reference/api-symbol-index.md)

## Runtime Contract

- JNI bridge loaded by `System.loadLibrary("pnfjni")`
- `Chart`, `Indicators`, and `DashboardServer` are `AutoCloseable`
- Native library path required at runtime (`-Djava.library.path=...`)

## Enum Types

- `BoxSizeMethod`
- `BoxType`
- `ColumnType`
- `ConstructionMethod`
- `LevelType`
- `PatternType`
- `SignalType`

## Configuration and Data Records

- `ChartConfig` (+ builder)
- `IndicatorConfig` (+ builder + immutable `withX` updates)
- `Signal`
- `Pattern`
- `SupportResistanceLevel`
- `PriceObjective`
- `CongestionZone`

## `Chart`

- constructors: `Chart()`, `Chart(ChartConfig)`
- ingest: `addData(high, low, close, timestamp)`, `addPrice(price, timestamp)`
- column queries: `columnCount()`, `xColumnCount()`, `oColumnCount()`
- value queries: `boxSize()`, `columnType(i)`, `columnBoxCount(i)`, `columnHighest(i)`, `columnLowest(i)`
- box queries: `boxPrice(col, box)`, `boxType(col, box)`, `boxMarker(col, box)`
- trend/bias: `hasBullishBias()`, `hasBearishBias()`
- utility: `clear()`, `toString()`, `toAscii()`, `close()`

## `Indicators`

### Configuration and Execution
- `configure(config)`
- `getConfig()`
- `calculate(chart)`
- `setSmaPeriods(short, medium, long)`
- `setBollingerParams(period, stdDevs)`
- `setRsiParams(period, overbought, oversold)`

### Counts and Alerts
- `supportLevelCount()`, `resistanceLevelCount()`, `congestionZoneCount()`
- `bullishPercent()`, `isBullishAlert()`, `isBearishAlert()`
- `currentSignal()`, `hasBuySignal()`, `hasSellSignal()`
- `signalCount()`, `buySignalCount()`, `sellSignalCount()`
- `patternCount()`, `bullishPatternCount()`, `bearishPatternCount()`

### SMA/Bollinger/RSI/OBV
- SMA: `smaShort`, `smaMedium`, `smaLong`, `sma5`, `sma10`, `sma20`, and corresponding `*Values()`
- Bollinger: `bollingerMiddle`, `bollingerUpper`, `bollingerLower`, `hasBollingerValue`, `isAboveUpperBand`, `isBelowLowerBand`, and `*Values()`
- RSI: `rsi`, `hasRsiValue`, `isOverbought`, `isOversold`, `rsiValues()`
- OBV: `obv`, `obvValues()`

### Signals, Patterns, Levels, Objectives, Congestion
- signals: `signals()`, `buySignals()`, `sellSignals()`
- patterns: `hasPattern(type)`, `patterns()`, `bullishPatterns()`, `bearishPatterns()`
- levels: `levels()`, `supportLevels()`, `resistanceLevels()`, `significantLevels(minTouches)`, `isNearSupport`, `isNearResistance`, `supportPrices()`, `resistancePrices()`
- objectives: `priceObjectives()`, `bullishObjectives()`, `bearishObjectives()`, `bullishTargets()`, `bearishTargets()`
- congestion: `congestionZones()`, `isInCongestion(column)`, `largestCongestionZone()`

### Summary and Lifecycle
- `summary()`
- `toString()`
- `close()`

## `DashboardServer`

- chart/indicator assignment: `setChart(...)`, `setIndicators(...)`
- start/stop: `start()`, `start(host, port)`, `stop()`
- publish: `publish()`, `snapshotJson()`, `buildSnapshotJson(...)`
- streaming: `startAutoPublish()`, `startAutoPublish(intervalMs)`, `stopAutoPublish()`
- endpoint: `url()`
- lifecycle: `close()`
