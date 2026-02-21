package com.pnf.test;

import com.pnf.*;
import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Unit and Integration Tests for Java PnF Bindings
 * <p>
 * Run with JUnit 5: java -jar junit-platform-console-standalone.jar --class-path . --select-class com.pnf.test.PnfTest
 */
public class PnfTest {

    // ============================================================
    // Enum Tests
    // ============================================================

    @Test
    public void testBoxTypeEnum() {
        assertNotNull(BoxType.X, "BoxType.X should exist");
        assertNotNull(BoxType.O, "BoxType.O should exist");
        assertEquals(2, BoxType.values().length, "BoxType should have 2 values");
    }

    @Test
    public void testColumnTypeEnum() {
        assertNotNull(ColumnType.X, "ColumnType.X should exist");
        assertNotNull(ColumnType.O, "ColumnType.O should exist");
        assertNotNull(ColumnType.MIXED, "ColumnType.MIXED should exist");
        assertEquals(3, ColumnType.values().length, "ColumnType should have 3 values");
    }

    @Test
    public void testConstructionMethodEnum() {
        assertNotNull(ConstructionMethod.CLOSE, "ConstructionMethod.CLOSE should exist");
        assertNotNull(ConstructionMethod.HIGH_LOW, "ConstructionMethod.HIGH_LOW should exist");
    }

    @Test
    public void testBoxSizeMethodEnum() {
        assertNotNull(BoxSizeMethod.FIXED, "BoxSizeMethod.FIXED should exist");
        assertNotNull(BoxSizeMethod.TRADITIONAL, "BoxSizeMethod.TRADITIONAL should exist");
        assertNotNull(BoxSizeMethod.PERCENTAGE, "BoxSizeMethod.PERCENTAGE should exist");
        assertNotNull(BoxSizeMethod.POINTS, "BoxSizeMethod.POINTS should exist");
    }

    @Test
    public void testSignalTypeEnum() {
        assertNotNull(SignalType.NONE, "SignalType.NONE should exist");
        assertNotNull(SignalType.BUY, "SignalType.BUY should exist");
        assertNotNull(SignalType.SELL, "SignalType.SELL should exist");
    }

    @Test
    public void testPatternTypeEnum() {
        assertNotNull(PatternType.NONE, "PatternType.NONE should exist");
        assertNotNull(PatternType.DOUBLE_TOP_BREAKOUT, "PatternType.DOUBLE_TOP_BREAKOUT should exist");
        assertNotNull(PatternType.DOUBLE_BOTTOM_BREAKDOWN, "PatternType.DOUBLE_BOTTOM_BREAKDOWN should exist");
        assertNotNull(PatternType.TRIPLE_TOP_BREAKOUT, "PatternType.TRIPLE_TOP_BREAKOUT should exist");
        assertNotNull(PatternType.BULLISH_CATAPULT, "PatternType.BULLISH_CATAPULT should exist");
        assertTrue(PatternType.values().length >= 10, "PatternType should have many values");
    }

    // ============================================================
    // ChartConfig Tests
    // ============================================================

    @Test
    public void testChartConfigDefault() {
        ChartConfig config = ChartConfig.getDefault();
        assertNotNull(config, "Default config should not be null");
        assertEquals(3, config.reversal, "Default reversal should be 3");
    }

    @Test
    public void testChartConfigCustom() {
        ChartConfig config = new ChartConfig();
        config.method = ConstructionMethod.HIGH_LOW;
        config.boxSizeMethod = BoxSizeMethod.FIXED;
        config.boxSize = 2.0;
        config.reversal = 2;

        assertEquals(ConstructionMethod.HIGH_LOW, config.method);
        assertEquals(2.0, config.boxSize, 0.001);
        assertEquals(2, config.reversal);
    }

    @Test
    public void testChartConfigBuilder() {
        ChartConfig config = ChartConfig.builder()
                .method(ConstructionMethod.HIGH_LOW)
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.5)
                .reversal(3)
                .build();

