/**
 */
package com.pnf;

/**
 * Configuration parameters for technical indicators.
 * Allows customization of moving average periods, Bollinger Band parameters,
 * RSI thresholds, and other indicator settings.
 */
/**
 * IndicatorConfig binding type.
 */
public class IndicatorConfig {
    /** Period for short-term simple moving average (default: 5) */
    public int smaShortPeriod = 5;

    /** Period for medium-term simple moving average (default: 10) */
    public int smaMediumPeriod = 10;

    /** Period for long-term simple moving average (default: 20) */
    public int smaLongPeriod = 20;

    /** Period for Bollinger Bands calculation (default: 20) */
    public int bollingerPeriod = 20;

    /** Number of standard deviations for Bollinger Bands (default: 2.0) */
    public double bollingerStdDevs = 2.0;

    /** Period for RSI calculation (default: 14) */
    public int rsiPeriod = 14;

    /** RSI level indicating overbought condition (default: 70.0) */
    public double rsiOverbought = 70.0;

    /** RSI level indicating oversold condition (default: 30.0) */
    public double rsiOversold = 30.0;

    /** Bullish percent threshold for bullish alert (default: 70.0) */
    public double bullishAlertThreshold = 70.0;

    /** Bullish percent threshold for bearish alert (default: 30.0) */
    public double bearishAlertThreshold = 30.0;

    /** Threshold for merging similar support/resistance levels (default: 0.01) */
    public double supportResistanceThreshold = 0.01;

    /** Minimum columns required to identify congestion zone (default: 4) */
    public int congestionMinColumns = 4;

    /** Price range percentage for congestion detection (default: 0.05) */
    public double congestionPriceRange = 0.05;

    /**
     */
    /**
     * g API.
     * @return 
     */
    public IndicatorConfig() {}

    /**
     */
    /**
     * g API.
     * @param smaShortPeriod 
     * @param smaMediumPeriod 
     * @param smaLongPeriod 
     * @return 
     */
    public IndicatorConfig(int smaShortPeriod, int smaMediumPeriod, int smaLongPeriod) {
        this.smaShortPeriod = smaShortPeriod;
        this.smaMediumPeriod = smaMediumPeriod;
        this.smaLongPeriod = smaLongPeriod;
    }

    /** Creates a default configuration with standard parameters */
    public static IndicatorConfig defaults() {
        return new IndicatorConfig();
    }

    /** Creates a default configuration with standard parameters */
    public static IndicatorConfig getDefault() {
        return new IndicatorConfig();
    }

    /** Returns a builder for fluent configuration */
    public static Builder builder() {
        return new Builder();
    }

    public static class Builder {
        private final IndicatorConfig config = new IndicatorConfig();

        /**
         */
        /**
         * smaShortPeriod API.
         * @param period 
         * @return 
         */
        public Builder smaShortPeriod(int period) {
            config.smaShortPeriod = period;
            return this;
        }

        /**
         */
        /**
         * smaMediumPeriod API.
         * @param period 
         * @return 
         */
        public Builder smaMediumPeriod(int period) {
            config.smaMediumPeriod = period;
            return this;
        }

        /**
         */
        /**
         * smaLongPeriod API.
         * @param period 
         * @return 
         */
        public Builder smaLongPeriod(int period) {
            config.smaLongPeriod = period;
            return this;
        }

        /**
         */
        /**
         * bollingerPeriod API.
         * @param period 
         * @return 
         */
        public Builder bollingerPeriod(int period) {
            config.bollingerPeriod = period;
            return this;
        }

        /**
         */
        /**
         * bollingerStdDevs API.
         * @param stdDevs 
         * @return 
         */
        public Builder bollingerStdDevs(double stdDevs) {
            config.bollingerStdDevs = stdDevs;
            return this;
        }

        /**
         */
        /**
         * rsiPeriod API.
         * @param period 
         * @return 
         */
        public Builder rsiPeriod(int period) {
            config.rsiPeriod = period;
            return this;
        }

        /**
         */
        /**
         * rsiOverbought API.
         * @param threshold 
         * @return 
         */
        public Builder rsiOverbought(double threshold) {
            config.rsiOverbought = threshold;
            return this;
        }

        /**
         */
        /**
         * rsiOversold API.
         * @param threshold 
         * @return 
         */
        public Builder rsiOversold(double threshold) {
            config.rsiOversold = threshold;
            return this;
        }

