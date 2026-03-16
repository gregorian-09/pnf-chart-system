package com.pnf;

/**
 * Input method used to build Point and Figure columns from market data.
 */
public enum ConstructionMethod {
    /** Build using close prices only. */
    CLOSE,
    /** Build using the high/low range of each bar. */
    HIGH_LOW
}
