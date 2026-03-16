package com.pnf;

/**
 * Java binding for the native Point and Figure chart engine.
 */
public class Chart implements AutoCloseable {
    static {
        System.loadLibrary("pnfjni");
    }

    private long nativePtr;

    /**
     * Creates a chart using default configuration.
     */
    public Chart() {
        nativePtr = nativeCreate();
    }

    /**
     * Creates a chart with custom configuration.
     *
     * @param config chart configuration
     */
    public Chart(ChartConfig config) {
        nativePtr = nativeCreateWithConfig(
                config.method.ordinal(),
                config.boxSizeMethod.ordinal(),
                config.boxSize,
                config.reversal
        );
    }

    /**
     * Adds OHLC input to the chart.
     *
     * @param high period high
     * @param low period low
     * @param close period close
     * @param timestamp timestamp associated with the input
     * @return {@code true} if chart state changed
     */
    public boolean addData(double high, double low, double close, long timestamp) {
        return nativeAddData(nativePtr, high, low, close, timestamp);
    }

    /**
     * Adds a single price point to the chart.
     *
     * @param price price value
     * @param timestamp timestamp associated with the input
     * @return {@code true} if chart state changed
     */
    public boolean addPrice(double price, long timestamp) {
        return nativeAddPrice(nativePtr, price, timestamp);
    }

    /**
     * Returns the number of columns.
     *
     * @return column count
     */
    public int columnCount() {
        return nativeColumnCount(nativePtr);
    }

    /**
     * Returns the number of X columns.
     *
     * @return X-column count
     */
    public int xColumnCount() {
        return nativeXColumnCount(nativePtr);
    }

    /**
     * Returns the number of O columns.
     *
     * @return O-column count
     */
    public int oColumnCount() {
        return nativeOColumnCount(nativePtr);
    }

    /**
     * Returns the effective box size used by the chart.
     *
     * @return box size
     */
    public double boxSize() {
        return nativeBoxSize(nativePtr);
    }

    /**
     * Returns the type of the specified column.
     *
     * @param index zero-based column index
     * @return column type
     */
    public ColumnType columnType(int index) {
        return ColumnType.values()[nativeColumnType(nativePtr, index)];
    }

    /**
     * Returns number of boxes in the specified column.
     *
     * @param index zero-based column index
     * @return box count in column
     */
    public int columnBoxCount(int index) {
        return nativeColumnBoxCount(nativePtr, index);
    }

    /**
     * Returns highest price in the specified column.
     *
     * @param index zero-based column index
     * @return highest price
     */
    public double columnHighest(int index) {
        return nativeColumnHighest(nativePtr, index);
    }

    /**
     * Returns lowest price in the specified column.
     *
     * @param index zero-based column index
     * @return lowest price
     */
    public double columnLowest(int index) {
        return nativeColumnLowest(nativePtr, index);
    }

    /**
     * Returns the price of the specified box.
     *
     * @param columnIndex zero-based column index
     * @param boxIndex zero-based box index within the column
     * @return box price
     */
    public double boxPrice(int columnIndex, int boxIndex) {
        return nativeBoxPrice(nativePtr, columnIndex, boxIndex);
    }

    /**
     * Returns the type of the specified box.
     *
     * @param columnIndex zero-based column index
     * @param boxIndex zero-based box index within the column
     * @return box type
     */
    public BoxType boxType(int columnIndex, int boxIndex) {
        return BoxType.values()[nativeBoxType(nativePtr, columnIndex, boxIndex)];
    }

    /**
     * Returns the marker text stored on the specified box.
     *
     * @param columnIndex zero-based column index
     * @param boxIndex zero-based box index within the column
     * @return box marker, or an empty string when no marker is set
     */
    public String boxMarker(int columnIndex, int boxIndex) {
        return nativeBoxMarker(nativePtr, columnIndex, boxIndex);
    }

    /**
     * Indicates whether chart bias is bullish.
     *
     * @return {@code true} when bullish bias is present
     */
    public boolean hasBullishBias() {
        return nativeHasBullishBias(nativePtr);
    }

    /**
     * Indicates whether chart bias is bearish.
     *
     * @return {@code true} when bearish bias is present
     */
    public boolean hasBearishBias() {
        return nativeHasBearishBias(nativePtr);
    }

    /**
     * Clears chart state.
     */
    public void clear() {
        nativeClear(nativePtr);
    }

    long getNativePtr() {
        return nativePtr;
    }

    /**
     * Returns a native-generated text snapshot of the chart.
     *
     * @return chart text
     */
    @Override
    public String toString() {
        return nativeToString(nativePtr);
    }

    /**
     * Returns an ASCII rendering of the chart.
     *
     * @return chart ASCII rendering
     */
    public String toAscii() {
        return nativeToAscii(nativePtr);
    }

    /**
     * Releases native resources.
     */
    @Override
    public void close() {
        if (nativePtr != 0) {
            nativeDestroy(nativePtr);
            nativePtr = 0;
        }
    }

    private static native long nativeCreate();
    private static native long nativeCreateWithConfig(int method, int boxSizeMethod,
                                                      double boxSize, int reversal);
    private static native void nativeDestroy(long ptr);
    private static native boolean nativeAddData(long ptr, double high, double low,
                                                double close, long timestamp);
    private static native boolean nativeAddPrice(long ptr, double price, long timestamp);
    private static native int nativeColumnCount(long ptr);
    private static native int nativeXColumnCount(long ptr);
    private static native int nativeOColumnCount(long ptr);
    private static native double nativeBoxSize(long ptr);
    private static native int nativeColumnType(long ptr, int index);
    private static native int nativeColumnBoxCount(long ptr, int index);
    private static native double nativeColumnHighest(long ptr, int index);
    private static native double nativeColumnLowest(long ptr, int index);
    private static native double nativeBoxPrice(long ptr, int columnIndex, int boxIndex);
    private static native int nativeBoxType(long ptr, int columnIndex, int boxIndex);
    private static native String nativeBoxMarker(long ptr, int columnIndex, int boxIndex);
    private static native boolean nativeHasBullishBias(long ptr);
    private static native boolean nativeHasBearishBias(long ptr);
    private static native void nativeClear(long ptr);
    private static native String nativeToString(long ptr);
    private static native String nativeToAscii(long ptr);
}
