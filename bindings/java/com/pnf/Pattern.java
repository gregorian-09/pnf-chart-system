package com.pnf;

/**
 * Pattern detected on the chart.
 *
 * @param type pattern type
 * @param columnIndex chart column index where the pattern is anchored
 * @param price price associated with the pattern
 * @param isBullish whether the pattern is bullish
 */
public record Pattern(PatternType type, int columnIndex, double price, boolean isBullish) {

    /**
     * Returns a concise textual representation.
     *
     * @return formatted pattern text
     */
    @Override
    public String toString() {
        return String.format("Pattern{type=%s, column=%d, price=%.2f, bullish=%b}",
                type, columnIndex, price, isBullish);
    }
}
