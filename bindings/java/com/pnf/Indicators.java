package com.pnf;

/**
 * Java binding for native indicator calculations on Point and Figure charts.
 */
public class Indicators implements AutoCloseable {
    static {
        System.loadLibrary("pnfjni");
    }

    private long nativePtr;
    private IndicatorConfig config;

    /**
     * Creates an indicator engine with default configuration.
     */
    public Indicators() {
        nativePtr = nativeCreate();
        config = new IndicatorConfig();
    }

    /**
     * Creates an indicator engine with custom configuration.
     *
     * @param config indicator configuration
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
     * Applies a new indicator configuration.
     *
     * @param config indicator configuration
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

    /**
     * Returns the current configuration.
     *
     * @return active indicator configuration
     */
    public IndicatorConfig getConfig() {
        return config;
    }

    /**
     * Runs indicator calculations for the supplied chart.
     *
     * @param chart chart to analyze
     */
    public void calculate(Chart chart) {
        nativeCalculate(nativePtr, chart.getNativePtr());
    }

    /**
     * Sets SMA periods and reapplies configuration.
     *
     * @param shortPeriod short SMA period
     * @param mediumPeriod medium SMA period
     * @param longPeriod long SMA period
     */
    public void setSmaPeriods(int shortPeriod, int mediumPeriod, int longPeriod) {
        config.smaShortPeriod = shortPeriod;
        config.smaMediumPeriod = mediumPeriod;
        config.smaLongPeriod = longPeriod;
        configure(config);
    }

    /**
     * Sets Bollinger settings and reapplies configuration.
     *
     * @param period Bollinger lookback period
     * @param stdDevs standard-deviation multiplier
     */
    public void setBollingerParams(int period, double stdDevs) {
        config.bollingerPeriod = period;
        config.bollingerStdDevs = stdDevs;
        configure(config);
    }

    /**
     * Sets RSI settings and reapplies configuration.
     *
     * @param period RSI period
     * @param overbought overbought threshold
     * @param oversold oversold threshold
     */
    public void setRsiParams(int period, double overbought, double oversold) {
        config.rsiPeriod = period;
        config.rsiOverbought = overbought;
        config.rsiOversold = oversold;
        configure(config);
    }

    /**
     * Returns count of support levels.
     *
     * @return support-level count
     */
    public int supportLevelCount() {
        double[] prices = supportPrices();
        return prices != null ? prices.length : 0;
    }

    /**
     * Returns count of resistance levels.
     *
     * @return resistance-level count
     */
    public int resistanceLevelCount() {
        double[] prices = resistancePrices();
        return prices != null ? prices.length : 0;
    }

    /**
     * Returns count of detected congestion zones.
     *
     * @return congestion-zone count
     */
    public int congestionZoneCount() {
        CongestionZone[] zones = congestionZones();
        return zones != null ? zones.length : 0;
    }

    /**
     * Returns short SMA at a column.
     *
     * @param column zero-based column index
     * @return short SMA value
     */
    public double smaShort(int column) {
        return nativeSmaShort(nativePtr, column);
    }

    /**
     * Returns medium SMA at a column.
     *
     * @param column zero-based column index
     * @return medium SMA value
     */
    public double smaMedium(int column) {
        return nativeSmaMiddle(nativePtr, column);
    }

    /**
     * Returns long SMA at a column.
     *
     * @param column zero-based column index
     * @return long SMA value
     */
    public double smaLong(int column) {
        return nativeSmaLong(nativePtr, column);
    }

    /**
     * Legacy alias for {@link #smaShort(int)}.
     *
     * @param column zero-based column index
     * @return short SMA value
     * @deprecated use {@link #smaShort(int)}
     */
    @Deprecated
    public double sma5(int column) {
        return smaShort(column);
    }

    /**
     * Legacy alias for {@link #smaMedium(int)}.
     *
     * @param column zero-based column index
     * @return medium SMA value
     * @deprecated use {@link #smaMedium(int)}
     */
    @Deprecated
    public double sma10(int column) {
        return smaMedium(column);
    }

