# Numeric Examples

This page collects numeric, concrete examples for chart construction, indicators, and pattern detection.

## Chart Construction
### Box Size + Rounding

### Example 1: Traditional Box Size

- Input price: `37.25`
- Traditional table: `< 100.0` ⇒ `box_size = 1.0`
- Rounded up: `ceil(37.25 / 1.0) * 1.0 = 38.0`
- Rounded down: `floor(37.25 / 1.0) * 1.0 = 37.0`

### Example 2: Percentage Box Size

- Input price: `250.00`
- Config: `box_size_method = Percentage`, `box_size = 2.0`
- Computed box size: `250.00 * 2.0 / 100 = 5.00`
- Rounded up: `ceil(250 / 5) * 5 = 250`
- Rounded down: `floor(250 / 5) * 5 = 250`

### Example 3: Reversal Threshold (X Column)

- Column type: `X`
- Highest price: `105.0`
- Box size: `1.0`
- Reversal: `3`
- Reversal level: `105.0 - 3*1.0 = 102.0`
- If incoming price `<= 102.0`, reversal to `O`

### Example 4: Appending Boxes (X Column)

- Highest price: `100.0`
- Box size: `1.0`
- Incoming rounded price: `103.0`
- Boxes appended at: `101.0`, `102.0`, `103.0`

### Example 5: Appending Boxes (O Column)

- Lowest price: `110.0`
- Box size: `2.0`
- Incoming rounded price: `104.0`
- Boxes appended at: `108.0`, `106.0`, `104.0`

---

## Indicators
### Indicator Calculations

### Example 6: SMA (Column Averages)

- Period: `3`
- Column averages: `100`, `102`, `104`, `106`
- SMA values:
  - Column 0-1: `0.0` (insufficient data)
  - Column 2: `(100 + 102 + 104) / 3 = 102.0`
  - Column 3: `(102 + 104 + 106) / 3 = 104.0`

### Example 7: Bollinger Bands

- Period: `3`
- Column averages: `100`, `102`, `104`
- Mean: `102.0`
- Std dev: `sqrt(((100-102)^2 + (102-102)^2 + (104-102)^2) / 3) = sqrt(8/3) ≈ 1.633`
- Std dev multiplier: `2.0`
- Upper: `102 + 2*1.633 ≈ 105.266`
- Lower: `102 - 2*1.633 ≈ 98.734`

### Example 8: RSI

- Period: `3`
- Column averages: `100`, `102`, `101`, `103`
- Changes: `+2`, `-1`, `+2`
- Gains: `2, 0, 2` → avg gain = `(2+0+2)/3 = 1.333`
- Losses: `0, 1, 0` → avg loss = `(0+1+0)/3 = 0.333`
- RS: `1.333 / 0.333 = 4.0`
- RSI: `100 - 100/(1+4) = 80.0`

### Example 9: OBV

- OHLC closes: `100`, `101`, `100.5`, `102`
- Volumes: `10`, `20`, `15`, `30`
- OBV series:
  - Start: `0`
  - 101 > 100: `+20` → `20`
  - 100.5 < 101: `-15` → `5`
  - 102 > 100.5: `+30` → `35`

### Example 10: Bullish Percent

- Columns: `X, O, X, X, O`
- Bullish columns: `3`
- Total columns: `5`
- Bullish percent: `3/5 * 100 = 60%`

---

## Patterns and Levels
### Patterns, Levels, Congestion, Objectives

### Example 11: Double Top Breakout

- X columns highs: `100` (previous), `102` (current)
- Condition: current X high > previous X high
- Pattern: `DoubleTopBreakout` at current column

### Example 12: Triple Bottom Breakdown

- O columns lows: `50`, `50`, `48`
- Condition: last low < prior equal lows
- Pattern: `TripleBottomBreakdown` at current column

### Example 13: Bull Trap

- Previous X column has 1 box at high `110`
- Earlier X highs: `108`, `108`
- Condition: prev high > matching highs, followed by O column
- Pattern: `BullTrap`

### Example 14: Support/Resistance Merge

- Existing support levels: `100 (touches 2)`, new low `101`
- Threshold: `2%`
- Relative diff: `|101-100|/100 = 1%` → merge
- Resulting support: weighted price, touches incremented

### Example 15: Congestion Zone

- min_columns = `4`, threshold = `2%`
- Columns 2..5 range within `1.5%`
- Zone recorded: start=2, end=5, high/low range

### Example 16: Price Objective (Vertical Count)

- X column box_count = `6`
- Box size = `1.0`
- Highest price in column = `105`
- Bullish target = `105 + 6*1.0 = 111`

---
