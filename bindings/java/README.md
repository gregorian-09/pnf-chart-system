# pnf-java-binding

[![Maven Central](https://img.shields.io/maven-central/v/io.github.gregorian-09/pnf-java-binding.svg)](https://search.maven.org/artifact/io.github.gregorian-09/pnf-java-binding)
[![Javadoc](https://javadoc.io/badge2/io.github.gregorian-09/pnf-java-binding/javadoc.svg)](https://javadoc.io/doc/io.github.gregorian-09/pnf-java-binding)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/gregorian-09/pnf-chart-system/blob/master/LICENSE)

Production-grade Java binding for the Point-and-Figure (PnF) analytics engine.

## Why This Package

`pnf-java-binding` covers full PnF analysis workflows:

| Area | Coverage |
| --- | --- |
| Chart Engine | `Close` and `HighLow` construction with configurable box sizing |
| Trend Context | Bullish bias / bearish bias and support-resistance context |
| Indicators | SMA, Bollinger Bands, RSI, OBV, Bullish Percent |
| Signals and Patterns | Buy/sell signal detection and named PnF patterns |
| Structure | Support/resistance levels, objectives, congestion zones |
| Visualization | Real-time localhost dashboard (`DashboardServer`) |

## Maven Dependency

```xml
<dependency>
  <groupId>io.github.gregorian-09</groupId>
  <artifactId>pnf-java-binding</artifactId>
  <version>0.1.0</version>
</dependency>
```

## Quick Start

```java
import com.pnf.Chart;
import com.pnf.ChartConfig;
import com.pnf.ConstructionMethod;
import com.pnf.BoxSizeMethod;
import com.pnf.Indicators;
import com.pnf.IndicatorConfig;

public final class QuickStart {
    public static void main(String[] args) {
        ChartConfig cfg = new ChartConfig();
        cfg.method = ConstructionMethod.HIGH_LOW;
        cfg.boxSizeMethod = BoxSizeMethod.TRADITIONAL;
        cfg.boxSize = 0.0;
        cfg.reversal = 3;

        try (Chart chart = new Chart(cfg); Indicators indicators = new Indicators(new IndicatorConfig())) {
            chart.addData(5000.0, 4950.0, 4985.0, 1700000000L);
            chart.addData(5040.0, 4980.0, 5030.0, 1700003600L);
            chart.addData(5065.0, 5010.0, 5055.0, 1700007200L);

            indicators.calculate(chart);

            System.out.println(chart.toAscii());
            System.out.println(indicators.summary());
        }
    }
}
```

## Trendline and Bias Checks

```java
double lastPrice = 5055.0;

System.out.println("Bullish bias: " + chart.hasBullishBias());
System.out.println("Bearish bias: " + chart.hasBearishBias());
System.out.println("Near support: " + indicators.isNearSupport(lastPrice, 1.0));
System.out.println("Near resistance: " + indicators.isNearResistance(lastPrice, 1.0));
```

## Indicators and Momentum

```java
int lastColumn = Math.max(0, chart.columnCount() - 1);

System.out.println("SMA short: " + indicators.smaShort(lastColumn));
System.out.println("SMA medium: " + indicators.smaMedium(lastColumn));
System.out.println("SMA long: " + indicators.smaLong(lastColumn));

System.out.println("Bollinger upper: " + indicators.bollingerUpper(lastColumn));
System.out.println("RSI: " + indicators.rsi(lastColumn));
System.out.println("OBV: " + indicators.obv(lastColumn));
System.out.println("Bullish %: " + indicators.bullishPercent());
```

## Signals and Patterns

```java
System.out.println("Current signal: " + indicators.currentSignal());
System.out.println("Signal count: " + indicators.signalCount());
System.out.println("Buy signals: " + indicators.buySignalCount());
System.out.println("Sell signals: " + indicators.sellSignalCount());

System.out.println("Pattern count: " + indicators.patternCount());
System.out.println("Bullish patterns: " + indicators.bullishPatternCount());
System.out.println("Bearish patterns: " + indicators.bearishPatternCount());

for (var pattern : indicators.patterns()) {
    System.out.println(pattern.type() + " column=" + pattern.columnIndex() + " price=" + pattern.price());
}
```

## Support, Resistance, Objectives, and Congestion

```java
System.out.println("Support levels: " + indicators.supportLevels().length);
System.out.println("Resistance levels: " + indicators.resistanceLevels().length);
System.out.println("Price objectives: " + indicators.priceObjectives().length);
System.out.println("Congestion zones: " + indicators.congestionZones().length);
```

## Real-Time Dashboard

```java
import com.pnf.DashboardServer;

try (DashboardServer server = new DashboardServer(chart, indicators)) {
    server.start("127.0.0.1", 8762);
    server.publish();
    System.out.println(server.url());
}
```

## Runtime Requirements

- Java 21+
- JNI library available at runtime

Example:

```bash
java \
  -Djava.library.path=/absolute/path/to/native/libs \
  -cp your-app.jar:pnf-java-binding-0.1.0.jar \
  com.example.Main
```

## API Overview

Core:
- `Chart`, `ChartConfig`

Indicators:
- `Indicators`, `IndicatorConfig`
- SMA/Bollinger/RSI/OBV values and vectors
- signal/pattern/support-resistance/objective/congestion APIs

Data records:
- `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`, `CongestionZone`

Dashboard:
- `DashboardServer`

Version helpers:
- `Version`

## Versioning

- Java artifact version follows the core engine release.
- Keep versions aligned across Rust/Python/C# when services share PnF outputs.
- See `CHANGELOG.md` for compatibility notes.

## Troubleshooting

- `UnsatisfiedLinkError`: check `-Djava.library.path` and architecture match.
- Missing indicator output: ensure chart has enough columns for configured periods.
- Unexpected behavior between services: confirm the same config defaults and package versions.

## Documentation and Links

- Javadoc: https://javadoc.io/doc/io.github.gregorian-09/pnf-java-binding
- Java binding reference: `docs/bindings/java.md`
- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
