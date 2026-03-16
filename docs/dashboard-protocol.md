# Dashboard Protocol

All localhost dashboard servers emit UTF-8 JSON text messages over WebSocket at `/ws`.

## Message Envelope

```json
{
  "type": "dashboard.snapshot",
  "version": 1,
  "timestamp": 1700000000000,
  "sequence": 1,
  "payload": {
    "meta": {},
    "chart": {},
    "indicators": {}
  }
}
```

## Required Fields

- `type`: always `dashboard.snapshot`
- `version`: protocol version, currently `1`
- `timestamp`: Unix epoch in milliseconds
- `sequence`: monotonically increasing integer per server instance
- `payload.meta.binding`: one of `python`, `java`, `rust`, `csharp`
- `payload.chart`: chart snapshot object
- `payload.indicators`: indicator snapshot object

## Chart Fields

- `box_size`
- `column_count`
- `x_column_count`
- `o_column_count`
- `has_bullish_bias`
- `has_bearish_bias`
- `columns`: array of `{ index, type, box_count, highest, lowest }`

## Indicator Fields

- `summary`
- `bullish_percent`
- `signal_count`
- `buy_signal_count`
- `sell_signal_count`
- `pattern_count`
- `support_level_count`
- `resistance_level_count`
- `congestion_zone_count`

Bindings may include more fields, but they must preserve the required fields above.
