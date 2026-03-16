package com.pnf;

/**
 * Signal emitted by the indicator engine.
 *
 * @param type signal type
 * @param columnIndex chart column index where the signal occurred
 * @param price price level associated with the signal
 */
public record Signal(SignalType type, int columnIndex, double price) {

    /**
     * Returns a concise textual representation.
     *
     * @return formatted signal text
     */
    @Override
    public String toString() {
        return String.format("Signal{type=%s, column=%d, price=%.2f}", type, columnIndex, price);
    }
}