    /**
     * Legacy alias for {@link #smaLong(int)}.
     *
     * @param column zero-based column index
     * @return long SMA value
     * @deprecated use {@link #smaLong(int)}
     */
    @Deprecated
    public double sma20(int column) {
        return smaLong(column);
    }

    /**
     * Returns all short SMA values.
     *
     * @return array of short SMA values
     */
    public double[] smaShortValues() {
        return nativeSmaShortValues(nativePtr);
    }

    /**
     * Returns all medium SMA values.
     *
     * @return array of medium SMA values
     */
    public double[] smaMediumValues() {
        return nativeSmaMediumValues(nativePtr);
    }

    /**
     * Returns all long SMA values.
     *
     * @return array of long SMA values
     */
    public double[] smaLongValues() {
        return nativeSmaLongValues(nativePtr);
    }

    /**
     * Returns Bollinger middle band at a column.
     *
     * @param column zero-based column index
     * @return middle-band value
     */
    public double bollingerMiddle(int column) {
        return nativeBollingerMiddle(nativePtr, column);
    }

    /**
     * Returns Bollinger upper band at a column.
     *
     * @param column zero-based column index
     * @return upper-band value
     */
    public double bollingerUpper(int column) {
        return nativeBollingerUpper(nativePtr, column);
    }

    /**
     * Returns Bollinger lower band at a column.
     *
     * @param column zero-based column index
     * @return lower-band value
     */
    public double bollingerLower(int column) {
        return nativeBollingerLower(nativePtr, column);
    }

    /**
     * Indicates whether Bollinger values are valid at a column.
     *
     * @param column zero-based column index
     * @return {@code true} if values exist
     */
    public boolean hasBollingerValue(int column) {
        return nativeHasBollingerValue(nativePtr, column);
    }

    /**
     * Checks whether price is above the upper Bollinger band.
     *
     * @param column zero-based column index
     * @param price price to test
     * @return {@code true} if price is above upper band
     */
    public boolean isAboveUpperBand(int column, double price) {
        return nativeIsAboveUpperBand(nativePtr, column, price);
    }

    /**
     * Checks whether price is below the lower Bollinger band.
     *
     * @param column zero-based column index
     * @param price price to test
     * @return {@code true} if price is below lower band
     */
    public boolean isBelowLowerBand(int column, double price) {
        return nativeIsBelowLowerBand(nativePtr, column, price);
    }

    /**
     * Returns all Bollinger middle-band values.
     *
     * @return middle-band value array
     */
    public double[] bollingerMiddleValues() {
        return nativeBollingerMiddleValues(nativePtr);
    }

    /**
     * Returns all Bollinger upper-band values.
     *
     * @return upper-band value array
     */
    public double[] bollingerUpperValues() {
        return nativeBollingerUpperValues(nativePtr);
    }

    /**
     * Returns all Bollinger lower-band values.
     *
     * @return lower-band value array
     */
    public double[] bollingerLowerValues() {
        return nativeBollingerLowerValues(nativePtr);
    }

    /**
     * Returns RSI value at a column.
     *
     * @param column zero-based column index
     * @return RSI value
     */
    public double rsi(int column) {
        return nativeRsi(nativePtr, column);
    }

    /**
     * Indicates whether RSI value is available at a column.
     *
     * @param column zero-based column index
     * @return {@code true} if RSI exists
     */
    public boolean hasRsiValue(int column) {
        return nativeHasRsiValue(nativePtr, column);
    }

    /**
     * Indicates whether RSI is above overbought threshold.
     *
     * @param column zero-based column index
     * @return {@code true} if overbought
     */
    public boolean isOverbought(int column) {
        return nativeIsOverbought(nativePtr, column);
    }

    /**
     * Indicates whether RSI is below oversold threshold.
     *
     * @param column zero-based column index
     * @return {@code true} if oversold
     */
    public boolean isOversold(int column) {
        return nativeIsOversold(nativePtr, column);
    }

