/// \file types.hpp
/// \brief Public enums and data types.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef TYPES_HPP
#define TYPES_HPP

#include <chrono>

namespace pnf {
    /**
     * @brief Types of boxes used in Point & Figure charts.
     */
    enum class BoxType { X, O };

    /**
     * @brief Types of columns in a Point & Figure chart.
     */
    enum class ColumnType { X, O, Mixed };

    /**
     * @brief Methods of constructing a Point & Figure chart.
     */
    enum class ConstructionMethod { Close, HighLow };

    /**
     * @brief Methods for determining box size.
     */
    enum class BoxSizeMethod { Fixed, Traditional, Percentage, Points };

    /**
     * @brief Types of trend lines.
     */
    enum class TrendLineType { BullishSupport, BearishResistance };

    /**
     * @brief Types of trading signals.
     */
    enum class SignalType { None, Buy, Sell };

    /**
     * @brief Types of chart patterns in Point & Figure analysis.
     */
    enum class PatternType {
        None,
        DoubleTopBreakout,
        DoubleBottomBreakdown,
        TripleTopBreakout,
        TripleBottomBreakdown,
        QuadrupleTopBreakout,
        QuadrupleBottomBreakdown,
        AscendingTripleTop,
        DescendingTripleBottom,
        BullishCatapult,
        BearishCatapult,
        BullishSignalReversed,
        BearishSignalReversed,
        BullishTriangle,
        BearishTriangle,
        LongTailDown,
        HighPole,
        LowPole,
        BullTrap,
        BearTrap,
        SpreadTripleTop,
        SpreadTripleBottom
    };

    /**
     * @brief Timestamp type used for OHLC and signals.
     */
    using Timestamp = std::chrono::system_clock::time_point;

    /**
     * @brief Structure representing OHLC (Open, High, Low, Close) data with optional volume.
     */
    struct OHLC {
        Timestamp time;     /**< Time of the bar/candle */
        double open{};      /**< Opening price */
        double high{};      /**< Highest price */
        double low{};       /**< Lowest price */
        double close{};     /**< Closing price */
        double volume = 0.0; /**< Volume, default 0 */
    };

    /**
     * @brief Structure representing a trading signal.
     */
    struct Signal {
        SignalType type{};          /**< Type of signal (Buy, Sell, None) */
        int column_index{};         /**< Index of the column where the signal occurs */
        double price{};             /**< Price of the signal */
        Timestamp time;             /**< Timestamp of the signal */
    };

    /**
     * @brief Structure representing a chart pattern.
     */
    struct Pattern {
        PatternType type;      /**< Type of pattern */
        int start_column;      /**< Index of the starting column */
        int end_column;        /**< Index of the ending column */
        double price;          /**< Price associated with the pattern */
    };

    /**
     * @brief Structure representing a support or resistance level.
     */
    struct SupportResistanceLevel {
        double price;          /**< Price of the level */
        int touch_count;       /**< Number of times the level was touched */
        bool is_support;       /**< True if support, false if resistance */
        int first_column;      /**< Index of the first column touching the level */
        int last_column;       /**< Index of the last column touching the level */
    };

    /**
     * @brief Structure representing a price objective.
     */
    struct PriceObjective {
        double target_price;   /**< Target price for the objective */
        int base_column;       /**< Base column used to calculate the objective */
        int box_count;         /**< Number of boxes involved in the objective */
        bool is_bullish;       /**< True if the objective is bullish */
    };

    /**
     * @brief Structure representing a point on a trend line.
     */
    struct TrendLinePoint {
        int column_index;      /**< Column index of the point */
        double price;          /**< Price at the trend line point */
        int box_index;         /**< Box index within the column */
    };

    /**
     * @brief Convert a PatternType enum to a human-readable string.
     *
     * @param type PatternType to convert
     * @return const char* String representation of the pattern type
     */
    const char* pattern_type_to_string(PatternType type);

    /**
     * @brief Check if a pattern type is bullish.
     *
     * @param type PatternType to check
     * @return true if bullish, false otherwise
     */
    bool is_bullish_pattern(PatternType type);

    /**
     * @brief Check if a pattern type is bearish.
     *
     * @param type PatternType to check
     * @return true if bearish, false otherwise
     */
    bool is_bearish_pattern(PatternType type);
} // namespace pnf


#endif //TYPES_HPP
