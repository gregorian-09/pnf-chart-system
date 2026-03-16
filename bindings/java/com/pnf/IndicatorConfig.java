package com.pnf;

/**
 * Configuration parameters for {@link Indicators} calculations.
 */
public class IndicatorConfig {
    /** Period for short-term simple moving average (default: 5). */
    public int smaShortPeriod = 5;

    /** Period for medium-term simple moving average (default: 10). */
    public int smaMediumPeriod = 10;

    /** Period for long-term simple moving average (default: 20). */
    public int smaLongPeriod = 20;

    /** Period for Bollinger Bands calculation (default: 20). */
    public int bollingerPeriod = 20;

    /** Number of standard deviations for Bollinger Bands (default: 2.0). */
    public double bollingerStdDevs = 2.0;

    /** Period for RSI calculation (default: 14). */
    public int rsiPeriod = 14;

    /** RSI level indicating overbought condition (default: 70.0). */
    public double rsiOverbought = 70.0;

    /** RSI level indicating oversold condition (default: 30.0). */
    public double rsiOversold = 30.0;

    /** Bullish percent threshold for bullish alert (default: 70.0). */
    public double bullishAlertThreshold = 70.0;

    /** Bullish percent threshold for bearish alert (default: 30.0). */
    public double bearishAlertThreshold = 30.0;

    /** Threshold for merging similar support/resistance levels (default: 0.01). */
    public double supportResistanceThreshold = 0.01;

    /** Minimum columns required to identify congestion zone (default: 4). */
    public int congestionMinColumns = 4;

    /** Price range percentage for congestion detection (default: 0.05). */
    public double congestionPriceRange = 0.05;

    /**
     * Creates default indicator configuration.
     */
    public IndicatorConfig() {}

    /**
     * Creates a configuration with custom SMA periods.
     *
     * @param smaShortPeriod short SMA period
     * @param smaMediumPeriod medium SMA period
     * @param smaLongPeriod long SMA period
     */
    public IndicatorConfig(int smaShortPeriod, int smaMediumPeriod, int smaLongPeriod) {
        this.smaShortPeriod = smaShortPeriod;
        this.smaMediumPeriod = smaMediumPeriod;
        this.smaLongPeriod = smaLongPeriod;
    }

    /**
     * Creates a default configuration.
     *
     * @return default indicator configuration
     */
    public static IndicatorConfig defaults() {
        return new IndicatorConfig();
    }

    /**
     * Creates a default configuration.
     *
     * @return default indicator configuration
     */
    public static IndicatorConfig getDefault() {
        return new IndicatorConfig();
    }

    /**
     * Returns a builder for fluent configuration.
     *
     * @return new builder instance
     */
    public static Builder builder() {
        return new Builder();
    }

    /**
     * Fluent builder for {@link IndicatorConfig}.
     */
    public static class Builder {
        private final IndicatorConfig config = new IndicatorConfig();

        /**
         * Creates a builder with default indicator settings.
         */
        public Builder() {}

        /**
         * Sets short SMA period.
         *
         * @param period short SMA period
         * @return this builder
         */
        public Builder smaShortPeriod(int period) {
            config.smaShortPeriod = period;
            return this;
        }

        /**
         * Sets medium SMA period.
         *
         * @param period medium SMA period
         * @return this builder
         */
        public Builder smaMediumPeriod(int period) {
            config.smaMediumPeriod = period;
            return this;
        }

        /**
         * Sets long SMA period.
         *
         * @param period long SMA period
         * @return this builder
         */
        public Builder smaLongPeriod(int period) {
            config.smaLongPeriod = period;
            return this;
        }

        /**
         * Sets Bollinger period.
         *
         * @param period Bollinger lookback period
         * @return this builder
         */
        public Builder bollingerPeriod(int period) {
            config.bollingerPeriod = period;
            return this;
        }

        /**
         * Sets Bollinger standard-deviation multiplier.
         *
         * @param stdDevs standard-deviation multiplier
         * @return this builder
         */
        public Builder bollingerStdDevs(double stdDevs) {
            config.bollingerStdDevs = stdDevs;
            return this;
        }

        /**
         * Sets RSI period.
         *
         * @param period RSI period
         * @return this builder
         */
        public Builder rsiPeriod(int period) {
            config.rsiPeriod = period;
            return this;
        }

        /**
         * Sets RSI overbought threshold.
         *
         * @param threshold overbought threshold
         * @return this builder
         */
        public Builder rsiOverbought(double threshold) {
            config.rsiOverbought = threshold;
            return this;
        }

