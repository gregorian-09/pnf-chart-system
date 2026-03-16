package com.pnf;

/**
 * Chart pattern categories detected by the indicator engine.
 */
public enum PatternType {
    /** No pattern. */
    NONE,
    /** Double top breakout pattern. */
    DOUBLE_TOP_BREAKOUT,
    /** Double bottom breakdown pattern. */
    DOUBLE_BOTTOM_BREAKDOWN,
    /** Triple top breakout pattern. */
    TRIPLE_TOP_BREAKOUT,
    /** Triple bottom breakdown pattern. */
    TRIPLE_BOTTOM_BREAKDOWN,
    /** Quadruple top breakout pattern. */
    QUADRUPLE_TOP_BREAKOUT,
    /** Quadruple bottom breakdown pattern. */
    QUADRUPLE_BOTTOM_BREAKDOWN,
    /** Ascending triple top pattern. */
    ASCENDING_TRIPLE_TOP,
    /** Descending triple bottom pattern. */
    DESCENDING_TRIPLE_BOTTOM,
    /** Bullish catapult pattern. */
    BULLISH_CATAPULT,
    /** Bearish catapult pattern. */
    BEARISH_CATAPULT,
    /** Bullish signal reversed pattern. */
    BULLISH_SIGNAL_REVERSED,
    /** Bearish signal reversed pattern. */
    BEARISH_SIGNAL_REVERSED,
    /** Bullish triangle pattern. */
    BULLISH_TRIANGLE,
    /** Bearish triangle pattern. */
    BEARISH_TRIANGLE,
    /** Long tail down pattern. */
    LONG_TAIL_DOWN,
    /** High pole warning pattern. */
    HIGH_POLE,
    /** Low pole warning pattern. */
    LOW_POLE,
    /** Bull trap pattern. */
    BULL_TRAP,
    /** Bear trap pattern. */
    BEAR_TRAP,
    /** Spread triple top pattern. */
    SPREAD_TRIPLE_TOP,
    /** Spread triple bottom pattern. */
    SPREAD_TRIPLE_BOTTOM
}