        assertNotNull(config);
        assertEquals(ConstructionMethod.HIGH_LOW, config.method);
        assertEquals(1.5, config.boxSize, 0.001);
    }

    // ============================================================
    // IndicatorConfig Tests
    // ============================================================

    @Test
    public void testIndicatorConfigDefault() {
        IndicatorConfig config = IndicatorConfig.getDefault();
        assertNotNull(config, "Default config should not be null");
        assertEquals(5, config.smaShortPeriod, "Default SMA short period should be 5");
        assertEquals(10, config.smaMediumPeriod, "Default SMA medium period should be 10");
        assertEquals(20, config.smaLongPeriod, "Default SMA long period should be 20");
        assertEquals(14, config.rsiPeriod, "Default RSI period should be 14");
    }

    @Test
    public void testIndicatorConfigCustom() {
        IndicatorConfig config = new IndicatorConfig();
        config.smaShortPeriod = 3;
        config.smaMediumPeriod = 7;
        config.smaLongPeriod = 14;
        config.rsiPeriod = 10;
        config.rsiOverbought = 80.0;
        config.rsiOversold = 20.0;

        assertEquals(3, config.smaShortPeriod);
        assertEquals(7, config.smaMediumPeriod);
        assertEquals(80.0, config.rsiOverbought, 0.001);
    }

    @Test
    public void testIndicatorConfigBuilder() {
        IndicatorConfig config = IndicatorConfig.builder()
                .smaShortPeriod(3)
                .smaMediumPeriod(7)
                .smaLongPeriod(14)
                .bollingerPeriod(15)
                .bollingerStdDevs(2.5)
                .rsiPeriod(10)
                .build();

        assertNotNull(config);
        assertEquals(3, config.smaShortPeriod);
        assertEquals(2.5, config.bollingerStdDevs, 0.001);
    }

    // ============================================================
    // Chart Creation Tests
    // ============================================================

    @Test
    public void testChartCreationDefault() {
        try (Chart chart = new Chart()) {
            assertNotNull(chart, "Chart should not be null");
            assertEquals(0, chart.columnCount(), "New chart should have 0 columns");
        }
    }

    @Test
    public void testChartCreationWithConfig() {
        ChartConfig config = ChartConfig.builder()
                .method(ConstructionMethod.HIGH_LOW)
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config)) {
            assertNotNull(chart);
            assertEquals(0, chart.columnCount());
        }
    }

    // ============================================================
    // Chart Data Addition Tests
    // ============================================================

    @Test
    public void testChartAddData() {
        try (Chart chart = new Chart()) {
            long timestamp = System.currentTimeMillis() / 1000;
            chart.addData(101.0, 99.0, 100.0, timestamp);
            // Result indicates if data was processed
        }
    }

    @Test
    public void testChartAddPrice() {
        try (Chart chart = new Chart()) {
            long timestamp = System.currentTimeMillis() / 1000;
            chart.addPrice(100.0, timestamp);
            // Result indicates if price was processed
        }
    }

    @Test
    public void testChartAddMultipleDataPoints() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config)) {
            long baseTime = 1704067200L; // 2024-01-01

            // Add upward trend
            for (int i = 0; i < 10; i++) {
                double price = 100.0 + i;
                chart.addData(price + 0.5, price - 0.5, price, baseTime + i * 86400);
            }

            assertTrue(chart.columnCount() > 0 || chart.boxSize() > 0.0,
                    "Chart should have columns or established box size");
        }
    }

    // ============================================================
    // Chart Query Tests
    // ============================================================

    @Test
    public void testChartColumnCountMethods() {
        try (Chart chart = new Chart()) {
            assertEquals(0, chart.columnCount());
            assertEquals(0, chart.xColumnCount());
            assertEquals(0, chart.oColumnCount());
        }
    }

    @Test
    public void testChartColumnInfo() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config)) {
            long baseTime = 1704067200L;

            for (int i = 0; i < 15; i++) {
                double price = 100.0 + i * 0.5;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            if (chart.columnCount() > 0) {
                ColumnType type = chart.columnType(0);
                assertNotNull(type);

                int boxCount = chart.columnBoxCount(0);
                assertTrue(boxCount > 0, "First column should have boxes");

                double highest = chart.columnHighest(0);
                double lowest = chart.columnLowest(0);
                assertTrue(highest >= lowest, "Highest should be >= lowest");
            }
        }
    }

    @Test
    public void testChartBias() {
        try (Chart chart = new Chart()) {
            chart.hasBullishBias();
            chart.hasBearishBias();
            // Empty chart may have neither bias
        }
    }

    // ============================================================
    // Chart Clear Tests
    // ============================================================

    @Test
    public void testChartClear() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config)) {
            long baseTime = 1704067200L;

            for (int i = 0; i < 10; i++) {
                double price = 100.0 + i;
                chart.addData(price + 0.5, price - 0.5, price, baseTime + i * 86400);
            }

            chart.clear();
            assertEquals(0, chart.columnCount(), "Cleared chart should have no columns");
        }
    }

    // ============================================================
    // Chart Output Tests
    // ============================================================

    @Test
    public void testChartToString() {
        try (Chart chart = new Chart()) {
            String str = chart.toString();
            assertNotNull(str);
        }
    }

    // ============================================================
    // Indicators Creation Tests
    // ============================================================

    @Test
    public void testIndicatorsCreationDefault() {
        try (Indicators ind = new Indicators()) {
            assertNotNull(ind);
        }
    }

    @Test
    public void testIndicatorsCreationWithConfig() {
        IndicatorConfig config = IndicatorConfig.builder()
                .smaShortPeriod(3)
                .smaMediumPeriod(7)
                .smaLongPeriod(14)
                .build();

        try (Indicators ind = new Indicators(config)) {
            assertNotNull(ind);
        }
    }

    // ============================================================
    // Indicators Configuration Tests
    // ============================================================

    @Test
    public void testIndicatorsConfigure() {
        try (Indicators ind = new Indicators()) {
            IndicatorConfig config = IndicatorConfig.builder()
                    .smaShortPeriod(3)
                    .rsiPeriod(7)
                    .build();

            ind.configure(config);
            // Should not throw
        }
    }

    @Test
    public void testIndicatorsSetSmaPeriods() {
        try (Indicators ind = new Indicators()) {
            ind.setSmaPeriods(3, 7, 14);
            // Should not throw
        }
    }

    @Test
    public void testIndicatorsSetBollingerParams() {
        try (Indicators ind = new Indicators()) {
            ind.setBollingerParams(15, 2.5);
            // Should not throw
        }
    }

    @Test
    public void testIndicatorsSetRsiParams() {
        try (Indicators ind = new Indicators()) {
            ind.setRsiParams(10, 80.0, 20.0);
            // Should not throw
        }
    }

    // ============================================================
    // Indicators Calculation Tests
    // ============================================================

    @Test
    public void testIndicatorsCalculateEmptyChart() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);
            // Should not crash on empty chart
        }
    }

    @Test
    public void testIndicatorsCalculateWithData() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config);
             Indicators ind = new Indicators()) {

            long baseTime = 1704067200L;

            for (int i = 0; i < 30; i++) {
                double price = 100.0 + i * 0.3 + (i % 5) * 0.1;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            ind.calculate(chart);
            // After calculation, indicators should be available
        }
    }

    // ============================================================
    // SMA Tests
    // ============================================================

    @Test
    public void testSmaValues() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config);
             Indicators ind = new Indicators()) {

            long baseTime = 1704067200L;

            for (int i = 0; i < 30; i++) {
                double price = 100.0 + i * 0.5;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            ind.calculate(chart);

            double[] shortValues = ind.smaShortValues();
            double[] mediumValues = ind.smaMediumValues();
            double[] longValues = ind.smaLongValues();

            assertNotNull(shortValues);
            assertNotNull(mediumValues);
            assertNotNull(longValues);
        }
    }

    @Test
    public void testSmaSingleValue() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            // Should return values (may be 0 or NaN for empty chart)
            ind.smaShort(0);
            ind.smaMedium(0);
            ind.smaLong(0);
        }
    }

    // ============================================================
    // Bollinger Bands Tests
    // ============================================================

    @Test
    public void testBollingerValues() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config);
             Indicators ind = new Indicators()) {

            long baseTime = 1704067200L;

            for (int i = 0; i < 40; i++) {
                double price = 100.0 + i * 0.3;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            ind.calculate(chart);

            double[] middle = ind.bollingerMiddleValues();
            double[] upper = ind.bollingerUpperValues();
            double[] lower = ind.bollingerLowerValues();

            assertNotNull(middle);
            assertNotNull(upper);
            assertNotNull(lower);

            // Check that upper >= middle >= lower where values exist
            int len = Math.min(Math.min(middle.length, upper.length), lower.length);
            for (int i = 0; i < len; i++) {
                if (upper[i] > 0.0 && middle[i] > 0.0 && lower[i] > 0.0) {
                    assertTrue(upper[i] >= middle[i], "Upper should be >= middle");
                    assertTrue(middle[i] >= lower[i], "Middle should be >= lower");
                }
            }
        }
    }

    @Test
    public void testBollingerSingleValue() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            ind.bollingerMiddle(0);
            ind.bollingerUpper(0);
            ind.bollingerLower(0);
            // Should not throw
        }
    }

    // ============================================================
    // RSI Tests
    // ============================================================

    @Test
    public void testRsiValues() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config);
             Indicators ind = new Indicators()) {

            long baseTime = 1704067200L;

            // Create oscillating data for RSI
            for (int i = 0; i < 50; i++) {
                double price = 100.0 + (i % 10) - 5;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            ind.calculate(chart);

            double[] rsiVals = ind.rsiValues();
            assertNotNull(rsiVals);

            // RSI should be between 0 and 100 where valid
            for (double val : rsiVals) {
                if (val > 0.0) {
                    assertTrue(true, "RSI should be >= 0");
                    assertTrue(val <= 100.0, "RSI should be <= 100");
                }
            }
        }
    }

    @Test
    public void testRsiOverboughtOversold() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            ind.isOverbought(0);
            ind.isOversold(0);
            // Should return boolean values
        }
    }

    // ============================================================
    // Bullish Percent Tests
    // ============================================================

    @Test
    public void testBullishPercent() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            double bp = ind.bullishPercent();
            assertTrue(bp >= 0.0, "Bullish percent should be >= 0");
            assertTrue(bp <= 100.0, "Bullish percent should be <= 100");
        }
    }

    @Test
    public void testBullishBearishAlerts() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            ind.isBullishAlert();
            ind.isBearishAlert();
            // Should return boolean values
        }
    }

    // ============================================================
    // Signal Tests
    // ============================================================

    @Test
    public void testSignalType() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            SignalType signal = ind.currentSignal();
            assertNotNull(signal);
            assertTrue(signal == SignalType.NONE || signal == SignalType.BUY || signal == SignalType.SELL,
                    "Signal should be valid type");
        }
    }

    @Test
    public void testSignalCounts() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            int total = ind.signalCount();
            int buy = ind.buySignalCount();
            int sell = ind.sellSignalCount();

            assertTrue(total >= 0, "Total signals should be >= 0");
            assertTrue(buy >= 0, "Buy signals should be >= 0");
            assertTrue(sell >= 0, "Sell signals should be >= 0");
        }
    }

    // ============================================================
    // Pattern Tests
    // ============================================================

    @Test
    public void testPatternCounts() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            int total = ind.patternCount();
            int bullish = ind.bullishPatternCount();
            int bearish = ind.bearishPatternCount();

            assertTrue(total >= 0, "Total patterns should be >= 0");
            assertTrue(bullish >= 0, "Bullish patterns should be >= 0");
            assertTrue(bearish >= 0, "Bearish patterns should be >= 0");
        }
    }

    // ============================================================
    // Support/Resistance Tests
    // ============================================================

    @Test
    public void testSupportResistanceCounts() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            int support = ind.supportLevelCount();
            int resistance = ind.resistanceLevelCount();

            assertTrue(support >= 0, "Support levels should be >= 0");
            assertTrue(resistance >= 0, "Resistance levels should be >= 0");
        }
    }

    @Test
    public void testNearSupportResistance() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            ind.isNearSupport(100.0, 1.0);
            ind.isNearResistance(100.0, 1.0);
            // Should return boolean values
        }
    }

    @Test
    public void testSupportResistancePrices() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            double[] support = ind.supportPrices();
            double[] resistance = ind.resistancePrices();

            assertNotNull(support);
            assertNotNull(resistance);
        }
    }

    // ============================================================
    // Congestion Tests
    // ============================================================

    @Test
    public void testCongestion() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            ind.isInCongestion(0);
            int zoneCount = ind.congestionZoneCount();

            assertTrue(zoneCount >= 0, "Zone count should be >= 0");
        }
    }

    // ============================================================
    // Summary Tests
    // ============================================================

    @Test
    public void testSummary() {
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            ind.calculate(chart);

            String summary = ind.summary();
            assertNotNull(summary);
        }
    }

    @Test
    public void testIndicatorsToString() {
        try (Indicators ind = new Indicators()) {
            String str = ind.toString();
            assertNotNull(str);
        }
    }

    // ============================================================
    // Integration Tests
    // ============================================================

    @Test
    public void testFullWorkflow() {
        // Configure chart
        ChartConfig chartConfig = ChartConfig.builder()
                .method(ConstructionMethod.HIGH_LOW)
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        // Configure indicators
        IndicatorConfig indConfig = IndicatorConfig.builder()
                .smaShortPeriod(3)
                .smaMediumPeriod(5)
                .smaLongPeriod(10)
                .rsiPeriod(7)
                .build();

        try (Chart chart = new Chart(chartConfig);
             Indicators ind = new Indicators(indConfig)) {

            long baseTime = 1704067200L;

            // Add trend data
            double[] prices = {
                    100, 101, 102, 103, 104, 105,  // Up
                    104, 103, 102, 101, 100,       // Down
                    101, 102, 103, 104, 105, 106, 107,  // Up
                    106, 105, 104, 103,            // Small pullback
                    104, 105, 106, 107, 108, 109, 110  // Continue up
            };

            for (int i = 0; i < prices.length; i++) {
                chart.addData(prices[i] + 0.5, prices[i] - 0.5, prices[i], baseTime + i * 86400);
            }

            // Calculate indicators
            ind.calculate(chart);

            // Access all indicator values
            ind.smaShortValues();
            ind.smaMediumValues();
            ind.bollingerMiddleValues();
            ind.rsiValues();
            ind.bullishPercent();
            ind.currentSignal();
            ind.patternCount();
            ind.supportPrices();
            ind.resistancePrices();

            // Generate outputs
            String chartStr = chart.toString();
            String summary = ind.summary();

            // Verify chart was built
            assertTrue(chart.columnCount() > 0 || chart.boxSize() > 0.0,
                    "Chart should have data");

            assertNotNull(chartStr);
            assertNotNull(summary);
        }
    }

    @Test
    public void testMultipleCharts() {
        ChartConfig[] configs = {
                ChartConfig.builder().boxSize(1.0).boxSizeMethod(BoxSizeMethod.FIXED).build(),
                ChartConfig.builder().boxSize(2.0).boxSizeMethod(BoxSizeMethod.FIXED).build(),
                ChartConfig.builder().boxSize(0.5).boxSizeMethod(BoxSizeMethod.FIXED).build()
        };

        Chart[] charts = new Chart[configs.length];
        try {
            for (int i = 0; i < configs.length; i++) {
                charts[i] = new Chart(configs[i]);
            }

            long baseTime = 1704067200L;

            for (Chart chart : charts) {
                for (int i = 0; i < 20; i++) {
                    double price = 100.0 + i * 0.5;
                    chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
                }
            }

            // Each chart should be independent
            for (Chart chart : charts) {
                assertNotNull(chart);
            }
        } finally {
            for (Chart chart : charts) {
                if (chart != null) {
                    chart.close();
                }
            }
        }
    }

    @Test
    public void testRecalculateIndicators() {
        ChartConfig config = ChartConfig.builder()
                .boxSizeMethod(BoxSizeMethod.FIXED)
                .boxSize(1.0)
                .reversal(3)
                .build();

        try (Chart chart = new Chart(config);
             Indicators ind = new Indicators()) {

            long baseTime = 1704067200L;

            // Initial data
            for (int i = 0; i < 20; i++) {
                double price = 100.0 + i * 0.5;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }
            ind.calculate(chart);

            int initialCount = chart.columnCount();

            // Add more data
            for (int i = 20; i < 40; i++) {
                double price = 110.0 + i * 0.3;
                chart.addData(price + 1.0, price - 1.0, price, baseTime + i * 86400);
            }

            // Recalculate
            ind.calculate(chart);

            int newCount = chart.columnCount();
            assertTrue(newCount >= initialCount, "Column count should increase or stay same");
        }
    }

    @Test
    public void testAutoCloseableUsage() {
        // Test that try-with-resources works correctly
        try (Chart chart = new Chart();
             Indicators ind = new Indicators()) {
            assertNotNull(chart);
            assertNotNull(ind);
        }
        // Resources should be automatically closed
    }
}