        /**
         */
        /**
         * bullishAlertThreshold API.
         * @param threshold 
         * @return 
         */
        public Builder bullishAlertThreshold(double threshold) {
            config.bullishAlertThreshold = threshold;
            return this;
        }

        /**
         */
        /**
         * bearishAlertThreshold API.
         * @param threshold 
         * @return 
         */
        public Builder bearishAlertThreshold(double threshold) {
            config.bearishAlertThreshold = threshold;
            return this;
        }

        /**
         */
        /**
         * supportResistanceThreshold API.
         * @param threshold 
         * @return 
         */
        public Builder supportResistanceThreshold(double threshold) {
            config.supportResistanceThreshold = threshold;
            return this;
        }

        /**
         */
        /**
         * congestionMinColumns API.
         * @param columns 
         * @return 
         */
        public Builder congestionMinColumns(int columns) {
            config.congestionMinColumns = columns;
            return this;
        }

        /**
         */
        /**
         * congestionPriceRange API.
         * @param range 
         * @return 
         */
        public Builder congestionPriceRange(double range) {
            config.congestionPriceRange = range;
            return this;
        }

        /**
         */
        /**
         * build API.
         * @return 
         */
        public IndicatorConfig build() {
            return config;
        }
    }

    /** Builder pattern for fluent configuration */
    /**
     * withSmaShortPeriod API.
     * @param period 
     * @return 
     */
    public IndicatorConfig withSmaShortPeriod(int period) {
        this.smaShortPeriod = period;
        return this;
    }

    /**
     */
    /**
     * withSmaMediumPeriod API.
     * @param period 
     * @return 
     */
    public IndicatorConfig withSmaMediumPeriod(int period) {
        this.smaMediumPeriod = period;
        return this;
    }

    /**
     */
    /**
     * withSmaLongPeriod API.
     * @param period 
     * @return 
     */
    public IndicatorConfig withSmaLongPeriod(int period) {
        this.smaLongPeriod = period;
        return this;
    }

    /**
     */
    /**
     * withBollingerPeriod API.
     * @param period 
     * @return 
     */
    public IndicatorConfig withBollingerPeriod(int period) {
        this.bollingerPeriod = period;
        return this;
    }

    /**
     */
    /**
     * withBollingerStdDevs API.
     * @param stdDevs 
     * @return 
     */
    public IndicatorConfig withBollingerStdDevs(double stdDevs) {
        this.bollingerStdDevs = stdDevs;
        return this;
    }

    /**
     */
    /**
     * withRsiPeriod API.
     * @param period 
     * @return 
     */
    public IndicatorConfig withRsiPeriod(int period) {
        this.rsiPeriod = period;
        return this;
    }

    /**
     */
    /**
     * withRsiOverbought API.
     * @param threshold 
     * @return 
     */
    public IndicatorConfig withRsiOverbought(double threshold) {
        this.rsiOverbought = threshold;
        return this;
    }

    /**
     */
    /**
     * withRsiOversold API.
     * @param threshold 
     * @return 
     */
    public IndicatorConfig withRsiOversold(double threshold) {
        this.rsiOversold = threshold;
        return this;
    }

    /**
     */
    /**
     * withBullishAlertThreshold API.
     * @param threshold 
     * @return 
     */
    public IndicatorConfig withBullishAlertThreshold(double threshold) {
        this.bullishAlertThreshold = threshold;
        return this;
    }

    /**
     */
    /**
     * withBearishAlertThreshold API.
     * @param threshold 
     * @return 
     */
    public IndicatorConfig withBearishAlertThreshold(double threshold) {
        this.bearishAlertThreshold = threshold;
        return this;
    }

    /**
     */
    /**
     * withSupportResistanceThreshold API.
     * @param threshold 
     * @return 
     */
    public IndicatorConfig withSupportResistanceThreshold(double threshold) {
        this.supportResistanceThreshold = threshold;
        return this;
    }

    /**
     */
    /**
     * withCongestionMinColumns API.
     * @param columns 
     * @return 
     */
    public IndicatorConfig withCongestionMinColumns(int columns) {
        this.congestionMinColumns = columns;
        return this;
    }

    /**
     */
    /**
     * withCongestionPriceRange API.
     * @param range 
     * @return 
     */
    public IndicatorConfig withCongestionPriceRange(double range) {
        this.congestionPriceRange = range;
        return this;
    }
}
