package com.pnf;

/**
 * Price target projected from Point and Figure counting rules.
 *
 * @param target projected target price
 * @param columnIndex originating column index
 * @param isBullish whether the objective is bullish
 */
public record PriceObjective(double target, int columnIndex, boolean isBullish) {

    /**
     * Returns a concise textual representation.
     *
     * @return formatted objective text
     */
    @Override
    public String toString() {
        return String.format("PriceObjective{target=%.2f, column=%d, bullish=%b}",
                target, columnIndex, isBullish);
    }
}
