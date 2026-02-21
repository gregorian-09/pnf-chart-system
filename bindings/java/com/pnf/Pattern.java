/**
 */
package com.pnf;

/**
 * Represents a chart pattern detected in a P&F chart.
 */
/**
 */
/**
 * Pattern API.
 * @param type 
 * @param columnIndex 
 * @param price 
 * @param isBullish 
 * @return 
 */
public record Pattern(PatternType type, int columnIndex, double price, boolean isBullish) {

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return String.format("Pattern{type=%s, column=%d, price=%.2f, bullish=%b}",
                type, columnIndex, price, isBullish);
    }
}
