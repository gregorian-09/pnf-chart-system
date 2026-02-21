/// \file indicators.hpp
/// \brief Indicator calculation APIs.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef INDICATORS_HPP
#define INDICATORS_HPP

#include "chart.hpp"
#include <vector>
#include <memory>

namespace pnf
{
    /**
     * @brief Configuration parameters for technical indicators.
     */
    struct IndicatorConfig {
        int sma_short_period = 5;               /**< Period for short SMA */
        int sma_medium_period = 10;             /**< Period for medium SMA */
        int sma_long_period = 20;               /**< Period for long SMA */
        int bollinger_period = 20;              /**< Period for Bollinger Bands */
        double bollinger_std_devs = 2.0;        /**< Standard deviations for Bollinger Bands */
        int rsi_period = 14;                    /**< RSI period */
        double rsi_overbought = 70.0;           /**< RSI overbought threshold */
        double rsi_oversold = 30.0;             /**< RSI oversold threshold */
        double bullish_alert_threshold = 70.0;  /**< Bullish percent threshold for alerts */
        double bearish_alert_threshold = 30.0;  /**< Bearish percent threshold for alerts */
        double support_resistance_threshold = 0.01;     /**< Price difference threshold for S/R levels */
        int congestion_min_columns = 4;                 /**< Minimum columns for congestion detection */
        double congestion_price_range = 0.05;           /**< Price range threshold for congestion detection */
    };

    /**
     * @brief Column-specific chart data for exporting.
     */
    struct ColumnData {
        int index{};              /**< Column index */
        ColumnType type{};        /**< Column type (X, O, Mixed) */
        double high{};            /**< Highest price in column */
        double low{};             /**< Lowest price in column */
        int box_count{};          /**< Number of boxes */
        std::string marker{};     /**< Optional marker (month, etc.) */
    };

    /**
     * @brief Exportable chart data structure.
     */
    struct ChartData {
        std::vector<ColumnData> columns{};    /**< Column details */
        std::vector<double> prices{};         /**< All prices in the chart */
        double box_size{};                    /**< Box size */
        int reversal{};                       /**< Reversal amount */
        ConstructionMethod method{};          /**< Chart construction method */
    };

    /**
     * @brief Exportable indicator results.
     */
    struct IndicatorData {
        std::vector<double> sma_short{};      /**< Short SMA values */
        std::vector<double> sma_medium{};     /**< Medium SMA values */
        std::vector<double> sma_long{};       /**< Long SMA values */
        std::vector<double> bollinger_middle{}; /**< Middle Bollinger Band */
        std::vector<double> bollinger_upper{};  /**< Upper Bollinger Band */
        std::vector<double> bollinger_lower{};  /**< Lower Bollinger Band */
        std::vector<double> rsi{};            /**< RSI values */
        std::vector<double> obv{};            /**< OBV values */
        double bullish_percent{};             /**< Bullish percent */
        std::vector<Signal> signals{};        /**< Trading signals */
        std::vector<Pattern> patterns{};      /**< Detected patterns */
        std::vector<SupportResistanceLevel> support_levels{}; /**< Support levels */
        std::vector<SupportResistanceLevel> resistance_levels{}; /**< Resistance levels */
        std::vector<PriceObjective> price_objectives{}; /**< Price objectives */
    };

    /**
     * @brief Simple Moving Average (SMA) indicator.
     */
    class MovingAverage {
    public:
        explicit MovingAverage(int period);

        void calculate(const Chart& chart);
        void set_period(int period);
        [[nodiscard]] double value(int column) const;
        [[nodiscard]] bool has_value(int column) const;
        [[nodiscard]] int period() const { return period_; }
        [[nodiscard]] const std::vector<double>& values() const { return values_; }
        [[nodiscard]] std::vector<double> values_copy() const { return values_; }
        [[nodiscard]] std::string to_string() const;

    private:
        static double column_average(const Column* col);

        int period_;                  /**< SMA period */
        std::vector<double> values_;  /**< Calculated SMA values */
    };

    /**
     * @brief Bollinger Bands indicator.
     */
    class BollingerBands {
    public:
        explicit BollingerBands(int period = 20, double std_devs = 2.0);

        void calculate(const Chart& chart);
        void set_period(int period);
        void set_std_devs(double devs);
        [[nodiscard]] double middle(int column) const;
        [[nodiscard]] double upper(int column) const;
        [[nodiscard]] double lower(int column) const;
        [[nodiscard]] bool has_value(int column) const;
        [[nodiscard]] bool is_above_upper(int column, double price) const;
        [[nodiscard]] bool is_below_lower(int column, double price) const;