    /**
     * Returns all RSI values.
     *
     * @return RSI value array
     */
    public double[] rsiValues() {
        return nativeRsiValues(nativePtr);
    }

    /**
     * Returns OBV value at a column.
     *
     * @param column zero-based column index
     * @return OBV value
     */
    public double obv(int column) {
        return nativeObv(nativePtr, column);
    }

    /**
     * Returns all OBV values.
     *
     * @return OBV value array
     */
    public double[] obvValues() {
        return nativeObvValues(nativePtr);
    }

    /**
     * Returns current bullish-percent value.
     *
     * @return bullish percent
     */
    public double bullishPercent() {
        return nativeBullishPercent(nativePtr);
    }

    /**
     * Indicates bullish-alert condition.
     *
     * @return {@code true} if bullish alert is active
     */
    public boolean isBullishAlert() {
        return nativeIsBullishAlert(nativePtr);
    }

    /**
     * Indicates bearish-alert condition.
     *
     * @return {@code true} if bearish alert is active
     */
    public boolean isBearishAlert() {
        return nativeIsBearishAlert(nativePtr);
    }

    /**
     * Returns the current signal type.
     *
     * @return most recent signal
     */
    public SignalType currentSignal() {
        return SignalType.values()[nativeCurrentSignal(nativePtr)];
    }

    /**
     * Indicates whether current signal is buy.
     *
     * @return {@code true} if buy signal exists
     */
    public boolean hasBuySignal() {
        return nativeHasBuySignal(nativePtr);
    }

    /**
     * Indicates whether current signal is sell.
     *
     * @return {@code true} if sell signal exists
     */
    public boolean hasSellSignal() {
        return nativeHasSellSignal(nativePtr);
    }

    /**
     * Returns number of detected signals.
     *
     * @return signal count
     */
    public int signalCount() {
        return nativeSignalCount(nativePtr);
    }

    /**
     * Returns number of buy signals.
     *
     * @return buy-signal count
     */
    public int buySignalCount() {
        return nativeBuySignalCount(nativePtr);
    }

    /**
     * Returns number of sell signals.
     *
     * @return sell-signal count
     */
    public int sellSignalCount() {
        return nativeSellSignalCount(nativePtr);
    }

    /**
     * Returns all signals.
     *
     * @return signal array
     */
    public Signal[] signals() {
        return nativeGetSignals(nativePtr);
    }

    /**
     * Returns all buy signals.
     *
     * @return buy-signal array
     */
    public Signal[] buySignals() {
        return nativeGetBuySignals(nativePtr);
    }

    /**
     * Returns all sell signals.
     *
     * @return sell-signal array
     */
    public Signal[] sellSignals() {
        return nativeGetSellSignals(nativePtr);
    }

    /**
     * Returns total pattern count.
     *
     * @return pattern count
     */
    public int patternCount() {
        return nativePatternCount(nativePtr);
    }

    /**
     * Returns bullish pattern count.
     *
     * @return bullish-pattern count
     */
    public int bullishPatternCount() {
        return nativeBullishPatternCount(nativePtr);
    }

    /**
     * Returns bearish pattern count.
     *
     * @return bearish-pattern count
     */
    public int bearishPatternCount() {
        return nativeBearishPatternCount(nativePtr);
    }

    /**
     * Checks whether a pattern type is present.
     *
     * @param type pattern type
     * @return {@code true} if pattern exists
     */
    public boolean hasPattern(PatternType type) {
        return nativeHasPattern(nativePtr, type.ordinal());
    }

    /**
     * Returns all detected patterns.
     *
     * @return pattern array
     */
    public Pattern[] patterns() {
        return nativeGetPatterns(nativePtr);
    }

    /**
     * Returns bullish patterns.
     *
     * @return bullish-pattern array
     */
    public Pattern[] bullishPatterns() {
        return nativeGetBullishPatterns(nativePtr);
    }

