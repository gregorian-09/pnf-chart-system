/// \file trendline.hpp
/// \brief Trend line models and manager.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef TRENDLINE_HPP
#define TRENDLINE_HPP

#include "column.hpp"
#include <vector>

namespace pnf {
    /**
     * @brief Represents a trend line in a Point & Figure chart.
     */
    class TrendLine {
    public:
        /**
         * @brief Constructs a new TrendLine.
         *
         * @param type Type of trend line (BullishSupport or BearishResistance)
         * @param start_column Index of the starting column
         * @param start_price Price at the start
         * @param start_box_index Index of the starting box in the column
         * @param box_size Size of each box used for price calculation
         */
        TrendLine(TrendLineType type, int start_column, double start_price,
                  int start_box_index, double box_size);

        /**
         * @brief Updates the end point of the trend line.
         *
         * @param column Index of the column
         * @param price Price at the column
         * @param box_index Index of the box at the column
         */
        void update_end_point(int column, double price, int box_index);

        /**
         * @brief Checks if the trend line has been broken at the given column and price.
         *
         * @param column Column index to check
         * @param price Price at the column
         * @return true if broken, false otherwise
         */
        bool is_broken(int column, double price) const;

        /**
         * @brief Tests a new point against the trend line and updates if necessary.
         *
         * @param column Column index
         * @param price Price at the column
         * @return true if the trend line was touched or updated
         */
        bool test(int column, double price);

        /**
         * @brief Computes the expected price of the trend line at a given column.
         *
         * @param column Column index
         * @return Price of the trend line at the column
         */
        double price_at_column(int column) const;

        /**
         * @brief Gets the type of the trend line.
         *
         * @return TrendLineType
         */
        TrendLineType type() const { return type_; }

        /**
         * @brief Gets the starting point of the trend line.
         *
         * @return TrendLinePoint struct
         */
        TrendLinePoint start_point() const { return start_; }

        /**
         * @brief Gets the current end point of the trend line.
         *
         * @return TrendLinePoint struct
         */
        TrendLinePoint end_point() const { return end_; }

        /**
         * @brief Checks if the trend line is currently active.
         *
         * @return true if active, false otherwise
         */
        bool is_active() const { return active_; }

        /**
         * @brief Sets the trend line as active or inactive.
         *
         * @param active True to set active, false to set inactive
         */
        void set_active(bool active) { active_ = active; }

        /**
         * @brief Checks if the trend line was touched by price action.
         *
         * @return true if touched
         */
        bool was_touched() const { return touched_; }

        /**
         * @brief Returns the number of touches the trend line has received.
         *
         * @return Touch count
         */
        int touch_count() const { return touch_count_; }

        /**
         * @brief Returns a string representation of the trend line.
         *
         * @return String describing the trend line
         */
        std::string to_string() const;

    private:
        TrendLineType type_;    /**< Type of trend line */
        TrendLinePoint start_;  /**< Starting point of the trend line */
        TrendLinePoint end_;    /**< Ending point of the trend line */
        double box_size_;       /**< Box size used for price calculations */
        bool active_;           /**< Whether the trend line is active */
        bool touched_;          /**< Whether it has been touched by price */
        int touch_count_;       /**< Number of touches */
    };

    /**
     * @brief Manages all trend lines in a Point & Figure chart.
     */
    class TrendLineManager {
    public:
        /**
         * @brief Constructs a TrendLineManager.
         *
         * @param box_size Box size used for trend line calculations
         */
        explicit TrendLineManager(double box_size);

        /**
         * @brief Updates trend lines based on the latest column data.
         *
         * @param columns Vector of column pointers
         * @param new_column_index Index of the newly added column
         */
        void update(const std::vector<std::unique_ptr<Column>>& columns, int new_column_index);

        /**
         * @brief Processes a new column to update trend lines.
         *
         * @param columns Vector of columns
         * @param column_index Index of the new column
         */
        void process_new_column(const std::vector<std::unique_ptr<Column>>& columns, int column_index);

        /**
         * @brief Checks if any trend lines have been broken at the given column.
         *
         * @param columns Vector of columns
         * @param column_index Column index to check
         */
        void check_break(const std::vector<std::unique_ptr<Column>>& columns, int column_index) const;

        /**
         * @brief Returns the currently active trend line.
         *
         * @return Pointer to the active TrendLine, or nullptr if none
         */
        TrendLine* active_trend_line() const { return active_; }

        /**
         * @brief Access all trend lines for modification.
         *
         * @return Vector of unique pointers to TrendLine objects
         */
        std::vector<std::unique_ptr<TrendLine>>& all_trend_lines() { return lines_; }

        /**
         * @brief Access all trend lines for read-only operations.
         *
         * @return Constant reference to the vector of TrendLine pointers
         */
        const std::vector<std::unique_ptr<TrendLine>>& all_trend_lines() const { return lines_; }

        /**
         * @brief Checks if a price is above the nearest bullish support trend line.
         *
         * @param column Column index
         * @param price Price to check
         * @return true if above bullish support
         */
        bool is_above_bullish_support(int column, double price) const;

        /**
         * @brief Checks if a price is below the nearest bearish resistance trend line.
         *
         * @param column Column index
         * @param price Price to check
         * @return true if below bearish resistance
         */
        bool is_below_bearish_resistance(int column, double price) const;

        /**
         * @brief Checks if the overall trend has a bullish bias.
         *
         * @return true if bullish bias
         */
        bool has_bullish_bias() const;

        /**
         * @brief Checks if the overall trend has a bearish bias.
         *
         * @return true if bearish bias
         */
        bool has_bearish_bias() const;

        /**
         * @brief Clears all trend lines.
         */
        void clear();

        /**
         * @brief Update the box size used for trend line calculations.
         *
         * @param box_size New box size
         */
        void set_box_size(double box_size) { box_size_ = box_size; }

        /**
         * @brief Returns a string representation of all trend lines.
         *
         * @return String describing trend lines
         */
        std::string to_string() const;

    private:
        /**
         * @brief Finds a significant low starting from a specific column.
         *
         * @param columns Vector of columns
         * @param from Column index to start search
         * @return Index of the significant low column
         */
        static int find_significant_low(const std::vector<std::unique_ptr<Column>>& columns, int from);

        /**
         * @brief Finds a significant high starting from a specific column.
         *
         * @param columns Vector of columns
         * @param from Column index to start search
         * @return Index of the significant high column
         */
        static int find_significant_high(const std::vector<std::unique_ptr<Column>>& columns, int from);

        /**
         * @brief Checks if a column is a significant low relative to surrounding columns.
         *
         * @param col Pointer to the column
         * @param all Vector of all columns
         * @param index Index of the column
         * @return true if significant low
         */
        static bool is_significant_low(const Column* col, const std::vector<std::unique_ptr<Column>>& all, int index);

        /**
         * @brief Checks if a column is a significant high relative to surrounding columns.
         *
         * @param col Pointer to the column
         * @param all Vector of all columns
         * @param index Index of the column
         * @return true if significant high
         */
        static bool is_significant_high(const Column* col, const std::vector<std::unique_ptr<Column>>& all, int index);

        std::vector<std::unique_ptr<TrendLine>> lines_; /**< All trend lines managed */
        TrendLine* active_;                              /**< Currently active trend line */
        double box_size_;                                /**< Box size used for trend line calculations */
    };
} // namespace pnf


#endif //TRENDLINE_HPP
