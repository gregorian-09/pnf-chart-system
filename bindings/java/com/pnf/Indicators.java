/**
 */
package com.pnf;

/**
 * Technical indicators for Point and Figure chart analysis.
 * Provides moving averages, Bollinger Bands, RSI, OBV, pattern recognition,
 * signal detection, support/resistance levels, price objectives, and congestion zones.
 */
/**
 * Indicators binding type.
 */
public class Indicators implements AutoCloseable {
    static {
        System.loadLibrary("pnfjni");
    }

    private long nativePtr;
    private IndicatorConfig config;

    /**
     */
    /**
     * s API.
     * @return 
     */
    public Indicators() {
        nativePtr = nativeCreate();
        config = new IndicatorConfig();
    }

    /**
     */
    /**
     * s API.
     * @param config 
     * @return 
     */
    public Indicators(IndicatorConfig config) {
        nativePtr = nativeCreateWithConfig(
                config.smaShortPeriod,
                config.smaMediumPeriod,
                config.smaLongPeriod,
                config.bollingerPeriod,
                config.bollingerStdDevs,
                config.rsiPeriod,
                config.rsiOverbought,
                config.rsiOversold,
                config.bullishAlertThreshold,
                config.bearishAlertThreshold,
                config.supportResistanceThreshold,
                config.congestionMinColumns,
                config.congestionPriceRange
        );
        this.config = config;
    }

    /**
     * Reconfigure indicators with new parameters.
     * Changes take effect on the next calculate() call.
     */
    /**
     */
    /**
     * configure API.
     * @param config 
     */
    public void configure(IndicatorConfig config) {
        nativeConfigure(nativePtr,
                config.smaShortPeriod,
                config.smaMediumPeriod,
                config.smaLongPeriod,
                config.bollingerPeriod,
                config.bollingerStdDevs,
                config.rsiPeriod,
                config.rsiOverbought,
                config.rsiOversold,
                config.bullishAlertThreshold,
                config.bearishAlertThreshold,
                config.supportResistanceThreshold,
                config.congestionMinColumns,
                config.congestionPriceRange
        );
        this.config = config;
    }

    /** Returns the current indicator configuration */
    /**
     * getConfig API.
     * @return 
     */
    public IndicatorConfig getConfig() {
        return config;
    }

    /**
     * Calculate all indicators based on the given chart.
     */
    /**
     */
    /**
     * calculate API.
     * @param chart 
     */
    public void calculate(Chart chart) {
        nativeCalculate(nativePtr, chart.getNativePtr());
    }

    /**
     * Set SMA periods conveniently.
     */
    /**
     */
    /**
     * setSmaPeriods API.
     * @param shortPeriod 
     * @param mediumPeriod 
     * @param longPeriod 
     */
    public void setSmaPeriods(int shortPeriod, int mediumPeriod, int longPeriod) {
        config.smaShortPeriod = shortPeriod;
        config.smaMediumPeriod = mediumPeriod;
        config.smaLongPeriod = longPeriod;
        configure(config);
    }

    /**
     * Set Bollinger Band parameters conveniently.
     */
    /**
     */
    /**
     * setBollingerParams API.
     * @param period 
     * @param stdDevs 
     */
    public void setBollingerParams(int period, double stdDevs) {
        config.bollingerPeriod = period;
        config.bollingerStdDevs = stdDevs;
        configure(config);
    }

    /**
     * Set RSI parameters conveniently.
     */
    /**
     */
    /**
     * setRsiParams API.
     * @param period 
     * @param overbought 
     * @param oversold 
     */
    public void setRsiParams(int period, double overbought, double oversold) {
        config.rsiPeriod = period;
        config.rsiOverbought = overbought;
        config.rsiOversold = oversold;
        configure(config);
    }

    /** Get count of support levels */
    /**
     * supportLevelCount API.
     * @return 
     */
    public int supportLevelCount() {
        double[] prices = supportPrices();
        return prices != null ? prices.length : 0;
    }

    /** Get count of resistance levels */
    /**
     * resistanceLevelCount API.
     * @return 
     */
    public int resistanceLevelCount() {
        double[] prices = resistancePrices();
        return prices != null ? prices.length : 0;
    }

    /** Get count of congestion zones */
    /**
     * congestionZoneCount API.
     * @return 
     */
    public int congestionZoneCount() {
        CongestionZone[] zones = congestionZones();
        return zones != null ? zones.length : 0;
    }

    // ========== Moving Averages ==========

    /** Get short-term SMA value for a specific column */
    /**
     * smaShort API.
     * @param column 
     * @return 
     */
    public double smaShort(int column) {
        return nativeSmaShort(nativePtr, column);
    }