        /**
         * Sets RSI oversold threshold.
         *
         * @param threshold oversold threshold
         * @return this builder
         */
        public Builder rsiOversold(double threshold) {
            config.rsiOversold = threshold;
            return this;
        }

        /**
         * Sets bullish alert threshold.
         *
         * @param threshold bullish alert threshold
         * @return this builder
         */
        public Builder bullishAlertThreshold(double threshold) {
            config.bullishAlertThreshold = threshold;
            return this;
        }

        /**
         * Sets bearish alert threshold.
         *
         * @param threshold bearish alert threshold
         * @return this builder
         */
        public Builder bearishAlertThreshold(double threshold) {
            config.bearishAlertThreshold = threshold;
            return this;
        }

        /**
         * Sets support/resistance merge threshold.
         *
         * @param threshold merge threshold
         * @return this builder
         */
        public Builder supportResistanceThreshold(double threshold) {
            config.supportResistanceThreshold = threshold;
            return this;
        }

        /**
         * Sets minimum congestion columns.
         *
         * @param columns minimum congestion columns
         * @return this builder
         */
        public Builder congestionMinColumns(int columns) {
            config.congestionMinColumns = columns;
            return this;
        }

        /**
         * Sets congestion range threshold.
         *
         * @param range congestion range threshold
         * @return this builder
         */
        public Builder congestionPriceRange(double range) {
            config.congestionPriceRange = range;
            return this;
        }

        /**
         * Builds the configured object.
         *
         * @return configured indicator config
         */
        public IndicatorConfig build() {
            return config;
        }
    }

    /**
     * Sets short SMA period.
     *
     * @param period short SMA period
     * @return this config
     */
    public IndicatorConfig withSmaShortPeriod(int period) {
        this.smaShortPeriod = period;
        return this;
    }

    /**
     * Sets medium SMA period.
     *
     * @param period medium SMA period
     * @return this config
     */
    public IndicatorConfig withSmaMediumPeriod(int period) {
        this.smaMediumPeriod = period;
        return this;
    }

    /**
     * Sets long SMA period.
     *
     * @param period long SMA period
     * @return this config
     */
    public IndicatorConfig withSmaLongPeriod(int period) {
        this.smaLongPeriod = period;
        return this;
    }

    /**
     * Sets Bollinger period.
     *
     * @param period Bollinger lookback period
     * @return this config
     */
    public IndicatorConfig withBollingerPeriod(int period) {
        this.bollingerPeriod = period;
        return this;
    }

    /**
     * Sets Bollinger standard-deviation multiplier.
     *
     * @param stdDevs standard-deviation multiplier
     * @return this config
     */
    public IndicatorConfig withBollingerStdDevs(double stdDevs) {
        this.bollingerStdDevs = stdDevs;
        return this;
    }

    /**
     * Sets RSI period.
     *
     * @param period RSI period
     * @return this config
     */
    public IndicatorConfig withRsiPeriod(int period) {
        this.rsiPeriod = period;
        return this;
    }

    /**
     * Sets RSI overbought threshold.
     *
     * @param threshold overbought threshold
     * @return this config
     */
    public IndicatorConfig withRsiOverbought(double threshold) {
        this.rsiOverbought = threshold;
        return this;
    }

    /**
     * Sets RSI oversold threshold.
     *
     * @param threshold oversold threshold
     * @return this config
     */
    public IndicatorConfig withRsiOversold(double threshold) {
        this.rsiOversold = threshold;
        return this;
    }

    /**
     * Sets bullish alert threshold.
     *
     * @param threshold bullish alert threshold
     * @return this config
     */
    public IndicatorConfig withBullishAlertThreshold(double threshold) {
        this.bullishAlertThreshold = threshold;
        return this;
    }

    /**
     * Sets bearish alert threshold.
     *
     * @param threshold bearish alert threshold
     * @return this config
     */
    public IndicatorConfig withBearishAlertThreshold(double threshold) {
        this.bearishAlertThreshold = threshold;
        return this;
    }

    /**
     * Sets support/resistance merge threshold.
     *
     * @param threshold merge threshold
     * @return this config
     */
    public IndicatorConfig withSupportResistanceThreshold(double threshold) {
        this.supportResistanceThreshold = threshold;
        return this;
    }

    /**
     * Sets minimum congestion columns.
     *
     * @param columns minimum congestion columns
     * @return this config
     */
    public IndicatorConfig withCongestionMinColumns(int columns) {
        this.congestionMinColumns = columns;
        return this;
    }

    /**
     * Sets congestion range threshold.
     *
     * @param range congestion range threshold
     * @return this config
     */
    public IndicatorConfig withCongestionPriceRange(double range) {
        this.congestionPriceRange = range;
        return this;
    }
}
