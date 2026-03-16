package com.pnf;

/**
 * Configuration options used when constructing a {@link Chart}.
 */
public class ChartConfig {
    /** Input method used to build columns. */
    public ConstructionMethod method = ConstructionMethod.CLOSE;

    /** Box sizing method. */
    public BoxSizeMethod boxSizeMethod = BoxSizeMethod.TRADITIONAL;

    /** Box size value for methods that require an explicit number. */
    public double boxSize = 0.0;

    /** Reversal amount in boxes. */
    public int reversal = 3;

    /**
     * Creates a default configuration.
     */
    public ChartConfig() {}

    /**
     * Creates a configuration with explicit values.
     *
     * @param method input method
     * @param boxSizeMethod box-size method
     * @param boxSize configured box size
     * @param reversal reversal amount in boxes
     */
    public ChartConfig(ConstructionMethod method, BoxSizeMethod boxSizeMethod,
                       double boxSize, int reversal) {
        this.method = method;
        this.boxSizeMethod = boxSizeMethod;
        this.boxSize = boxSize;
        this.reversal = reversal;
    }

    /**
     * Returns a default chart configuration.
     *
     * @return default configuration
     */
    public static ChartConfig getDefault() {
        return new ChartConfig();
    }

    /**
     * Creates a builder for fluent configuration.
     *
     * @return new builder
     */
    public static Builder builder() {
        return new Builder();
    }

    /**
     * Fluent builder for {@link ChartConfig}.
     */
    public static class Builder {
        private ConstructionMethod method = ConstructionMethod.CLOSE;
        private BoxSizeMethod boxSizeMethod = BoxSizeMethod.TRADITIONAL;
        private double boxSize = 0.0;
        private int reversal = 3;

        /**
         * Creates a builder with default chart settings.
         */
        public Builder() {}

        /**
         * Sets construction method.
         *
         * @param method input method
         * @return this builder
         */
        public Builder method(ConstructionMethod method) {
            this.method = method;
            return this;
        }

        /**
         * Sets box-size method.
         *
         * @param boxSizeMethod box-size method
         * @return this builder
         */
        public Builder boxSizeMethod(BoxSizeMethod boxSizeMethod) {
            this.boxSizeMethod = boxSizeMethod;
            return this;
        }

        /**
         * Sets box size.
         *
         * @param boxSize box size value
         * @return this builder
         */
        public Builder boxSize(double boxSize) {
            this.boxSize = boxSize;
            return this;
        }

        /**
         * Sets reversal amount.
         *
         * @param reversal reversal in boxes
         * @return this builder
         */
        public Builder reversal(int reversal) {
            this.reversal = reversal;
            return this;
        }

        /**
         * Builds the final configuration.
         *
         * @return chart configuration
         */
        public ChartConfig build() {
            return new ChartConfig(method, boxSizeMethod, boxSize, reversal);
        }
    }
}
