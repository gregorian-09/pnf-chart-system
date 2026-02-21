/**
 */
package com.pnf;

/**
 * Represents a buy or sell signal detected in a P&F chart.
 */
/**
 */
/**
 * Signal API.
 * @param type 
 * @param columnIndex 
 * @param price 
 * @return 
 */
public record Signal(SignalType type, int columnIndex, double price) {

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return String.format("Signal{type=%s, column=%d, price=%.2f}", type, columnIndex, price);
    }
}