    /**
     * Returns bearish patterns.
     *
     * @return bearish-pattern array
     */
    public Pattern[] bearishPatterns() {
        return nativeGetBearishPatterns(nativePtr);
    }

    /**
     * Returns all support/resistance levels.
     *
     * @return level array
     */
    public SupportResistanceLevel[] levels() {
        return nativeGetLevels(nativePtr);
    }

    /**
     * Returns support levels.
     *
     * @return support-level array
     */
    public SupportResistanceLevel[] supportLevels() {
        return nativeGetSupportLevels(nativePtr);
    }

    /**
     * Returns resistance levels.
     *
     * @return resistance-level array
     */
    public SupportResistanceLevel[] resistanceLevels() {
        return nativeGetResistanceLevels(nativePtr);
    }

    /**
     * Returns levels with at least a minimum touch count.
     *
     * @param minTouches minimum touch count
     * @return filtered level array
     */
    public SupportResistanceLevel[] significantLevels(int minTouches) {
        return nativeGetSignificantLevels(nativePtr, minTouches);
    }

    /**
     * Checks whether a price is near support.
     *
     * @param price price to test
     * @param tolerance proximity tolerance
     * @return {@code true} if near support
     */
    public boolean isNearSupport(double price, double tolerance) {
        return nativeIsNearSupport(nativePtr, price, tolerance);
    }

    /**
     * Checks whether a price is near resistance.
     *
     * @param price price to test
     * @param tolerance proximity tolerance
     * @return {@code true} if near resistance
     */
    public boolean isNearResistance(double price, double tolerance) {
        return nativeIsNearResistance(nativePtr, price, tolerance);
    }

    /**
     * Returns support prices.
     *
     * @return support-price array
     */
    public double[] supportPrices() {
        return nativeGetSupportPrices(nativePtr);
    }

    /**
     * Returns resistance prices.
     *
     * @return resistance-price array
     */
    public double[] resistancePrices() {
        return nativeGetResistancePrices(nativePtr);
    }

    /**
     * Returns all price objectives.
     *
     * @return price-objective array
     */
    public PriceObjective[] priceObjectives() {
        return nativeGetPriceObjectives(nativePtr);
    }

    /**
     * Returns bullish price objectives.
     *
     * @return bullish-objective array
     */
    public PriceObjective[] bullishObjectives() {
        return nativeGetBullishObjectives(nativePtr);
    }

    /**
     * Returns bearish price objectives.
     *
     * @return bearish-objective array
     */
    public PriceObjective[] bearishObjectives() {
        return nativeGetBearishObjectives(nativePtr);
    }

    /**
     * Returns bullish target prices.
     *
     * @return bullish-target array
     */
    public double[] bullishTargets() {
        return nativeGetBullishTargets(nativePtr);
    }

    /**
     * Returns bearish target prices.
     *
     * @return bearish-target array
     */
    public double[] bearishTargets() {
        return nativeGetBearishTargets(nativePtr);
    }

    /**
     * Returns detected congestion zones.
     *
     * @return congestion-zone array
     */
    public CongestionZone[] congestionZones() {
        return nativeGetCongestionZones(nativePtr);
    }

    /**
     * Checks whether a column lies in any congestion zone.
     *
     * @param column zero-based column index
     * @return {@code true} if column is in congestion
     */
    public boolean isInCongestion(int column) {
        return nativeIsInCongestion(nativePtr, column);
    }

    /**
     * Returns the largest congestion zone.
     *
     * @return largest congestion zone, or implementation-defined empty value
     */
    public CongestionZone largestCongestionZone() {
        return nativeGetLargestCongestionZone(nativePtr);
    }

    /**
     * Returns a text summary of computed indicators.
     *
     * @return summary text
     */
    public String summary() {
        return nativeSummary(nativePtr);
    }

    /**
     * Returns a native-generated text representation.
     *
     * @return indicator text
     */
    @Override
    public String toString() {
        return nativeToString(nativePtr);
    }

