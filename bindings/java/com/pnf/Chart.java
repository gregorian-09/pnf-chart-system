/**
 */
package com.pnf;

/**
 * Chart binding type.
 */
public class Chart implements AutoCloseable {
    static {
        System.loadLibrary("pnfjni");
    }

    private long nativePtr;

    /**
     */
    /**
     * t API.
     * @return 
     */
    public Chart() {
        nativePtr = nativeCreate();
    }

    /**
     */
    /**
     * t API.
     * @param config 
     * @return 
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
     */
    /**
     * addData API.
     * @param high 
     * @param low 
     * @param close 
     * @param timestamp 
     * @return 
     */
    public boolean addData(double high, double low, double close, long timestamp) {
        return nativeAddData(nativePtr, high, low, close, timestamp);
    }

    /**
     */
    /**
     * addPrice API.
     * @param price 
     * @param timestamp 
     * @return 
     */
    public boolean addPrice(double price, long timestamp) {
        return nativeAddPrice(nativePtr, price, timestamp);
    }

    /**
     */
    /**
     * columnCount API.
     * @return 
     */
    public int columnCount() {
        return nativeColumnCount(nativePtr);
    }

    /**
     */
    /**
     * xColumnCount API.
     * @return 
     */
    public int xColumnCount() {
        return nativeXColumnCount(nativePtr);
    }

    /**
     */
    /**
     * oColumnCount API.
     * @return 
     */
    public int oColumnCount() {
        return nativeOColumnCount(nativePtr);
    }

    /**
     */
    /**
     * boxSize API.
     * @return 
     */
    public double boxSize() {
        return nativeBoxSize(nativePtr);
    }

    /**
     */
    /**
     * columnType API.
     * @param index 
     * @return 
     */
    public ColumnType columnType(int index) {
        return ColumnType.values()[nativeColumnType(nativePtr, index)];
    }

    /**
     */
    /**
     * columnBoxCount API.
     * @param index 
     * @return 
     */
    public int columnBoxCount(int index) {
        return nativeColumnBoxCount(nativePtr, index);
    }

    /**
     */
    /**
     * columnHighest API.
     * @param index 
     * @return 
     */
    public double columnHighest(int index) {
        return nativeColumnHighest(nativePtr, index);
    }

    /**
     */
    /**
     * columnLowest API.
     * @param index 
     * @return 
     */
    public double columnLowest(int index) {
        return nativeColumnLowest(nativePtr, index);
    }

    /**
     */
    /**
     * hasBullishBias API.
     * @return 
     */
    public boolean hasBullishBias() {
        return nativeHasBullishBias(nativePtr);
    }

    /**
     */
    /**
     * hasBearishBias API.
     * @return 
     */
    public boolean hasBearishBias() {
        return nativeHasBearishBias(nativePtr);
    }

    /**
     */
    /**
     * clear API.
     */
    public void clear() {
        nativeClear(nativePtr);
    }

    long getNativePtr() {
        return nativePtr;
    }

    @Override
    /**
     */
    /**
     * toString API.
     * @return 
     */
    public String toString() {
        return nativeToString(nativePtr);
    }

    @Override
    /**
     */
    /**
     * close API.
     */
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
    private static native boolean nativeHasBullishBias(long ptr);
    private static native boolean nativeHasBearishBias(long ptr);
    private static native void nativeClear(long ptr);
    private static native String nativeToString(long ptr);
}