    /** Get medium-term SMA value for a specific column */
    /**
     * smaMedium API.
     * @param column 
     * @return 
     */
    public double smaMedium(int column) {
        return nativeSmaMiddle(nativePtr, column);
    }

    /** Get long-term SMA value for a specific column */
    /**
     * smaLong API.
     * @param column 
     * @return 
     */
    public double smaLong(int column) {
        return nativeSmaLong(nativePtr, column);
    }

    /** @deprecated Use smaShort() instead */
    @Deprecated
    /**
     */
    /**
     * sma5 API.
     * @param column 
     * @return 
     */
    public double sma5(int column) {
        return smaShort(column);
    }

    /** @deprecated Use smaMedium() instead */
    @Deprecated
    /**
     */
    /**
     * sma10 API.
     * @param column 
     * @return 
     */
    public double sma10(int column) {
        return smaMedium(column);
    }

    /** @deprecated Use smaLong() instead */
    @Deprecated
    /**
     */
    /**
     * sma20 API.
     * @param column 
     * @return 
     */
    public double sma20(int column) {
        return smaLong(column);
    }

    /** Get all short-term SMA values as an array */
    /**
     * smaShortValues API.
     * @return 
     */
    public double[] smaShortValues() {
        return nativeSmaShortValues(nativePtr);
    }

    /** Get all medium-term SMA values as an array */
    /**
     * smaMediumValues API.
     * @return 
     */
    public double[] smaMediumValues() {
        return nativeSmaMediumValues(nativePtr);
    }

    /** Get all long-term SMA values as an array */
    /**
     * smaLongValues API.
     * @return 
     */
    public double[] smaLongValues() {
        return nativeSmaLongValues(nativePtr);
    }

    // ========== Bollinger Bands ==========

    /** Get Bollinger Band middle band value for a specific column */
    /**
     * bollingerMiddle API.
     * @param column 
     * @return 
     */
    public double bollingerMiddle(int column) {
        return nativeBollingerMiddle(nativePtr, column);
    }

    /** Get Bollinger Band upper band value for a specific column */
    /**
     * bollingerUpper API.
     * @param column 
     * @return 
     */
    public double bollingerUpper(int column) {
        return nativeBollingerUpper(nativePtr, column);
    }

    /** Get Bollinger Band lower band value for a specific column */
    /**
     * bollingerLower API.
     * @param column 
     * @return 
     */
    public double bollingerLower(int column) {
        return nativeBollingerLower(nativePtr, column);
    }

    /** Check if column has valid Bollinger Band values */
    /**
     * hasBollingerValue API.
     * @param column 
     * @return 
     */
    public boolean hasBollingerValue(int column) {
        return nativeHasBollingerValue(nativePtr, column);
    }

    /** Check if price is above upper Bollinger Band */
    /**
     * isAboveUpperBand API.
     * @param column 
     * @param price 
     * @return 
     */
    public boolean isAboveUpperBand(int column, double price) {
        return nativeIsAboveUpperBand(nativePtr, column, price);
    }

    /** Check if price is below lower Bollinger Band */
    /**
     * isBelowLowerBand API.
     * @param column 
     * @param price 
     * @return 
     */
    public boolean isBelowLowerBand(int column, double price) {
        return nativeIsBelowLowerBand(nativePtr, column, price);
    }

    /** Get all Bollinger middle band values as an array */
    /**
     * bollingerMiddleValues API.
     * @return 
     */
    public double[] bollingerMiddleValues() {
        return nativeBollingerMiddleValues(nativePtr);
    }

    /** Get all Bollinger upper band values as an array */
    /**
     * bollingerUpperValues API.
     * @return 
     */
    public double[] bollingerUpperValues() {
        return nativeBollingerUpperValues(nativePtr);
    }

    /** Get all Bollinger lower band values as an array */
    /**
     * bollingerLowerValues API.
     * @return 
     */
    public double[] bollingerLowerValues() {
        return nativeBollingerLowerValues(nativePtr);
    }

    // ========== RSI ==========

    /** Get RSI value for a specific column */
    /**
     * rsi API.
     * @param column 
     * @return 
     */
    public double rsi(int column) {
        return nativeRsi(nativePtr, column);
    }

    /** Check if column has valid RSI value */
    /**
     * hasRsiValue API.
     * @param column 
     * @return 
     */
    public boolean hasRsiValue(int column) {
        return nativeHasRsiValue(nativePtr, column);
    }

