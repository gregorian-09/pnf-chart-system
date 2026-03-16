package com.pnf;

/**
 * Sideways congestion zone identified on the chart.
 *
 * @param startColumn start column index
 * @param endColumn end column index
 * @param highPrice highest price in the zone
 * @param lowPrice lowest price in the zone
 * @param columnCount number of columns in the zone
 */
public record CongestionZone(int startColumn, int endColumn, double highPrice, double lowPrice, int columnCount) {

    /**
     * Returns a concise textual representation.
     *
     * @return formatted congestion-zone text
     */
    @Override
    public String toString() {
        return String.format("CongestionZone{columns=%d-%d, range=%.2f-%.2f, count=%d}",
                startColumn, endColumn, lowPrice, highPrice, columnCount);
    }
}