        [[nodiscard]] int period() const { return period_; }
        [[nodiscard]] double std_devs() const { return std_devs_; }
        [[nodiscard]] const std::vector<double>& middle_band() const { return middle_; }
        [[nodiscard]] const std::vector<double>& upper_band() const { return upper_; }
        [[nodiscard]] const std::vector<double>& lower_band() const { return lower_; }
        [[nodiscard]] std::vector<double> middle_copy() const { return middle_; }
        [[nodiscard]] std::vector<double> upper_copy() const { return upper_; }
        [[nodiscard]] std::vector<double> lower_copy() const { return lower_; }

        [[nodiscard]] std::string to_string() const;

    private:
        static double std_deviation(const std::vector<double>& vals, double mean);

        int period_;                   /**< Bollinger Bands period */
        double std_devs_;              /**< Number of standard deviations */
        std::vector<double> middle_;   /**< Middle band */
        std::vector<double> upper_;    /**< Upper band */
        std::vector<double> lower_;    /**< Lower band */
    };

    /**
     * @brief Relative Strength Index (RSI) indicator.
     */
    class RSI {
    public:
        explicit RSI(int period = 14);

        void calculate(const Chart& chart);
        void set_period(int period);
        void set_thresholds(double overbought, double oversold);
        [[nodiscard]] double value(int column) const;
        [[nodiscard]] bool has_value(int column) const;
        [[nodiscard]] bool is_overbought(int column) const;
        [[nodiscard]] bool is_oversold(int column) const;
        [[nodiscard]] bool is_overbought_custom(int column, double threshold) const;
        [[nodiscard]] bool is_oversold_custom(int column, double threshold) const;

        [[nodiscard]] int period() const { return period_; }
        [[nodiscard]] double overbought_threshold() const { return overbought_; }
        [[nodiscard]] double oversold_threshold() const { return oversold_; }
        [[nodiscard]] const std::vector<double>& values() const { return values_; }
        [[nodiscard]] std::vector<double> values_copy() const { return values_; }
        [[nodiscard]] std::string to_string() const;

    private:
        int period_;                  /**< RSI period */
        double overbought_ = 70.0;    /**< Overbought threshold */
        double oversold_ = 30.0;      /**< Oversold threshold */
        std::vector<double> values_;  /**< Calculated RSI values */
    };

    /**
     * @brief On-Balance Volume (OBV) indicator.
     */
    class OnBalanceVolume {
    public:
        OnBalanceVolume() = default;

        void calculate(const Chart& chart, const std::vector<OHLC>& ohlc_data);
        [[nodiscard]] double value(int column) const;
        [[nodiscard]] bool has_value(int column) const;

        [[nodiscard]] const std::vector<double>& values() const { return values_; }
        [[nodiscard]] std::vector<double> values_copy() const { return values_; }
        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<double> values_{}; /**< OBV values */
    };

    /**
     * @brief Bullish Percent indicator.
     */
    class BullishPercent {
    public:
        BullishPercent() = default;

        void calculate(const Chart& chart);
        void set_thresholds(double bullish, double bearish);
        [[nodiscard]] double value() const { return value_; }
        [[nodiscard]] bool is_bullish_alert() const { return value_ > bullish_threshold_; }
        [[nodiscard]] bool is_bearish_alert() const { return value_ < bearish_threshold_; }
        [[nodiscard]] double bullish_threshold() const { return bullish_threshold_; }
        [[nodiscard]] double bearish_threshold() const { return bearish_threshold_; }

        [[nodiscard]] std::string to_string() const;

    private:
        double value_ = 50.0;           /**< Current bullish percent value */
        double bullish_threshold_ = 70.0; /**< Bullish alert threshold */
        double bearish_threshold_ = 30.0; /**< Bearish alert threshold */
    };

    /**
     * @brief Detects trading signals from chart data.
     */
    class SignalDetector {
    public:
        SignalDetector() = default;

        void detect(const Chart& chart);
        [[nodiscard]] SignalType current_signal() const { return current_; }
        [[nodiscard]] const std::vector<Signal>& signals() const { return signals_; }
        [[nodiscard]] std::vector<Signal> signals_copy() const { return signals_; }
        [[nodiscard]] Signal last_signal() const;
        [[nodiscard]] bool has_buy_signal() const { return current_ == SignalType::Buy; }
        [[nodiscard]] bool has_sell_signal() const { return current_ == SignalType::Sell; }

        [[nodiscard]] std::vector<Signal> buy_signals() const;
        [[nodiscard]] std::vector<Signal> sell_signals() const;
        [[nodiscard]] int buy_count() const;
        [[nodiscard]] int sell_count() const;

        [[nodiscard]] std::string to_string() const;

    private:
        static bool is_buy_signal(const Chart& chart, int column);
        static bool is_sell_signal(const Chart& chart, int column);

