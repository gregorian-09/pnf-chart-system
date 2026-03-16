# Indicators and Signals

## Indicator Stack

`Indicators` composes multiple analyzers and exposes unified retrieval methods.

```mermaid
flowchart TD
  A[Indicators.calculate(chart)] --> B[Moving Averages]
  A --> C[Bollinger Bands]
  A --> D[RSI]
  A --> E[On-Balance Volume]
  A --> F[Signal Detector]
  A --> G[Pattern Recognizer]
  A --> H[Support/Resistance]
  A --> I[Price Objectives]
  A --> J[Congestion]
```

## Moving Averages

- Independent short/medium/long period tracks
- Value lookup by column index
- Missing values for early columns before warm-up window

## Bollinger Bands

- middle/upper/lower tracks
- period and standard deviation multiplier are configurable
- useful for expansion/compression and relative extremes

## RSI

- per-column RSI values
- built-in overbought/oversold threshold checks
- configurable thresholds and period

## Signal Detector

- detects buy/sell events from chart column transitions and breakout logic
- exposes current signal and historical signals
- includes convenience counts (`buy_count`, `sell_count`)

## Operational Notes

- Calculations are column-based, not raw tick-based.
- Always call `calculate(chart)` after mutating chart input.
