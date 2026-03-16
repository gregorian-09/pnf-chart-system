package com.pnf;

/**
 * Support or resistance level detected from chart structure.
 *
 * @param type support or resistance
 * @param price level price
 * @param touches number of touches
 * @param firstColumn first column index contributing to the level
 * @param lastColumn last column index contributing to the level
 */
public record SupportResistanceLevel(LevelType type, double price, int touches, int firstColumn, int lastColumn) {

    /**
     * Returns a concise textual representation.
     *
     * @return formatted level text
     */
    @Override
    public String toString() {
        return String.format("SupportResistanceLevel{type=%s, price=%.2f, touches=%d, columns=%d-%d}",
                type, price, touches, firstColumn, lastColumn);
    }
}
