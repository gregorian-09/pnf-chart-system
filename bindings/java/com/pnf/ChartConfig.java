/**
 */
package com.pnf;

/**
 * ChartConfig binding type.
 */
public class ChartConfig {
    public ConstructionMethod method = ConstructionMethod.CLOSE;
    public BoxSizeMethod boxSizeMethod = BoxSizeMethod.TRADITIONAL;
    public double boxSize = 0.0;
    public int reversal = 3;

    /**
     */
    /**
     * g API.
     * @return 
     */
    public ChartConfig() {}

    /**
     */
    public ChartConfig(ConstructionMethod method, BoxSizeMethod boxSizeMethod,
                       double boxSize, int reversal) {
        this.method = method;
        this.boxSizeMethod = boxSizeMethod;
        this.boxSize = boxSize;
        this.reversal = reversal;
    }

    public static ChartConfig getDefault() {
        return new ChartConfig();
    }

    public static Builder builder() {
        return new Builder();
    }

    public static class Builder {
        private ConstructionMethod method = ConstructionMethod.CLOSE;
        private BoxSizeMethod boxSizeMethod = BoxSizeMethod.TRADITIONAL;
        private double boxSize = 0.0;
        private int reversal = 3;

        /**
         */
        /**
         * method API.
         * @param method 
         * @return 
         */
        public Builder method(ConstructionMethod method) {
            this.method = method;
            return this;
        }

        /**
         */
        /**
         * boxSizeMethod API.
         * @param boxSizeMethod 
         * @return 
         */
        public Builder boxSizeMethod(BoxSizeMethod boxSizeMethod) {
            this.boxSizeMethod = boxSizeMethod;
            return this;
        }

        /**
         */
        /**
         * boxSize API.
         * @param boxSize 
         * @return 
         */
        public Builder boxSize(double boxSize) {
            this.boxSize = boxSize;
            return this;
        }

        /**
         */
        /**
         * reversal API.
         * @param reversal 
         * @return 
         */
        public Builder reversal(int reversal) {
            this.reversal = reversal;
            return this;
        }

        /**
         */
        /**
         * build API.
         * @return 
         */
        public ChartConfig build() {
            return new ChartConfig(method, boxSizeMethod, boxSize, reversal);
        }
    }
}
