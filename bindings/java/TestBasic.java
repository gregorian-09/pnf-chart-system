import com.pnf.*;

public class TestBasic {
    public static void main(String[] args) {
        System.out.println("Testing PnF Java Bindings with JNI...");

        try {
            System.out.println("\n1. Creating Chart with default config...");
            Chart chart = new Chart();
            System.out.println("   ✓ Chart created successfully");

            System.out.println("\n2. Adding price data...");
            long timestamp = System.currentTimeMillis() / 1000;
            chart.addPrice(100.0, timestamp);
            chart.addPrice(101.0, timestamp + 1);
            chart.addPrice(102.0, timestamp + 2);
            System.out.println("   ✓ Price data added");

            System.out.println("\n3. Checking chart properties...");
            System.out.println("   - Column count: " + chart.columnCount());
            System.out.println("   - X columns: " + chart.xColumnCount());
            System.out.println("   - O columns: " + chart.oColumnCount());
            System.out.println("   - Box size: " + chart.boxSize());

            System.out.println("\n4. Creating Indicators...");
            Indicators indicators = new Indicators();
            System.out.println("   ✓ Indicators created successfully");

            System.out.println("\n5. Calculating indicators...");
            indicators.calculate(chart);
            System.out.println("   ✓ Indicators calculated");

            System.out.println("\n6. Getting indicator values...");
            System.out.println("   - Bullish percent: " + indicators.bullishPercent());
            System.out.println("   - Current signal: " + indicators.currentSignal());
            System.out.println("   - Pattern count: " + indicators.patternCount());

            System.out.println("\n7. Cleaning up...");
            chart.close();
            indicators.close();
            System.out.println("   ✓ Resources cleaned up");

            System.out.println("\n✅ ALL TESTS PASSED! JNI bindings are working correctly.");

        } catch (Exception e) {
            System.err.println("\n❌ TEST FAILED!");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
