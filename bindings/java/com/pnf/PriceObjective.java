/**
 */
package com.pnf;

/**
 * Represents a price objective calculated using P&F vertical count method.
 */
/**
 */
/**
 * PriceObjective API.
 * @param target 
 * @param columnIndex 
 * @param isBullish 
 * @return 
 */
public record PriceObjective(double target, int columnIndex, boolean isBullish) {

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return String.format("PriceObjective{target=%.2f, column=%d, bullish=%b}",
                target, columnIndex, isBullish);
    }
}