    /**
     * Releases native resources.
     */
    @Override
    public void close() {
        if (nativePtr != 0) {
            nativeDestroy(nativePtr);
            nativePtr = 0;
        }
    }

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

    private static native double nativeSmaShort(long ptr, int column);
    private static native double nativeSmaMiddle(long ptr, int column);
    private static native double nativeSmaLong(long ptr, int column);
    private static native double[] nativeSmaShortValues(long ptr);
    private static native double[] nativeSmaMediumValues(long ptr);
    private static native double[] nativeSmaLongValues(long ptr);

    private static native double nativeBollingerMiddle(long ptr, int column);
    private static native double nativeBollingerUpper(long ptr, int column);
    private static native double nativeBollingerLower(long ptr, int column);
    private static native boolean nativeHasBollingerValue(long ptr, int column);
    private static native boolean nativeIsAboveUpperBand(long ptr, int column, double price);
    private static native boolean nativeIsBelowLowerBand(long ptr, int column, double price);
    private static native double[] nativeBollingerMiddleValues(long ptr);
    private static native double[] nativeBollingerUpperValues(long ptr);
    private static native double[] nativeBollingerLowerValues(long ptr);

    private static native double nativeRsi(long ptr, int column);
    private static native boolean nativeHasRsiValue(long ptr, int column);
    private static native boolean nativeIsOverbought(long ptr, int column);
    private static native boolean nativeIsOversold(long ptr, int column);
    private static native double[] nativeRsiValues(long ptr);

    private static native double nativeObv(long ptr, int column);
    private static native double[] nativeObvValues(long ptr);

    private static native double nativeBullishPercent(long ptr);
    private static native boolean nativeIsBullishAlert(long ptr);
    private static native boolean nativeIsBearishAlert(long ptr);

    private static native int nativeCurrentSignal(long ptr);
    private static native boolean nativeHasBuySignal(long ptr);
    private static native boolean nativeHasSellSignal(long ptr);
    private static native int nativeSignalCount(long ptr);
    private static native int nativeBuySignalCount(long ptr);
    private static native int nativeSellSignalCount(long ptr);
    private static native Signal[] nativeGetSignals(long ptr);
    private static native Signal[] nativeGetBuySignals(long ptr);
    private static native Signal[] nativeGetSellSignals(long ptr);

    private static native int nativePatternCount(long ptr);
    private static native int nativeBullishPatternCount(long ptr);
    private static native int nativeBearishPatternCount(long ptr);
    private static native boolean nativeHasPattern(long ptr, int patternType);
    private static native Pattern[] nativeGetPatterns(long ptr);
    private static native Pattern[] nativeGetBullishPatterns(long ptr);
    private static native Pattern[] nativeGetBearishPatterns(long ptr);

    private static native SupportResistanceLevel[] nativeGetLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetSupportLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetResistanceLevels(long ptr);
    private static native SupportResistanceLevel[] nativeGetSignificantLevels(long ptr, int minTouches);
    private static native boolean nativeIsNearSupport(long ptr, double price, double tolerance);
    private static native boolean nativeIsNearResistance(long ptr, double price, double tolerance);
    private static native double[] nativeGetSupportPrices(long ptr);
    private static native double[] nativeGetResistancePrices(long ptr);

    private static native PriceObjective[] nativeGetPriceObjectives(long ptr);
    private static native PriceObjective[] nativeGetBullishObjectives(long ptr);
    private static native PriceObjective[] nativeGetBearishObjectives(long ptr);
    private static native double[] nativeGetBullishTargets(long ptr);
    private static native double[] nativeGetBearishTargets(long ptr);

    private static native CongestionZone[] nativeGetCongestionZones(long ptr);
    private static native boolean nativeIsInCongestion(long ptr, int column);
    private static native CongestionZone nativeGetLargestCongestionZone(long ptr);

    private static native String nativeSummary(long ptr);
    private static native String nativeToString(long ptr);
}
