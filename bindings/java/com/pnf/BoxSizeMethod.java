package com.pnf;

/**
 * Supported box-size calculation modes for Point and Figure chart construction.
 */
public enum BoxSizeMethod {
    /** Use a fixed numeric box size. */
    FIXED,
    /** Use traditional scaling rules based on instrument price range. */
    TRADITIONAL,
    /** Use a percentage of price for dynamic box sizing. */
    PERCENTAGE,
    /** Use point-based sizing from the underlying instrument tick scale. */
    POINTS
}