    /** Check if RSI indicates overbought condition */
    /**
     * isOverbought API.
     * @param column 
     * @return 
     */
    public boolean isOverbought(int column) {
        return nativeIsOverbought(nativePtr, column);
    }

    /** Check if RSI indicates oversold condition */
    /**
     * isOversold API.
     * @param column 
     * @return 
     */
    public boolean isOversold(int column) {
        return nativeIsOversold(nativePtr, column);
    }

    /** Get all RSI values as an array */
    /**
     * rsiValues API.
     * @return 
     */
    public double[] rsiValues() {
        return nativeRsiValues(nativePtr);
    }

    // ========== On-Balance Volume ==========

    /** Get OBV value for a specific column */
    /**
     * obv API.
     * @param column 
     * @return 
     */
    public double obv(int column) {
        return nativeObv(nativePtr, column);
    }

    /** Get all OBV values as an array */
    /**
     * obvValues API.
     * @return 
     */
    public double[] obvValues() {
        return nativeObvValues(nativePtr);
    }

    // ========== Bullish Percent ==========

    /** Get current bullish percent value */
    /**
     * bullishPercent API.
     * @return 
     */
    public double bullishPercent() {
        return nativeBullishPercent(nativePtr);
    }

    /** Check if bullish percent indicates bullish alert */
    /**
     * isBullishAlert API.
     * @return 
     */
    public boolean isBullishAlert() {
        return nativeIsBullishAlert(nativePtr);
    }

    /** Check if bullish percent indicates bearish alert */
    /**
     * isBearishAlert API.
     * @return 
     */
    public boolean isBearishAlert() {
        return nativeIsBearishAlert(nativePtr);
    }

    // ========== Signals ==========

    /** Get the current (most recent) signal type */
    /**
     * currentSignal API.
     * @return 
     */
    public SignalType currentSignal() {
        return SignalType.values()[nativeCurrentSignal(nativePtr)];
    }

    /** Check if current signal is a buy signal */
    /**
     * hasBuySignal API.
     * @return 
     */
    public boolean hasBuySignal() {
        return nativeHasBuySignal(nativePtr);
    }

    /** Check if current signal is a sell signal */
    /**
     * hasSellSignal API.
     * @return 
     */
    public boolean hasSellSignal() {
        return nativeHasSellSignal(nativePtr);
    }

    /** Get count of all detected signals */
    /**
     * signalCount API.
     * @return 
     */
    public int signalCount() {
        return nativeSignalCount(nativePtr);
    }

    /** Get count of buy signals */
    /**
     * buySignalCount API.
     * @return 
     */
    public int buySignalCount() {
        return nativeBuySignalCount(nativePtr);
    }

    /** Get count of sell signals */
    /**
     * sellSignalCount API.
     * @return 
     */
    public int sellSignalCount() {
        return nativeSellSignalCount(nativePtr);
    }

    /** Get all detected signals */
    /**
     * signals API.
     * @return 
     */
    public Signal[] signals() {
        return nativeGetSignals(nativePtr);
    }

    /** Get all buy signals */
    /**
     * buySignals API.
     * @return 
     */
    public Signal[] buySignals() {
        return nativeGetBuySignals(nativePtr);
    }

    /** Get all sell signals */
    /**
     * sellSignals API.
     * @return 
     */
    public Signal[] sellSignals() {
        return nativeGetSellSignals(nativePtr);
    }

    // ========== Patterns ==========

    /** Get total count of detected patterns */
    /**
     * patternCount API.
     * @return 
     */
    public int patternCount() {
        return nativePatternCount(nativePtr);
    }

    /** Get count of bullish patterns */
    /**
     * bullishPatternCount API.
     * @return 
     */
    public int bullishPatternCount() {
        return nativeBullishPatternCount(nativePtr);
    }

    /** Get count of bearish patterns */
    /**
     * bearishPatternCount API.
     * @return 
     */
    public int bearishPatternCount() {
        return nativeBearishPatternCount(nativePtr);
    }

    /** Check if a specific pattern type was detected */
    /**
     * hasPattern API.
     * @param type 
     * @return 
     */
    public boolean hasPattern(PatternType type) {
        return nativeHasPattern(nativePtr, type.ordinal());
    }

    /** Get all detected patterns */
    /**
     * patterns API.
     * @return 
     */
    public Pattern[] patterns() {
        return nativeGetPatterns(nativePtr);
    }

    /** Get all bullish patterns */
    /**
     * bullishPatterns API.
     * @return 
     */
    public Pattern[] bullishPatterns() {
        return nativeGetBullishPatterns(nativePtr);
    }

