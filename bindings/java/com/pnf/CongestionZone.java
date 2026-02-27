/**
 */
package com.pnf;

/**
 * Represents a congestion zone (sideways price movement) in a P&F chart.
 */
/**
 */
/**
 * CongestionZone API.
 * @param startColumn 
 * @param endColumn 
 * @param highPrice 
 * @param lowPrice 
 * @param columnCount 
 * @return 
 */
public record CongestionZone(int startColumn, int endColumn, double highPrice, double lowPrice, int columnCount) {

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return String.format("CongestionZone{columns=%d-%d, range=%.2f-%.2f, count=%d}",
                startColumn, endColumn, lowPrice, highPrice, columnCount);
    }
}
