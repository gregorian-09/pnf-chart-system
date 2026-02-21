/**
 */
package com.pnf;

/**
 * Represents a support or resistance level identified in a P&F chart.
 */
/**
 */
/**
 * SupportResistanceLevel API.
 * @param type 
 * @param price 
 * @param touches 
 * @param firstColumn 
 * @param lastColumn 
 * @return 
 */
public record SupportResistanceLevel(LevelType type, double price, int touches, int firstColumn, int lastColumn) {

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return String.format("SupportResistanceLevel{type=%s, price=%.2f, touches=%d, columns=%d-%d}",
                type, price, touches, firstColumn, lastColumn);
    }
}