        std::vector<Signal> signals_{}; /**< Detected signals */
        SignalType current_ = SignalType::None; /**< Current signal type */
    };

    /**
     * @brief Recognizes chart patterns in Point & Figure charts.
     */
    class PatternRecognizer {
    public:
        PatternRecognizer() = default;

        bool detect_double_top_breakout(const Chart& chart, int col);
        bool detect_double_bottom_breakdown(const Chart& chart, int col);
        bool detect_triple_top_breakout(const Chart& chart, int col);
        bool detect_triple_bottom_breakdown(const Chart& chart, int col);
        bool detect_quadruple_top_breakout(const Chart& chart, int col);
        bool detect_quadruple_bottom_breakdown(const Chart& chart, int col);
        bool detect_ascending_triple_top(const Chart& chart, int col);
        bool detect_descending_triple_bottom(const Chart& chart, int col);
        bool detect_bullish_catapult(int col);
        bool detect_bearish_catapult(int col);
        bool detect_bullish_signal_reversed(const Chart& chart, int col);
        bool detect_bearish_signal_reversed(const Chart& chart, int col);
        bool detect_bullish_triangle(const Chart& chart, int col);
        bool detect_bearish_triangle(const Chart& chart, int col);
        bool detect_long_tail_down(const Chart& chart, int col);
        bool detect_high_pole(const Chart& chart, int col);
        bool detect_low_pole(const Chart& chart, int col);
        bool detect_bull_trap(const Chart& chart, int col);
        bool detect_bear_trap(const Chart& chart, int col);
        bool detect_spread_triple_top(const Chart& chart, int col);
        bool detect_spread_triple_bottom(const Chart& chart, int col);
        void detect(const Chart& chart);
        [[nodiscard]] const std::vector<Pattern>& patterns() const { return patterns_; }
        [[nodiscard]] std::vector<Pattern> patterns_copy() const { return patterns_; }
        [[nodiscard]] std::vector<Pattern> bullish_patterns() const;
        [[nodiscard]] std::vector<Pattern> bearish_patterns() const;
        [[nodiscard]] Pattern latest_pattern() const;
        [[nodiscard]] bool has_pattern(PatternType type) const;

        [[nodiscard]] std::vector<Pattern> patterns_of_type(PatternType type) const;
        [[nodiscard]] int pattern_count() const { return static_cast<int>(patterns_.size()); }
        [[nodiscard]] int bullish_count() const;
        [[nodiscard]] int bearish_count() const;

        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<Pattern> patterns_{}; /**< Detected patterns */
    };

    /**
     * @brief Identifies support and resistance levels in the chart.
     */
    class SupportResistance {
    public:
        explicit SupportResistance(double threshold = 0.01);

        void identify(const Chart& chart);
        void set_threshold(double threshold);
        [[nodiscard]] double threshold() const { return threshold_; }

        [[nodiscard]] const std::vector<SupportResistanceLevel>& levels() const { return levels_; }
        [[nodiscard]] std::vector<SupportResistanceLevel> levels_copy() const { return levels_; }
        [[nodiscard]] std::vector<SupportResistanceLevel> support_levels() const;
        [[nodiscard]] std::vector<SupportResistanceLevel> resistance_levels() const;
        [[nodiscard]] std::vector<SupportResistanceLevel> significant_levels(int min_touches = 3) const;
        [[nodiscard]] bool is_near_support(double price, double tolerance = 0.02) const;
        [[nodiscard]] bool is_near_resistance(double price, double tolerance = 0.02) const;

        [[nodiscard]] std::vector<double> support_prices() const;
        [[nodiscard]] std::vector<double> resistance_prices() const;

        [[nodiscard]] std::string to_string() const;

    private:
        void merge_similar_levels();

        std::vector<SupportResistanceLevel> levels_; /**< All detected levels */
        double threshold_;                             /**< Price tolerance for merging levels */
    };

    /**
     * @brief Calculates price objectives using vertical count method.
     */
    class PriceObjectiveCalculator {
    public:
        PriceObjectiveCalculator() = default;

        void calculate_vertical_count(const Chart& chart, int column);
        void calculate_all(const Chart& chart);
        [[nodiscard]] const std::vector<PriceObjective>& objectives() const { return objectives_; }
        [[nodiscard]] std::vector<PriceObjective> objectives_copy() const { return objectives_; }
        [[nodiscard]] PriceObjective latest() const;

        [[nodiscard]] std::vector<PriceObjective> bullish_objectives() const;
        [[nodiscard]] std::vector<PriceObjective> bearish_objectives() const;
        [[nodiscard]] std::vector<double> bullish_targets() const;
        [[nodiscard]] std::vector<double> bearish_targets() const;

        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<PriceObjective> objectives_{}; /**< Calculated price objectives */
    };

