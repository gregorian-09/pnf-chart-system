# pnf-java-binding

`pnf-java-binding` is the official Java binding for the PnF (Point and Figure) chart engine.

It provides:
- Native-performance chart construction (JNI to C++ core).
- Technical indicators and pattern/signal analysis.
- A localhost real-time dashboard server for browser visualization.

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
import com.pnf.Indicators;

public class Example {
    public static void main(String[] args) {
        Chart chart = new Chart();
        chart.addPrice(5000.0, System.currentTimeMillis() / 1000);
        chart.addPrice(5030.0, System.currentTimeMillis() / 1000 + 1);

        Indicators indicators = new Indicators();
        indicators.calculate(chart);

        System.out.println(chart.toAscii());
        System.out.println(indicators.summary());
    }
}
```

## Real-Time Dashboard

```java
import com.pnf.DashboardServer;

DashboardServer server = new DashboardServer(chart, indicators);
server.start("127.0.0.1", 8762);
server.publish();
System.out.println(server.url());
```

## Runtime Requirements

- Java 21+
- Native JNI library available in `java.library.path`

Example:

```bash
java -Djava.library.path=build-linux/lib -cp your_app.jar:pnf-java-binding.jar YourMainClass
```

## API Highlights

- Core:
  - `Chart`
  - `ChartConfig`
- Indicators:
  - `Indicators`
  - `IndicatorConfig`
- Data records:
  - `Signal`, `Pattern`, `SupportResistanceLevel`, `PriceObjective`, `CongestionZone`
- Dashboard:
  - `DashboardServer`
- Version:
  - `Version` helpers and native version APIs

## Documentation

- Java API reference: `docs/bindings/java.md`
- Full symbol index: `docs/reference/api-symbol-index.md`
- Project docs: `docs/index.md`

## Troubleshooting

- `UnsatisfiedLinkError`: verify `-Djava.library.path` points to JNI/native binaries.
- If using custom packaging, ensure native files are copied with your artifact.
- Keep Java binding version synchronized with core library version.

## Links

- Source: https://github.com/gregorian-09/pnf-chart-system
- Issues: https://github.com/gregorian-09/pnf-chart-system/issues
- Changelog: https://github.com/gregorian-09/pnf-chart-system/blob/master/CHANGELOG.md