    /** Get all bearish patterns */
    /**
     * bearishPatterns API.
     * @return 
     */
    public Pattern[] bearishPatterns() {
        return nativeGetBearishPatterns(nativePtr);
    }

    // ========== Support/Resistance ==========

    /** Get all support/resistance levels */
    /**
     * levels API.
     * @return 
     */
    public SupportResistanceLevel[] levels() {
        return nativeGetLevels(nativePtr);
    }

    /** Get support levels only */
    /**
     * supportLevels API.
     * @return 
     */
    public SupportResistanceLevel[] supportLevels() {
        return nativeGetSupportLevels(nativePtr);
    }

    /** Get resistance levels only */
    /**
     * resistanceLevels API.
     * @return 
     */
    public SupportResistanceLevel[] resistanceLevels() {
        return nativeGetResistanceLevels(nativePtr);
    }

    /** Get significant levels with minimum touch count */
    /**
     * significantLevels API.
     * @param minTouches 
     * @return 
     */
    public SupportResistanceLevel[] significantLevels(int minTouches) {
        return nativeGetSignificantLevels(nativePtr, minTouches);
    }

    /** Check if price is near a support level */
    /**
     * isNearSupport API.
     * @param price 
     * @param tolerance 
     * @return 
     */
    public boolean isNearSupport(double price, double tolerance) {
        return nativeIsNearSupport(nativePtr, price, tolerance);
    }

    /** Check if price is near a resistance level */
    /**
     * isNearResistance API.
     * @param price 
     * @param tolerance 
     * @return 
     */
    public boolean isNearResistance(double price, double tolerance) {
        return nativeIsNearResistance(nativePtr, price, tolerance);
    }

    /** Get support level prices as array */
    /**
     * supportPrices API.
     * @return 
     */
    public double[] supportPrices() {
        return nativeGetSupportPrices(nativePtr);
    }

    /** Get resistance level prices as array */
    /**
     * resistancePrices API.
     * @return 
     */
    public double[] resistancePrices() {
        return nativeGetResistancePrices(nativePtr);
    }

    // ========== Price Objectives ==========

    /** Get all price objectives */
    /**
     * priceObjectives API.
     * @return 
     */
    public PriceObjective[] priceObjectives() {
        return nativeGetPriceObjectives(nativePtr);
    }

    /** Get bullish price objectives */
    /**
     * bullishObjectives API.
     * @return 
     */
    public PriceObjective[] bullishObjectives() {
        return nativeGetBullishObjectives(nativePtr);
    }

    /** Get bearish price objectives */
    /**
     * bearishObjectives API.
     * @return 
     */
    public PriceObjective[] bearishObjectives() {
        return nativeGetBearishObjectives(nativePtr);
    }

    /** Get bullish target prices as array */
    /**
     * bullishTargets API.
     * @return 
     */
    public double[] bullishTargets() {
        return nativeGetBullishTargets(nativePtr);
    }

    /** Get bearish target prices as array */
    /**
     * bearishTargets API.
     * @return 
     */
    public double[] bearishTargets() {
        return nativeGetBearishTargets(nativePtr);
    }

    // ========== Congestion Zones ==========

    /** Get all detected congestion zones */
    /**
     * congestionZones API.
     * @return 
     */
    public CongestionZone[] congestionZones() {
        return nativeGetCongestionZones(nativePtr);
    }

    /** Check if a column is within a congestion zone */
    /**
     * isInCongestion API.
     * @param column 
     * @return 
     */
    public boolean isInCongestion(int column) {
        return nativeIsInCongestion(nativePtr, column);
    }

    /** Get the largest congestion zone */
    /**
     * largestCongestionZone API.
     * @return 
     */
    public CongestionZone largestCongestionZone() {
        return nativeGetLargestCongestionZone(nativePtr);
    }

    // ========== Summary/Output ==========

