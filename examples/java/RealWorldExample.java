import com.pnf.*;
import java.io.*;
import java.util.*;

public class RealWorldExample {

    static class OHLCBar {
        String timestamp;
        double open;
        double high;
        double low;
        double close;
    }

    static List<OHLCBar> loadCSV(String filename) throws IOException {
        List<OHLCBar> data = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line = br.readLine();

            while ((line = br.readLine()) != null) {
                String[] fields = line.split(",");
                if (fields.length >= 6) {
                    OHLCBar bar = new OHLCBar();
                    bar.timestamp = fields[0];
                    bar.open = Double.parseDouble(fields[2]);
                    bar.high = Double.parseDouble(fields[3]);
                    bar.low = Double.parseDouble(fields[4]);
                    bar.close = Double.parseDouble(fields[5]);
                    data.add(bar);
                }
            }
        }

        System.out.println("Loaded " + data.size() + " bars from " + filename);
        return data;
    }

    static void processDataset(String csvFile, String outputPrefix) throws IOException {
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Processing: " + csvFile);
        System.out.println("=".repeat(60));

        List<OHLCBar> data = loadCSV(csvFile);
        if (data.isEmpty()) {
            System.err.println("No data loaded, skipping...");
            return;
        }

        ChartConfig.Builder configBuilder = ChartConfig.builder()
            .method(ConstructionMethod.HIGH_LOW)
            .reversal(3);

        if (csvFile.contains("GBPUSD") || csvFile.contains("EURUSD")) {
            configBuilder.boxSizeMethod(BoxSizeMethod.POINTS).boxSize(0.0020);
        } else {
            configBuilder.boxSizeMethod(BoxSizeMethod.TRADITIONAL).boxSize(0.0);
        }

        ChartConfig config = configBuilder.build();

        Chart chart = new Chart(config);

        long timestamp = System.currentTimeMillis();
        for (OHLCBar bar : data) {
            chart.addData(bar.high, bar.low, bar.close, timestamp);
            timestamp += 1000;
        }

        System.out.println("\nChart Statistics:");
        System.out.println("  Total columns: " + chart.columnCount());
        System.out.println("  X columns: " + chart.xColumnCount());
        System.out.println("  O columns: " + chart.oColumnCount());
        System.out.println("  Box size: " + chart.boxSize());
        System.out.println("  Has bullish bias: " + (chart.hasBullishBias() ? "Yes" : "No"));
        System.out.println("  Has bearish bias: " + (chart.hasBearishBias() ? "Yes" : "No"));

        IndicatorConfig indConfig = IndicatorConfig.builder()
            .smaShortPeriod(5)
            .smaMediumPeriod(10)
            .smaLongPeriod(20)
            .bollingerPeriod(20)
            .bollingerStdDevs(2.0)
            .rsiPeriod(14)
            .rsiOverbought(70.0)
            .rsiOversold(30.0)
            .build();

        Indicators indicators = new Indicators(indConfig);
        indicators.calculate(chart);

        System.out.println("\nIndicator Results:");
        System.out.println("  Bullish Percent: " + String.format("%.2f", indicators.bullishPercent()) + "%");
        System.out.println("  Signals: " + indicators.signalCount());
        System.out.println("  Patterns: " + indicators.patternCount());
        System.out.println("  Buy signals: " + indicators.buySignalCount());
        System.out.println("  Sell signals: " + indicators.sellSignalCount());
        System.out.println("  Support levels: " + indicators.supportLevelCount());
        System.out.println("  Resistance levels: " + indicators.resistanceLevelCount());

        Signal[] signals = indicators.signals();
        System.out.println("\nRecent Signals:");
        for (int i = 0; i < Math.min(5, signals.length); i++) {
            String signalType = signals[i].type() == SignalType.BUY ? "BUY" : "SELL";
            System.out.println("  Column " + signals[i].columnIndex() + ": " + signalType +
                             " (Price: " + String.format("%.5f", signals[i].price()) + ")");
        }

        Pattern[] patterns = indicators.patterns();
        System.out.println("\nRecent Patterns:");
        for (int i = 0; i < Math.min(5, patterns.length); i++) {
            String sentiment = patterns[i].isBullish() ? "BULLISH" : "BEARISH";
            System.out.println("  Column " + patterns[i].columnIndex() + ": " +
                             patterns[i].type() + " (" + sentiment + ", Price: " +
                             String.format("%.5f", patterns[i].price()) + ")");
        }

        System.out.println("\nProcessing complete!");

        chart.close();
        indicators.close();
    }

    public static void main(String[] args) {
        System.out.println("PnF Chart Library - Real World Data Example (Java)");

        try {
            processDataset("../../fixtures/GBPUSD_PERIOD_M1.csv", "output/gbpusd_m1");
            processDataset("../../fixtures/Boom_500_Index_PERIOD_H1.csv", "output/boom500_h1");
            processDataset("../../fixtures/Volatility_75_Index_PERIOD_D1.csv", "output/volatility75_d1");

            System.out.println("\n" + "=".repeat(60));
            System.out.println("All datasets processed successfully!");
            System.out.println("=".repeat(60));

        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
}
