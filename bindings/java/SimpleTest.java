import com.pnf.*;

public class SimpleTest {
    public static void main(String[] args) {
        System.out.println("=== PnF Java Bindings Test ===\n");

        try (Chart chart = new Chart(); Indicators indicators = new Indicators()) {
            System.out.println("Chart created successfully");
            System.out.println("Initial column count: " + chart.columnCount());

            long timestamp = System.currentTimeMillis() / 1000;
            System.out.println("\nAdding price data...");
            for (int i = 0; i < 20; i++) {
                double price = 100.0 + i * 0.5;
                chart.addPrice(price, timestamp + i);
            }

            System.out.println("Column count after adding data: " + chart.columnCount());
            System.out.println("X column count: " + chart.xColumnCount());
            System.out.println("O column count: " + chart.oColumnCount());
            System.out.println("Box size: " + chart.boxSize());
            System.out.println("Has bullish bias: " + chart.hasBullishBias());
            System.out.println("Has bearish bias: " + chart.hasBearishBias());

            System.out.println("\nCalculating indicators...");
            indicators.calculate(chart);
            System.out.println("Indicators calculated");

            System.out.println("Bullish percent: " + indicators.bullishPercent());
            System.out.println("Signal count: " + indicators.signalCount());
            System.out.println("Pattern count: " + indicators.patternCount());
            System.out.println("Buy signals: " + indicators.buySignalCount());
            System.out.println("Sell signals: " + indicators.sellSignalCount());

            Signal[] signals = indicators.signals();
            System.out.println("\nSignals retrieved: " + signals.length);
            if (signals.length > 0) {
                System.out.println("First signal type: " + signals[0].type());
                System.out.println("First signal column: " + signals[0].columnIndex());
                System.out.println("First signal price: " + signals[0].price());
            }

            Pattern[] patterns = indicators.patterns();
            System.out.println("\nPatterns retrieved: " + patterns.length);
            if (patterns.length > 0) {
                System.out.println("First pattern type: " + patterns[0].type());
                System.out.println("First pattern column: " + patterns[0].columnIndex());
            }

            SupportResistanceLevel[] support = indicators.supportLevels();
            SupportResistanceLevel[] resistance = indicators.resistanceLevels();
            System.out.println("\nSupport levels: " + support.length);
            System.out.println("Resistance levels: " + resistance.length);

            System.out.println("\n=== Testing chart methods ===");
            if (chart.columnCount() > 0) {
                ColumnType colType = chart.columnType(0);
                System.out.println("First column type: " + colType);
                System.out.println("First column box count: " + chart.columnBoxCount(0));
                System.out.println("First column highest: " + chart.columnHighest(0));
                System.out.println("First column lowest: " + chart.columnLowest(0));
            }

            System.out.println("\n=== Chart toString ===");
            String chartStr = chart.toString();
            if (chartStr != null && chartStr.length() > 0) {
                System.out.println(chartStr.substring(0, Math.min(200, chartStr.length())) + "...");
            }

            System.out.println("\n=== Testing clear ===");
            chart.clear();
            System.out.println("Column count after clear: " + chart.columnCount());

            System.out.println("\n=== ALL TESTS PASSED ===");

        } catch (Exception e) {
            System.err.println("ERROR: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
}
