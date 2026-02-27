/// \file chart.hpp
/// \brief Point & Figure chart core API.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef CHART_HPP
#define CHART_HPP

#include "column.hpp"
#include "trendline.hpp"
#include <vector>
#include <memory>
#include <chrono>
#include <array>

namespace pnf {
    /**
     * @brief Configuration settings for a Point & Figure chart.
     */
    struct ChartConfig {
        ConstructionMethod method = ConstructionMethod::Close; /**< Chart construction method */
        BoxSizeMethod box_size_method = BoxSizeMethod::Traditional; /**< Method to determine box size */
        double box_size = 0.0; /**< Fixed box size (if applicable) */
        int reversal = 3; /**< Reversal amount in boxes */
    };

    /**
     * @brief Represents a Point & Figure chart.
     *
     * Manages columns, boxes, and trend lines. Supports adding price or OHLC data,
     * computing bullish/bearish biases, and maintaining trend lines.
     */
    class Chart {
    public:
        /**
         * @brief Constructs a new Chart object with optional configuration.
         *
         * @param config ChartConfig structure with settings (optional)
         */
        explicit Chart(const ChartConfig& config = {});

        /**
         * @brief Adds a data point using high, low, and close prices.
         *
         * @param high High price
         * @param low Low price
         * @param close Close price
         * @param time Timestamp of the data
         * @return true if the chart was updated
         */
        bool add_data(double high, double low, double close, Timestamp time);

        /**
         * @brief Adds a single price data point.
         *
         * @param price Price
         * @param time Timestamp
         * @return true if the chart was updated
         */
        bool add_data(double price, Timestamp time);

        /**
         * @brief Adds OHLC data to the chart.
         *
         * @param ohlc OHLC structure
         * @return true if the chart was updated
         */
        bool add_ohlc(const OHLC& ohlc);

        /**
         * @brief Returns the number of columns in the chart.
         *
         * @return Column count
         */
        size_t column_count() const { return columns_.size(); }

        /**
         * @brief Returns a pointer to a column at a given index.
         *
         * @param index Column index
         * @return Pointer to Column or nullptr if out of range
         */
        Column* column(size_t index);
        const Column* column(size_t index) const;

        /**
         * @brief Returns the last column in the chart.
         *
         * @return Pointer to the last Column or nullptr if no columns exist
         */
        Column* last_column();
        const Column* last_column() const;

        /**
         * @brief Counts X-type columns.
         *
         * @return Number of X columns
         */
        size_t x_column_count() const;

        /**
         * @brief Counts O-type columns.
         *
         * @return Number of O columns
         */
        size_t o_column_count() const;

        /**
         * @brief Counts mixed-type columns.
         *
         * @return Number of mixed columns
         */
        size_t mixed_column_count() const;

        /**
         * @brief Returns indices of X columns.
         *
         * @return Vector of X column indices
         */
        std::vector<size_t> x_column_indices() const;

        /**
         * @brief Returns indices of O columns.
         *
         * @return Vector of O column indices
         */
        std::vector<size_t> o_column_indices() const;

        /**
         * @brief Returns indices of mixed columns.
         *
         * @return Vector of mixed column indices
         */
        std::vector<size_t> mixed_column_indices() const;

        /**
         * @brief Returns all prices from all boxes in the chart.
         *
         * @return Vector of prices
         */
        std::vector<double> all_prices() const;

        /**
         * @brief Returns the chart configuration.
         *
         * @return ChartConfig structure
         */
        ChartConfig config() const { return config_; }

        /**
         * @brief Returns the current box size.
         *
         * @return Box size in price units
         */
        double current_box_size() const { return last_box_size_; }

        /**
         * @brief Returns the trend line manager.
         *
         * @return Pointer to TrendLineManager
         */
        TrendLineManager* trend_line_manager() const { return trend_manager_.get(); }

        /**
         * @brief Checks if the chart has a bullish bias.
         *
         * @return true if bullish
         */
        bool has_bullish_bias() const;

        /**
         * @brief Checks if the chart has a bearish bias.
         *
         * @return true if bearish
         */
        bool has_bearish_bias() const;

        /**
         * @brief Determines if bullish signals should be taken.
         *
         * @return true if bullish signals are valid
         */
        bool should_take_bullish_signals() const;

        /**
         * @brief Determines if bearish signals should be taken.
         *
         * @return true if bearish signals are valid
         */
        bool should_take_bearish_signals() const;

        /**
         * @brief Checks if a price is above bullish support lines.
         *
         * @param price Price to check
         * @return true if above support
         */
        bool is_above_bullish_support(double price) const;

        /**
         * @brief Checks if a price is below bearish resistance lines.
         *
         * @param price Price to check
         * @return true if below resistance
         */
        bool is_below_bearish_resistance(double price) const;

        /**
         * @brief Clears the chart and all columns.
         */
        void clear();

        /**
         * @brief Returns a string representation of the chart.
         *
         * @return String describing chart state
         */
        std::string to_string() const;

        /**
         * @brief Returns all columns in the chart.
         *
         * @return Vector of unique pointers to columns
         */
        const std::vector<std::unique_ptr<Column>>& columns() const { return columns_; }

    private:
        /**
         * @brief Processes high/low data for chart updates.
         *
         * @param high High price
         * @param low Low price
         * @param time Timestamp
         * @return true if chart updated
         */
        bool process_high_low(double high, double low, Timestamp time);

        /**
         * @brief Processes closing price for chart updates.
         *
         * @param close Closing price
         * @param time Timestamp
         * @return true if chart updated
         */
        bool process_close(double close, Timestamp time);

        /**
         * @brief Calculates appropriate box size for a price.
         *
         * @param price Reference price
         * @return Box size
         */
        double calculate_box_size(double price);

        /**
         * @brief Rounds a price to the nearest box size.
         *
         * @param price Price to round
         * @param round_up True to round up, false to round down
         * @return Rounded price
         */
        double round_to_box_size(double price, bool round_up);

        /**
         * @brief Checks if a price triggers a reversal.
         *
         * @param price Price to test
         * @param current Current column
         * @param new_type Output new box type if reversal occurs
         * @return true if reversal occurred
         */
        bool is_reversal(double price, const Column* current, BoxType& new_type);

        /**
         * @brief Returns a marker string for a month number.
         *
         * @param month Month index (1-12)
         * @return Marker string
         */
        static std::string get_month_marker(int month);

        /**
         * @brief Checks if the month has changed since the last processed time.
         *
         * @param current Current timestamp
         * @param marker Output month marker
         * @return true if month changed
         */
        bool has_month_changed(Timestamp current, std::string& marker) const;

        /**
         * @brief Returns the month marker for a given timestamp.
         *
         * @param time Timestamp
         * @return Month marker string
         */
        std::string get_month_marker_for_data(Timestamp time) const;

        std::vector<std::unique_ptr<Column>> columns_; /**< Chart columns */
        std::unique_ptr<TrendLineManager> trend_manager_; /**< Trend line manager */
        ChartConfig config_; /**< Chart configuration */
        Timestamp last_time_; /**< Last timestamp added */
        Timestamp last_processed_time_; /**< Last processed timestamp */
        int last_month_; /**< Last month number processed */
        double last_box_size_; /**< Last computed box size in price units */

        static constexpr std::array<const char*, 12> month_markers_ = {
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C"
        }; /**< Markers for each month */
    };
} // namespace pnf

#endif //CHART_HPP