    /**
     * @brief Detects congestion zones in the chart.
     */
    class CongestionDetector {
    public:
        /**
         * @brief Represents a congestion zone.
         */
        struct CongestionZone {
            int start_column;
            int end_column;
            double high_price;
            double low_price;
            int column_count;
        };

        explicit CongestionDetector(int min_columns = 4, double price_range_threshold = 0.05);

        void detect(const Chart& chart);
        void set_min_columns(int min);
        void set_threshold(double threshold);
        [[nodiscard]] int min_columns() const { return min_columns_; }
        [[nodiscard]] double threshold() const { return threshold_; }

        [[nodiscard]] const std::vector<CongestionZone>& zones() const { return zones_; }
        [[nodiscard]] std::vector<CongestionZone> zones_copy() const { return zones_; }
        [[nodiscard]] bool is_in_congestion(int column) const;
        [[nodiscard]] CongestionZone largest_zone() const;

        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<CongestionZone> zones_; /**< Detected congestion zones */
        int min_columns_;                   /**< Minimum columns for congestion */
        double threshold_;                  /**< Price range threshold for congestion */
    };

    /**
     * @brief High-level container for all technical indicators.
     */
    class Indicators {
    public:
        Indicators();
        explicit Indicators(const IndicatorConfig& config);

        void configure(const IndicatorConfig& config);
        [[nodiscard]] IndicatorConfig config() const { return config_; }

        void calculate(const Chart& chart) const;
        void calculate_with_volume(const Chart& chart, const std::vector<OHLC>& ohlc_data) const;

        [[nodiscard]] MovingAverage* sma_short() { return sma_short_.get(); }
        [[nodiscard]] MovingAverage* sma_medium() { return sma_medium_.get(); }
        [[nodiscard]] MovingAverage* sma_long() { return sma_long_.get(); }
        [[nodiscard]] BollingerBands* bollinger() { return bollinger_.get(); }
        [[nodiscard]] RSI* rsi() { return rsi_.get(); }
        [[nodiscard]] OnBalanceVolume* obv() { return obv_.get(); }
        [[nodiscard]] BullishPercent* bullish_percent() { return bullish_percent_.get(); }
        [[nodiscard]] SignalDetector* signals() { return signals_.get(); }
        [[nodiscard]] PatternRecognizer* patterns() { return patterns_.get(); }
        [[nodiscard]] SupportResistance* support_resistance() { return support_resistance_.get(); }
        [[nodiscard]] PriceObjectiveCalculator* objectives() { return objectives_.get(); }
        [[nodiscard]] CongestionDetector* congestion() { return congestion_.get(); }

        [[nodiscard]] const MovingAverage* sma_short() const { return sma_short_.get(); }
        [[nodiscard]] const MovingAverage* sma_medium() const { return sma_medium_.get(); }
        [[nodiscard]] const MovingAverage* sma_long() const { return sma_long_.get(); }
        [[nodiscard]] const BollingerBands* bollinger() const { return bollinger_.get(); }
        [[nodiscard]] const RSI* rsi() const { return rsi_.get(); }
        [[nodiscard]] const OnBalanceVolume* obv() const { return obv_.get(); }
        [[nodiscard]] const BullishPercent* bullish_percent() const { return bullish_percent_.get(); }
        [[nodiscard]] const SignalDetector* signals() const { return signals_.get(); }
        [[nodiscard]] const PatternRecognizer* patterns() const { return patterns_.get(); }
        [[nodiscard]] const SupportResistance* support_resistance() const { return support_resistance_.get(); }
        [[nodiscard]] const PriceObjectiveCalculator* objectives() const { return objectives_.get(); }
        [[nodiscard]] const CongestionDetector* congestion() const { return congestion_.get(); }

        [[nodiscard]] IndicatorData export_data() const;
        static ChartData export_chart_data(const Chart& chart);

        [[nodiscard]] std::string summary() const;
        [[nodiscard]] std::string to_string() const;

    private:
        void initialize();

        IndicatorConfig config_;
        std::unique_ptr<MovingAverage> sma_short_;
        std::unique_ptr<MovingAverage> sma_medium_;
        std::unique_ptr<MovingAverage> sma_long_;
        std::unique_ptr<BollingerBands> bollinger_;
        std::unique_ptr<RSI> rsi_;
        std::unique_ptr<OnBalanceVolume> obv_;
        std::unique_ptr<BullishPercent> bullish_percent_;
        std::unique_ptr<SignalDetector> signals_;
        std::unique_ptr<PatternRecognizer> patterns_;
        std::unique_ptr<SupportResistance> support_resistance_;
        std::unique_ptr<PriceObjectiveCalculator> objectives_;
        std::unique_ptr<CongestionDetector> congestion_;
    };
} // namespace pnf

#endif //INDICATORS_HPP