    /** Get text summary of all indicators */
    /**
     * summary API.
     * @return 
     */
    public String summary() {
        return nativeSummary(nativePtr);
    }

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return nativeToString(nativePtr);
    }

    @Override
    /**
     */
    /**
     * close API.
     */
    public void close() {
        if (nativePtr != 0) {
            nativeDestroy(nativePtr);
            nativePtr = 0;
        }
    }

    // ========== Native Methods ==========

    private static native long nativeCreate();
    private static native long nativeCreateWithConfig(
            int smaShortPeriod, int smaMediumPeriod, int smaLongPeriod,
            int bollingerPeriod, double bollingerStdDevs,
            int rsiPeriod, double rsiOverbought, double rsiOversold,
            double bullishAlertThreshold, double bearishAlertThreshold,
            double supportResistanceThreshold, int congestionMinColumns, double congestionPriceRange
    );
    private static native void nativeDestroy(long ptr);
    private static native void nativeConfigure(long ptr,
                                               int smaShortPeriod, int smaMediumPeriod, int smaLongPeriod,
                                               int bollingerPeriod, double bollingerStdDevs,
                                               int rsiPeriod, double rsiOverbought, double rsiOversold,
                                               double bullishAlertThreshold, double bearishAlertThreshold,
                                               double supportResistanceThreshold, int congestionMinColumns, double congestionPriceRange
    );
    private static native void nativeCalculate(long ptr, long chartPtr);

    // Moving averages
    private static native double nativeSmaShort(long ptr, int column);
    private static native double nativeSmaMiddle(long ptr, int column);
    private static native double nativeSmaLong(long ptr, int column);
    private static native double[] nativeSmaShortValues(long ptr);
    private static native double[] nativeSmaMediumValues(long ptr);
    private static native double[] nativeSmaLongValues(long ptr);

    // Bollinger Bands
    private static native double nativeBollingerMiddle(long ptr, int column);
    private static native double nativeBollingerUpper(long ptr, int column);
    private static native double nativeBollingerLower(long ptr, int column);
    private static native boolean nativeHasBollingerValue(long ptr, int column);
    private static native boolean nativeIsAboveUpperBand(long ptr, int column, double price);
    private static native boolean nativeIsBelowLowerBand(long ptr, int column, double price);
    private static native double[] nativeBollingerMiddleValues(long ptr);
    private static native double[] nativeBollingerUpperValues(long ptr);
    private static native double[] nativeBollingerLowerValues(long ptr);

    // RSI
    private static native double nativeRsi(long ptr, int column);
    private static native boolean nativeHasRsiValue(long ptr, int column);
    private static native boolean nativeIsOverbought(long ptr, int column);
    private static native boolean nativeIsOversold(long ptr, int column);
    private static native double[] nativeRsiValues(long ptr);

    // OBV
    private static native double nativeObv(long ptr, int column);
    private static native double[] nativeObvValues(long ptr);

    // Bullish Percent
    private static native double nativeBullishPercent(long ptr);
    private static native boolean nativeIsBullishAlert(long ptr);
    private static native boolean nativeIsBearishAlert(long ptr);

    // Signals
    private static native int nativeCurrentSignal(long ptr);
    private static native boolean nativeHasBuySignal(long ptr);
    private static native boolean nativeHasSellSignal(long ptr);
    private static native int nativeSignalCount(long ptr);
    private static native int nativeBuySignalCount(long ptr);
    private static native int nativeSellSignalCount(long ptr);
    private static native Signal[] nativeGetSignals(long ptr);
    private static native Signal[] nativeGetBuySignals(long ptr);
    private static native Signal[] nativeGetSellSignals(long ptr);

    // Patterns
    private static native int nativePatternCount(long ptr);
    private static native int nativeBullishPatternCount(long ptr);
    private static native int nativeBearishPatternCount(long ptr);
    private static native boolean nativeHasPattern(long ptr, int patternType);
    private static native Pattern[] nativeGetPatterns(long ptr);
    private static native Pattern[] nativeGetBullishPatterns(long ptr);
    private static native Pattern[] nativeGetBearishPatterns(long ptr);

    // Support/Resistance
    private static native SupportResistanceLevel[] nativeGetLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetSupportLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetResistanceLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetSignificantLevels(long ptr, int minTouches);
    private static native boolean nativeIsNearSupport(long ptr, double price, double tolerance);
    private static native boolean nativeIsNearResistance(long ptr, double price, double tolerance);
    private static native double[] nativeGetSupportPrices(long ptr);
    private static native double[] nativeGetResistancePrices(long ptr);

    // Price Objectives
    private static native PriceObjective[] nativeGetPriceObjectives(long ptr);
    private static native PriceObjective[] nativeGetBullishObjectives(long ptr);
    private static native PriceObjective[] nativeGetBearishObjectives(long ptr);
    private static native double[] nativeGetBullishTargets(long ptr);
    private static native double[] nativeGetBearishTargets(long ptr);

    // Congestion Zones
    private static native CongestionZone[] nativeGetCongestionZones(long ptr);
    private static native boolean nativeIsInCongestion(long ptr, int column);
    private static native CongestionZone nativeGetLargestCongestionZone(long ptr);

    // Summary
    private static native String nativeSummary(long ptr);
    private static native String nativeToString(long ptr);
}
