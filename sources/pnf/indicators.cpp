/// \file indicators.cpp
/// \brief Indicators implementation.

//
// Created by gregorian-rayne on 12/01/2026.
//

#include "pnf/indicators.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <cmath>

namespace pnf {
    MovingAverage::MovingAverage(const int period) : period_(period) {}

    void MovingAverage::set_period(const int period) {
        period_ = period;
        values_.clear();
    }

    double MovingAverage::column_average(const Column* col) {
        if (!col || col->box_count() == 0) return 0.0;
        return (col->highest_price() + col->lowest_price()) / 2.0;
    }

    void MovingAverage::calculate(const Chart& chart) {
        values_.clear();
        const size_t count = chart.column_count();
        for (size_t i = 0; i < count; i++) {
            if (static_cast<int>(i) < period_ - 1) {
                values_.push_back(0.0);
                continue;
            }
            double sum = 0.0;
            for (int j = 0; j < period_; j++)
                sum += column_average(chart.column(i - j));
            values_.push_back(sum / period_);
        }
    }

    double MovingAverage::value(const int column) const {
        if (column < 0 || column >= static_cast<int>(values_.size())) return 0.0;
        return values_[column];
    }

    bool MovingAverage::has_value(const int column) const {
        return column >= period_ - 1 && column < static_cast<int>(values_.size());
    }

    std::string MovingAverage::to_string() const {
        std::ostringstream oss;
        oss << "SMA(" << period_ << "): " << values_.size() << " values";
        return oss.str();
    }

    BollingerBands::BollingerBands(const int period, const double std_devs)
        : period_(period), std_devs_(std_devs) {}

    void BollingerBands::set_period(const int period) {
        period_ = period;
        middle_.clear();
        upper_.clear();
        lower_.clear();
    }

    void BollingerBands::set_std_devs(const double devs) {
        std_devs_ = devs;
    }

    double BollingerBands::std_deviation(const std::vector<double>& vals, const double mean) {
        double sum = 0.0;
        for (const double v : vals) {
            const double diff = v - mean;
            sum += diff * diff;
        }
        return std::sqrt(sum / static_cast<double>(vals.size()));
    }

    void BollingerBands::calculate(const Chart& chart) {
        middle_.clear();
        upper_.clear();
        lower_.clear();

        const size_t count = chart.column_count();
        for (size_t i = 0; i < count; i++) {
            if (static_cast<int>(i) < period_ - 1) {
                middle_.push_back(0.0);
                upper_.push_back(0.0);
                lower_.push_back(0.0);
                continue;
            }

            std::vector<double> avgs;
            for (int j = 0; j < period_; j++) {
                const Column* col = chart.column(i - j);
                avgs.push_back((col->highest_price() + col->lowest_price()) / 2.0);
            }

            double mean = std::accumulate(avgs.begin(), avgs.end(), 0.0) / period_;
            const double stddev = std_deviation(avgs, mean);

            middle_.push_back(mean);
            upper_.push_back(mean + std_devs_ * stddev);
            lower_.push_back(mean - std_devs_ * stddev);
        }
    }

    double BollingerBands::middle(const int column) const {
        return (column >= 0 && column < static_cast<int>(middle_.size())) ? middle_[column] : 0.0;
    }

    double BollingerBands::upper(const int column) const {
        return (column >= 0 && column < static_cast<int>(upper_.size())) ? upper_[column] : 0.0;
    }

    double BollingerBands::lower(const int column) const {
        return (column >= 0 && column < static_cast<int>(lower_.size())) ? lower_[column] : 0.0;
    }

    bool BollingerBands::has_value(const int column) const {
        return column >= period_ - 1 && column < static_cast<int>(middle_.size());
    }

    bool BollingerBands::is_above_upper(const int column, const double price) const {
        return has_value(column) && price > upper_[column];
    }

    bool BollingerBands::is_below_lower(const int column, const double price) const {
        return has_value(column) && price < lower_[column];
    }

    std::string BollingerBands::to_string() const {
        std::ostringstream oss;
        oss << "Bollinger Bands(" << period_ << ", " << std_devs_ << "): "
            << middle_.size() << " values";
        return oss.str();
    }

    RSI::RSI(const int period) : period_(period) {}

    void RSI::set_period(const int period) {
        period_ = period;
        values_.clear();
    }

    void RSI::set_thresholds(const double overbought, const double oversold) {
        overbought_ = overbought;
        oversold_ = oversold;
    }

    void RSI::calculate(const Chart& chart) {
        values_.clear();
        const size_t count = chart.column_count();
        if (count < 2) return;

        std::vector<double> gains, losses;
        for (size_t i = 1; i < count; i++) {
            const Column* curr = chart.column(i);
            const Column* prev = chart.column(i - 1);
            const double curr_avg = (curr->highest_price() + curr->lowest_price()) / 2.0;
            const double prev_avg = (prev->highest_price() + prev->lowest_price()) / 2.0;
            double change = curr_avg - prev_avg;
            gains.push_back(change > 0 ? change : 0);
            losses.push_back(change < 0 ? -change : 0);
        }

        values_.push_back(50.0);
        for (size_t i = 1; i < count; i++) {
            if (static_cast<int>(i) < period_) {
                values_.push_back(50.0);
                continue;
            }

            double avg_gain = 0, avg_loss = 0;
            for (int j = 0; j < period_; j++) {
                if (const size_t idx = i - 1 - j; idx < gains.size()) {
                    avg_gain += gains[idx];
                    avg_loss += losses[idx];
                }
            }
            avg_gain /= period_;
            avg_loss /= period_;

            if (avg_loss == 0) {
                values_.push_back(100.0);
            } else {
                const double rs = avg_gain / avg_loss;
                values_.push_back(100.0 - (100.0 / (1.0 + rs)));
            }
        }
    }

    double RSI::value(const int column) const {
        return (column >= 0 && column < static_cast<int>(values_.size())) ? values_[column] : 50.0;
    }

    bool RSI::has_value(const int column) const {
        return column >= period_ && column < static_cast<int>(values_.size());
    }

    bool RSI::is_overbought(const int column) const {
        return has_value(column) && values_[column] > overbought_;
    }

    bool RSI::is_oversold(const int column) const {
        return has_value(column) && values_[column] < oversold_;
    }

    bool RSI::is_overbought_custom(const int column, const double threshold) const {
        return has_value(column) && values_[column] > threshold;
    }

    bool RSI::is_oversold_custom(const int column, const double threshold) const {
        return has_value(column) && values_[column] < threshold;
    }

    std::string RSI::to_string() const {
        std::ostringstream oss;
        oss << "RSI(" << period_ << "): " << values_.size() << " values";
        return oss.str();
    }

    void OnBalanceVolume::calculate(const Chart& chart, const std::vector<OHLC>& ohlc_data) {
        values_.clear();
        if (ohlc_data.empty()) return;

        double obv = 0;
        values_.push_back(obv);
        for (size_t i = 1; i < ohlc_data.size() && i < chart.column_count(); i++) {
            if (ohlc_data[i].close > ohlc_data[i-1].close)
                obv += ohlc_data[i].volume;
            else if (ohlc_data[i].close < ohlc_data[i-1].close)
                obv -= ohlc_data[i].volume;
            values_.push_back(obv);
        }
    }

    double OnBalanceVolume::value(const int column) const {
        return (column >= 0 && column < static_cast<int>(values_.size())) ? values_[column] : 0.0;
    }

    bool OnBalanceVolume::has_value(const int column) const {
        return column >= 0 && column < static_cast<int>(values_.size());
    }

    std::string OnBalanceVolume::to_string() const {
        std::ostringstream oss;
        oss << "OBV: " << values_.size() << " values";
        return oss.str();
    }

    void BullishPercent::calculate(const Chart& chart) {
        const size_t count = chart.column_count();
        if (count == 0) {
            value_ = 50.0;
            return;
        }

        size_t bullish = 0;
        for (size_t i = 0; i < count; i++) {
            if (const Column* col = chart.column(i); col && col->type() == ColumnType::X)
                bullish++;
        }
        value_ = (static_cast<double>(bullish) / static_cast<double>(count)) * 100.0;
    }

    void BullishPercent::set_thresholds(const double bullish, const double bearish) {
        bullish_threshold_ = bullish;
        bearish_threshold_ = bearish;
    }

    std::string BullishPercent::to_string() const {
        std::ostringstream oss;
        oss << "Bullish Percent: " << value_ << "%";
        return oss.str();
    }

    bool SignalDetector::is_buy_signal(const Chart& chart, const int column) {
        if (column < 2) return false;
        const Column* curr = chart.column(column);
        if (curr->type() != ColumnType::X) return false;

        int prev_x = -1;
        for (int i = column - 1; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::X) {
                prev_x = i;
                break;
            }
        }
        if (prev_x < 0) return false;
        return curr->highest_price() > chart.column(prev_x)->highest_price();
    }

    bool SignalDetector::is_sell_signal(const Chart& chart, const int column) {
        if (column < 2) return false;
        const Column* curr = chart.column(column);
        if (curr->type() != ColumnType::O) return false;

        int prev_o = -1;
        for (int i = column - 1; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::O) {
                prev_o = i;
                break;
            }
        }
        if (prev_o < 0) return false;
        return curr->lowest_price() < chart.column(prev_o)->lowest_price();
    }

    void SignalDetector::detect(const Chart& chart) {
        signals_.clear();
        current_ = SignalType::None;

        const size_t count = chart.column_count();
        for (size_t i = 0; i < count; i++) {
            if (is_buy_signal(chart, static_cast<int>(i))) {
                const Column* col = chart.column(i);
                signals_.push_back({SignalType::Buy, static_cast<int>(i),
                                   col->highest_price(), std::chrono::system_clock::now()});
                current_ = SignalType::Buy;
            } else if (is_sell_signal(chart, static_cast<int>(i))) {
                const Column* col = chart.column(i);
                signals_.push_back({SignalType::Sell, static_cast<int>(i),
                                   col->lowest_price(), std::chrono::system_clock::now()});
                current_ = SignalType::Sell;
            }
        }
    }

    Signal SignalDetector::last_signal() const {
        if (signals_.empty())
            return {SignalType::None, -1, 0.0, std::chrono::system_clock::now()};
        return signals_.back();
    }

    std::vector<Signal> SignalDetector::buy_signals() const {
        std::vector<Signal> result;
        for (const auto& s : signals_) {
            if (s.type == SignalType::Buy) result.push_back(s);
        }
        return result;
    }

    std::vector<Signal> SignalDetector::sell_signals() const {
        std::vector<Signal> result;
        for (const auto& s : signals_) {
            if (s.type == SignalType::Sell) result.push_back(s);
        }
        return result;
    }

    int SignalDetector::buy_count() const {
        return static_cast<int>(std::ranges::count_if(signals_,
                                                      [](const Signal& s) { return s.type == SignalType::Buy; }));
    }

    int SignalDetector::sell_count() const {
        return static_cast<int>(std::ranges::count_if(signals_,
                                                      [](const Signal& s) { return s.type == SignalType::Sell; }));
    }

    std::string SignalDetector::to_string() const {
        std::ostringstream oss;
        oss << "Signals: " << signals_.size() << " total, Current: "
            << (current_ == SignalType::Buy ? "BUY" :
                current_ == SignalType::Sell ? "SELL" : "NONE");
        return oss.str();
    }

    bool PatternRecognizer::detect_double_top_breakout(const Chart& chart, const int col) {
        if (col < 2) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::X) return false;

        int prev_x = -1;
        for (int i = col - 1; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::X) { prev_x = i; break; }
        }
        if (prev_x < 0) return false;

        const double curr_high = curr->highest_price();
        if (const double prev_high = chart.column(prev_x)->highest_price(); curr_high > prev_high) {
            patterns_.push_back({PatternType::DoubleTopBreakout, prev_x, col, curr_high});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_double_bottom_breakdown(const Chart& chart, const int col) {
        if (col < 2) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::O) return false;

        int prev_o = -1;
        for (int i = col - 1; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::O) { prev_o = i; break; }
        }
        if (prev_o < 0) return false;

        const double curr_low = curr->lowest_price();
        if (const double prev_low = chart.column(prev_o)->lowest_price(); curr_low < prev_low) {
            patterns_.push_back({PatternType::DoubleBottomBreakdown, prev_o, col, curr_low});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_triple_top_breakout(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (const Column* curr = chart.column(col); curr->type() != ColumnType::X) return false;

        std::vector<int> x_indices;
        for (int i = col; i >= 0 && x_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::X) x_indices.push_back(i);
        }
        if (x_indices.size() < 3) return false;

        const double h0 = chart.column(x_indices[0])->highest_price();
        const double h1 = chart.column(x_indices[1])->highest_price();
        const double h2 = chart.column(x_indices[2])->highest_price();

        if (std::abs(h1 - h2) < 0.0001 && h0 > h1) {
            patterns_.push_back({PatternType::TripleTopBreakout, x_indices[2], col, h0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_triple_bottom_breakdown(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (const Column* curr = chart.column(col); curr->type() != ColumnType::O) return false;

        std::vector<int> o_indices;
        for (int i = col; i >= 0 && o_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::O) o_indices.push_back(i);
        }
        if (o_indices.size() < 3) return false;

        const double l0 = chart.column(o_indices[0])->lowest_price();
        const double l1 = chart.column(o_indices[1])->lowest_price();

        if (const double l2 = chart.column(o_indices[2])->lowest_price(); std::abs(l1 - l2) < 0.0001 && l0 < l1) {
            patterns_.push_back({PatternType::TripleBottomBreakdown, o_indices[2], col, l0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_quadruple_top_breakout(const Chart& chart, const int col) {
        if (col < 6) return false;
        if (chart.column(col)->type() != ColumnType::X) return false;

        std::vector<int> x_indices;
        for (int i = col; i >= 0 && x_indices.size() < 4; i--) {
            if (chart.column(i)->type() == ColumnType::X) x_indices.push_back(i);
        }
        if (x_indices.size() < 4) return false;

        const double h0 = chart.column(x_indices[0])->highest_price();
        const double h1 = chart.column(x_indices[1])->highest_price();
        const double h2 = chart.column(x_indices[2])->highest_price();

        if (const double h3 = chart.column(x_indices[3])->highest_price(); std::abs(h1-h2) < 0.0001 && std::abs(h2-h3) < 0.0001 && h0 > h1) {
            patterns_.push_back({PatternType::QuadrupleTopBreakout, x_indices[3], col, h0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_quadruple_bottom_breakdown(const Chart& chart, const int col) {
        if (col < 6) return false;
        if (chart.column(col)->type() != ColumnType::O) return false;

        std::vector<int> o_indices;
        for (int i = col; i >= 0 && o_indices.size() < 4; i--) {
            if (chart.column(i)->type() == ColumnType::O) o_indices.push_back(i);
        }
        if (o_indices.size() < 4) return false;

        const double l0 = chart.column(o_indices[0])->lowest_price();
        const double l1 = chart.column(o_indices[1])->lowest_price();
        const double l2 = chart.column(o_indices[2])->lowest_price();

        if (const double l3 = chart.column(o_indices[3])->lowest_price(); std::abs(l1-l2) < 0.0001 && std::abs(l2-l3) < 0.0001 && l0 < l1) {
            patterns_.push_back({PatternType::QuadrupleBottomBreakdown, o_indices[3], col, l0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_ascending_triple_top(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (chart.column(col)->type() != ColumnType::X) return false;

        std::vector<int> x_indices;
        for (int i = col; i >= 0 && x_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::X) x_indices.push_back(i);
        }
        if (x_indices.size() < 3) return false;

        const double h0 = chart.column(x_indices[0])->highest_price();
        const double h1 = chart.column(x_indices[1])->highest_price();

        if (const double h2 = chart.column(x_indices[2])->highest_price(); h0 > h1 && h1 > h2) {
            patterns_.push_back({PatternType::AscendingTripleTop, x_indices[2], col, h0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_descending_triple_bottom(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (chart.column(col)->type() != ColumnType::O) return false;

        std::vector<int> o_indices;
        for (int i = col; i >= 0 && o_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::O) o_indices.push_back(i);
        }
        if (o_indices.size() < 3) return false;

        const double l0 = chart.column(o_indices[0])->lowest_price();
        const double l1 = chart.column(o_indices[1])->lowest_price();

        if (const double l2 = chart.column(o_indices[2])->lowest_price(); l0 < l1 && l1 < l2) {
            patterns_.push_back({PatternType::DescendingTripleBottom, o_indices[2], col, l0});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_bullish_catapult(const int col) {
        if (patterns_.size() < 2) return false;
        if (const Pattern& last = patterns_[patterns_.size() - 1]; last.type == PatternType::DoubleTopBreakout && last.end_column == col) {
            if (const Pattern& second_last = patterns_[patterns_.size() - 2]; second_last.type == PatternType::TripleTopBreakout) {
                patterns_.push_back({PatternType::BullishCatapult,
                                   second_last.start_column, col, last.price});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bearish_catapult(const int col) {
        if (patterns_.size() < 2) return false;
        if (const Pattern& last = patterns_[patterns_.size() - 1]; last.type == PatternType::DoubleBottomBreakdown && last.end_column == col) {
            if (const Pattern& second_last = patterns_[patterns_.size() - 2]; second_last.type == PatternType::TripleBottomBreakdown) {
                patterns_.push_back({PatternType::BearishCatapult,
                                   second_last.start_column, col, last.price});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bullish_signal_reversed(const Chart& chart, const int col) {
        if (col < 6) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::O) return false;

        bool rising = true;
        for (int i = col - 1; i >= col - 5 && i >= 0; i--) {
            const Column* c1 = chart.column(i);
            if (const Column* c2 = chart.column(i + 1); c1->highest_price() >= c2->highest_price() ||
                c1->lowest_price() >= c2->lowest_price()) {
                rising = false;
                break;
                }
        }

        if (rising) {
            int prev_o = -1;
            for (int i = col - 1; i >= 0; i--) {
                if (chart.column(i)->type() == ColumnType::O) { prev_o = i; break; }
            }
            if (prev_o >= 0 && curr->lowest_price() < chart.column(prev_o)->lowest_price()) {
                patterns_.push_back({PatternType::BullishSignalReversed, col - 5, col,
                                   curr->lowest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bearish_signal_reversed(const Chart& chart, const int col) {
        if (col < 6) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::X) return false;

        bool falling = true;
        for (int i = col - 1; i >= col - 5 && i >= 0; i--) {
            const Column* c1 = chart.column(i);
            if (const Column* c2 = chart.column(i + 1); c1->highest_price() <= c2->highest_price() ||
                c1->lowest_price() <= c2->lowest_price()) {
                falling = false;
                break;
                }
        }

        if (falling) {
            int prev_x = -1;
            for (int i = col - 1; i >= 0; i--) {
                if (chart.column(i)->type() == ColumnType::X) { prev_x = i; break; }
            }
            if (prev_x >= 0 && curr->highest_price() > chart.column(prev_x)->highest_price()) {
                patterns_.push_back({PatternType::BearishSignalReversed, col - 5, col,
                                   curr->highest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bullish_triangle(const Chart& chart, const int col) {
        if (col < 6) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::X) return false;

        bool rising_bottoms = true, falling_tops = true;
        for (int i = col - 1; i >= col - 5 && i >= 0; i--) {
            const Column* c1 = chart.column(i);
            const Column* c2 = chart.column(i + 1);
            if (c1->lowest_price() >= c2->lowest_price()) rising_bottoms = false;
            if (c1->highest_price() <= c2->highest_price()) falling_tops = false;
        }

        if (rising_bottoms && falling_tops) {
            int prev_x = -1;
            for (int i = col - 1; i >= 0; i--) {
                if (chart.column(i)->type() == ColumnType::X) { prev_x = i; break; }
            }
            if (prev_x >= 0 && curr->highest_price() > chart.column(prev_x)->highest_price()) {
                patterns_.push_back({PatternType::BullishTriangle, col - 5, col,
                                   curr->highest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bearish_triangle(const Chart& chart, const int col) {
        if (col < 6) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::O) return false;

        bool rising_bottoms = true, falling_tops = true;
        for (int i = col - 1; i >= col - 5 && i >= 0; i--) {
            const Column* c1 = chart.column(i);
            const Column* c2 = chart.column(i + 1);
            if (c1->lowest_price() >= c2->lowest_price()) rising_bottoms = false;
            if (c1->highest_price() <= c2->highest_price()) falling_tops = false;
        }

        if (rising_bottoms && falling_tops) {
            int prev_o = -1;
            for (int i = col - 1; i >= 0; i--) {
                if (chart.column(i)->type() == ColumnType::O) { prev_o = i; break; }
            }
            if (prev_o >= 0 && curr->lowest_price() < chart.column(prev_o)->lowest_price()) {
                patterns_.push_back({PatternType::BearishTriangle, col - 5, col,
                                   curr->lowest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_long_tail_down(const Chart& chart, const int col) {
        if (col < 1) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::X) return false;

        if (const Column* prev = chart.column(col - 1); prev->type() != ColumnType::O || prev->box_count() < 20) return false;

        patterns_.push_back({PatternType::LongTailDown, col - 1, col, curr->highest_price()});
        return true;
    }

    bool PatternRecognizer::detect_high_pole(const Chart& chart, const int col) {
        if (col < 2) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::O) return false;

        const Column* prev_x = chart.column(col - 1);
        if (prev_x->type() != ColumnType::X || prev_x->box_count() < 2) return false;

        double prev_high = 0;
        for (int i = col - 2; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::X) {
                prev_high = chart.column(i)->highest_price();
                break;
            }
        }

        if (prev_high > 0) {
            const double rise = prev_x->highest_price() - prev_high;
            const double box_size = std::abs(prev_x->get_box_at(prev_x->box_count()-1)->price() -
                                       prev_x->get_box_at(prev_x->box_count()-2)->price());
            if (const double retracement = prev_x->highest_price() - curr->lowest_price(); rise >= 3 * box_size && retracement >= rise * 0.5) {
                patterns_.push_back({PatternType::HighPole, col - 1, col,
                                   prev_x->highest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_low_pole(const Chart& chart, const int col) {
        if (col < 2) return false;
        const Column* curr = chart.column(col);
        if (curr->type() != ColumnType::X) return false;

        const Column* prev_o = chart.column(col - 1);
        if (prev_o->type() != ColumnType::O || prev_o->box_count() < 2) return false;

        double prev_low = 0;
        for (int i = col - 2; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::O) {
                prev_low = chart.column(i)->lowest_price();
                break;
            }
        }

        if (prev_low > 0) {
            const double fall = prev_low - prev_o->lowest_price();
            const double box_size = std::abs(prev_o->get_box_at(prev_o->box_count()-2)->price() -
                                       prev_o->get_box_at(prev_o->box_count()-1)->price());
            if (const double retracement = curr->highest_price() - prev_o->lowest_price(); fall >= 3 * box_size && retracement >= fall * 0.5) {
                patterns_.push_back({PatternType::LowPole, col - 1, col,
                                   prev_o->lowest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bull_trap(const Chart& chart, const int col) {
        if (col < 2) return false;
        if (chart.column(col)->type() != ColumnType::O) return false;

        const Column* prev = chart.column(col - 1);
        if (prev->type() != ColumnType::X || prev->box_count() != 1) return false;

        std::vector<int> x_indices;
        for (int i = col - 2; i >= 0 && x_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::X) x_indices.push_back(i);
        }

        if (x_indices.size() >= 2) {
            const double h0 = chart.column(x_indices[0])->highest_price();
            if (const double h1 = chart.column(x_indices[1])->highest_price(); std::abs(h0 - h1) < 0.0001 && prev->highest_price() > h0) {
                patterns_.push_back({PatternType::BullTrap, x_indices[1], col,
                                   prev->highest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_bear_trap(const Chart& chart, const int col) {
        if (col < 2) return false;
        if (chart.column(col)->type() != ColumnType::X) return false;

        const Column* prev = chart.column(col - 1);
        if (prev->type() != ColumnType::O || prev->box_count() != 1) return false;

        std::vector<int> o_indices;
        for (int i = col - 2; i >= 0 && o_indices.size() < 3; i--) {
            if (chart.column(i)->type() == ColumnType::O) o_indices.push_back(i);
        }

        if (o_indices.size() >= 2) {
            const double l0 = chart.column(o_indices[0])->lowest_price();
            if (const double l1 = chart.column(o_indices[1])->lowest_price(); std::abs(l0 - l1) < 0.0001 && prev->lowest_price() < l0) {
                patterns_.push_back({PatternType::BearTrap, o_indices[1], col,
                                   prev->lowest_price()});
                return true;
            }
        }
        return false;
    }

    bool PatternRecognizer::detect_spread_triple_top(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (chart.column(col)->type() != ColumnType::X) return false;

        std::vector<int> x_indices;
        for (int i = col; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::X) x_indices.push_back(i);
        }
        if (x_indices.size() < 3) return false;

        const double curr_high = chart.column(x_indices[0])->highest_price();
        int match_count = 0;
        for (size_t i = 1; i < x_indices.size() && match_count < 2; i++) {
            if (const double high = chart.column(x_indices[i])->highest_price(); std::abs(high - curr_high) < 0.0001) match_count++;
        }

        if (match_count >= 2) {
            patterns_.push_back({PatternType::SpreadTripleTop, x_indices[2], col, curr_high});
            return true;
        }
        return false;
    }

    bool PatternRecognizer::detect_spread_triple_bottom(const Chart& chart, const int col) {
        if (col < 4) return false;
        if (chart.column(col)->type() != ColumnType::O) return false;

        std::vector<int> o_indices;
        for (int i = col; i >= 0; i--) {
            if (chart.column(i)->type() == ColumnType::O) o_indices.push_back(i);
        }
        if (o_indices.size() < 3) return false;

        const double curr_low = chart.column(o_indices[0])->lowest_price();
        int match_count = 0;
        for (size_t i = 1; i < o_indices.size() && match_count < 2; i++) {
            if (const double low = chart.column(o_indices[i])->lowest_price(); std::abs(low - curr_low) < 0.0001) match_count++;
        }

        if (match_count >= 2) {
            patterns_.push_back({PatternType::SpreadTripleBottom, o_indices[2], col, curr_low});
            return true;
        }
        return false;
    }

    void PatternRecognizer::detect(const Chart& chart) {
        patterns_.clear();
        const size_t count = chart.column_count();
        for (size_t i = 0; i < count; i++) {
            const int col = static_cast<int>(i);
            detect_double_top_breakout(chart, col);
            detect_double_bottom_breakdown(chart, col);
            detect_triple_top_breakout(chart, col);
            detect_triple_bottom_breakdown(chart, col);
            detect_quadruple_top_breakout(chart, col);
            detect_quadruple_bottom_breakdown(chart, col);
            detect_ascending_triple_top(chart, col);
            detect_descending_triple_bottom(chart, col);
            detect_bullish_catapult(col);
            detect_bearish_catapult(col);
            detect_bullish_signal_reversed(chart, col);
            detect_bearish_signal_reversed(chart, col);
            detect_bullish_triangle(chart, col);
            detect_bearish_triangle(chart, col);
            detect_long_tail_down(chart, col);
            detect_high_pole(chart, col);
            detect_low_pole(chart, col);
            detect_bull_trap(chart, col);
            detect_bear_trap(chart, col);
            detect_spread_triple_top(chart, col);
            detect_spread_triple_bottom(chart, col);
        }
    }

    std::vector<Pattern> PatternRecognizer::bullish_patterns() const {
        std::vector<Pattern> result;
        for (const auto& p : patterns_) {
            if (is_bullish_pattern(p.type)) result.push_back(p);
        }
        return result;
    }

    std::vector<Pattern> PatternRecognizer::bearish_patterns() const {
        std::vector<Pattern> result;
        for (const auto& p : patterns_) {
            if (is_bearish_pattern(p.type)) result.push_back(p);
        }
        return result;
    }

    std::vector<Pattern> PatternRecognizer::patterns_of_type(PatternType type) const {
        std::vector<Pattern> result;
        for (const auto& p : patterns_) {
            if (p.type == type) result.push_back(p);
        }
        return result;
    }

    int PatternRecognizer::bullish_count() const {
        return static_cast<int>(std::ranges::count_if(patterns_,
                                                      [](const Pattern& p) { return is_bullish_pattern(p.type); }));
    }

    int PatternRecognizer::bearish_count() const {
        return static_cast<int>(std::ranges::count_if(patterns_,
                                                      [](const Pattern& p) { return is_bearish_pattern(p.type); }));
    }

    Pattern PatternRecognizer::latest_pattern() const {
        if (patterns_.empty()) return {PatternType::None, -1, -1, 0.0};
        return patterns_.back();
    }

    bool PatternRecognizer::has_pattern(PatternType type) const {
        return std::ranges::any_of(patterns_,
                                   [type](const Pattern& p) { return p.type == type; });
    }

    std::string PatternRecognizer::to_string() const {
        std::ostringstream oss;
        oss << "Pattern Recognizer: " << patterns_.size() << " patterns detected\n";
        for (const auto& p : patterns_) {
            oss << "  " << pattern_type_to_string(p.type) << ": cols "
                << p.start_column << "-" << p.end_column << ", price " << p.price << "\n";
        }
        return oss.str();
    }

    SupportResistance::SupportResistance(const double threshold) : threshold_(threshold) {}

    void SupportResistance::set_threshold(const double threshold) {
        threshold_ = threshold;
    }

    void SupportResistance::identify(const Chart& chart) {
        levels_.clear();
        const size_t count = chart.column_count();

        for (size_t i = 0; i < count; i++) {
            if (const Column* col = chart.column(i); col->type() == ColumnType::O) {
                const double low = col->lowest_price();
                bool found = false;
                for (auto& level : levels_) {
                    if (level.is_support && std::abs(level.price - low) / low < threshold_) {
                        level.touch_count++;
                        level.last_column = static_cast<int>(i);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    levels_.push_back({low, 1, true, static_cast<int>(i), static_cast<int>(i)});
                }
            } else if (col->type() == ColumnType::X) {
                const double high = col->highest_price();
                bool found = false;
                for (auto& level : levels_) {
                    if (!level.is_support && std::abs(level.price - high) / high < threshold_) {
                        level.touch_count++;
                        level.last_column = static_cast<int>(i);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    levels_.push_back({high, 1, false, static_cast<int>(i), static_cast<int>(i)});
                }
            }
        }
        merge_similar_levels();
    }

    void SupportResistance::merge_similar_levels() {
        for (size_t i = 0; i < levels_.size(); i++) {
            for (size_t j = i + 1; j < levels_.size();) {
                if (levels_[i].is_support == levels_[j].is_support &&
                    std::abs(levels_[i].price - levels_[j].price) / levels_[i].price < threshold_) {
                    levels_[i].touch_count += levels_[j].touch_count;
                    levels_[i].price = (levels_[i].price * levels_[i].touch_count +
                                        levels_[j].price * levels_[j].touch_count) /
                                       (levels_[i].touch_count + levels_[j].touch_count);
                    levels_[i].last_column = std::max(levels_[i].last_column, levels_[j].last_column);
                    levels_.erase(levels_.begin() + static_cast<long long>(j));
                    } else {
                        j++;
                    }
            }
        }
    }

    std::vector<SupportResistanceLevel> SupportResistance::support_levels() const {
        std::vector<SupportResistanceLevel> result;
        for (const auto& l : levels_) {
            if (l.is_support) result.push_back(l);
        }
        return result;
    }

    std::vector<SupportResistanceLevel> SupportResistance::resistance_levels() const {
        std::vector<SupportResistanceLevel> result;
        for (const auto& l : levels_) {
            if (!l.is_support) result.push_back(l);
        }
        return result;
    }

    std::vector<SupportResistanceLevel> SupportResistance::significant_levels(const int min_touches) const {
        std::vector<SupportResistanceLevel> result;
        for (const auto& l : levels_) {
            if (l.touch_count >= min_touches) result.push_back(l);
        }
        return result;
    }

    std::vector<double> SupportResistance::support_prices() const {
        std::vector<double> result;
        for (const auto& l : levels_) {
            if (l.is_support) result.push_back(l.price);
        }
        return result;
    }

    std::vector<double> SupportResistance::resistance_prices() const {
        std::vector<double> result;
        for (const auto& l : levels_) {
            if (!l.is_support) result.push_back(l.price);
        }
        return result;
    }

    bool SupportResistance::is_near_support(double price, double tolerance) const {
        return std::ranges::any_of(levels_,
                                   [price, tolerance](const SupportResistanceLevel& l) {
                                       return l.is_support && std::abs(price - l.price) / l.price < tolerance;
                                   });
    }

    bool SupportResistance::is_near_resistance(double price, double tolerance) const {
        return std::ranges::any_of(levels_,
                                   [price, tolerance](const SupportResistanceLevel& l) {
                                       return !l.is_support && std::abs(price - l.price) / l.price < tolerance;
                                   });
    }

    std::string SupportResistance::to_string() const {
        std::ostringstream oss;
        oss << "Support/Resistance: " << levels_.size() << " levels\n";
        oss << "Support Levels:\n";
        for (const auto& l : levels_) {
            if (l.is_support) oss << "  Price: " << l.price << ", Touches: " << l.touch_count << "\n";
        }
        oss << "Resistance Levels:\n";
        for (const auto& l : levels_) {
            if (!l.is_support) oss << "  Price: " << l.price << ", Touches: " << l.touch_count << "\n";
        }
        return oss.str();
    }

    void PriceObjectiveCalculator::calculate_vertical_count(const Chart& chart, const int column) {
        if (column < 2) return;
        const Column* curr = chart.column(column);
        const size_t box_count = curr->box_count();
        if (box_count < 2) return;

        const double box_size = (curr->type() == ColumnType::X) ?
            std::abs(curr->get_box_at(box_count-1)->price() - curr->get_box_at(box_count-2)->price()) :
            std::abs(curr->get_box_at(box_count-2)->price() - curr->get_box_at(box_count-1)->price());

        if (curr->type() == ColumnType::X) {
            const double target = curr->highest_price() + (static_cast<double>(box_count) * box_size);
            objectives_.push_back({target, column, static_cast<int>(box_count), true});
        } else if (curr->type() == ColumnType::O) {
            const double target = curr->lowest_price() - (static_cast<double>(box_count) * box_size);
            objectives_.push_back({target, column, static_cast<int>(box_count), false});
        }
    }

    void PriceObjectiveCalculator::calculate_all(const Chart& chart) {
        objectives_.clear();
        const size_t count = chart.column_count();
        for (size_t i = 0; i < count; i++) {
            calculate_vertical_count(chart, static_cast<int>(i));
        }
    }

    PriceObjective PriceObjectiveCalculator::latest() const {
        if (objectives_.empty()) return {0.0, -1, 0, true};
        return objectives_.back();
    }

    std::vector<PriceObjective> PriceObjectiveCalculator::bullish_objectives() const {
        std::vector<PriceObjective> result;
        for (const auto& obj : objectives_) {
            if (obj.is_bullish) result.push_back(obj);
        }
        return result;
    }

    std::vector<PriceObjective> PriceObjectiveCalculator::bearish_objectives() const {
        std::vector<PriceObjective> result;
        for (const auto& obj : objectives_) {
            if (!obj.is_bullish) result.push_back(obj);
        }
        return result;
    }

    std::vector<double> PriceObjectiveCalculator::bullish_targets() const {
        std::vector<double> result;
        for (const auto& obj : objectives_) {
            if (obj.is_bullish) result.push_back(obj.target_price);
        }
        return result;
    }

    std::vector<double> PriceObjectiveCalculator::bearish_targets() const {
        std::vector<double> result;
        for (const auto& obj : objectives_) {
            if (!obj.is_bullish) result.push_back(obj.target_price);
        }
        return result;
    }

    std::string PriceObjectiveCalculator::to_string() const {
        std::ostringstream oss;
        oss << "Price Objectives: " << objectives_.size() << " calculated\n";
        for (const auto& obj : objectives_) {
            oss << "  " << (obj.is_bullish ? "Bullish" : "Bearish")
                << " Target: " << obj.target_price << " (" << obj.box_count << " boxes)\n";
        }
        return oss.str();
    }

    CongestionDetector::CongestionDetector(const int min_columns, const double price_range_threshold)
        : min_columns_(min_columns), threshold_(price_range_threshold) {}

    void CongestionDetector::set_min_columns(const int min) {
        min_columns_ = min;
    }

    void CongestionDetector::set_threshold(const double threshold) {
        threshold_ = threshold;
    }

    void CongestionDetector::detect(const Chart& chart) {
        zones_.clear();
        const size_t count = chart.column_count();
        if (count < static_cast<size_t>(min_columns_)) return;

        size_t start = 0;
        while (start < count) {
            double high = chart.column(start)->highest_price();
            double low = chart.column(start)->lowest_price();
            size_t end = start;

            for (size_t i = start + 1; i < count; i++) {
                double col_high = chart.column(i)->highest_price();
                double col_low = chart.column(i)->lowest_price();

                const double range_high = std::max(high, col_high);
                const double range_low = std::min(low, col_low);

                if (const double range = (range_high - range_low) / range_low; range < threshold_) {
                    high = range_high;
                    low = range_low;
                    end = i;
                } else {
                    break;
                }
            }

            if (static_cast<int>(end - start + 1) >= min_columns_) {
                zones_.push_back({static_cast<int>(start), static_cast<int>(end),
                                 high, low, static_cast<int>(end - start + 1)});
            }
            start = end + 1;
        }
    }

    bool CongestionDetector::is_in_congestion(int column) const {
        return std::ranges::any_of(zones_,
                                   [column](const CongestionZone& z) {
                                       return column >= z.start_column && column <= z.end_column;
                                   });
    }

    CongestionDetector::CongestionZone CongestionDetector::largest_zone() const {
        if (zones_.empty()) return {-1, -1, 0.0, 0.0, 0};
        return *std::ranges::max_element(zones_,
                                         [](const CongestionZone& a, const CongestionZone& b) {
                                             return a.column_count < b.column_count;
                                         });
    }

    std::string CongestionDetector::to_string() const {
        std::ostringstream oss;
        oss << "Congestion: " << zones_.size() << " zones detected\n";
        for (const auto& z : zones_) {
            oss << "  Cols " << z.start_column << "-" << z.end_column
                << ", Range: " << z.low_price << "-" << z.high_price << "\n";
        }
        return oss.str();
    }

    Indicators::Indicators() {
        initialize();
    }

    Indicators::Indicators(const IndicatorConfig& config) : config_(config) {
        initialize();
    }

    void Indicators::initialize() {
        sma_short_ = std::make_unique<MovingAverage>(config_.sma_short_period);
        sma_medium_ = std::make_unique<MovingAverage>(config_.sma_medium_period);
        sma_long_ = std::make_unique<MovingAverage>(config_.sma_long_period);
        bollinger_ = std::make_unique<BollingerBands>(config_.bollinger_period, config_.bollinger_std_devs);
        rsi_ = std::make_unique<RSI>(config_.rsi_period);
        rsi_->set_thresholds(config_.rsi_overbought, config_.rsi_oversold);
        obv_ = std::make_unique<OnBalanceVolume>();
        bullish_percent_ = std::make_unique<BullishPercent>();
        bullish_percent_->set_thresholds(config_.bullish_alert_threshold, config_.bearish_alert_threshold);
        signals_ = std::make_unique<SignalDetector>();
        patterns_ = std::make_unique<PatternRecognizer>();
        support_resistance_ = std::make_unique<SupportResistance>(config_.support_resistance_threshold);
        objectives_ = std::make_unique<PriceObjectiveCalculator>();
        congestion_ = std::make_unique<CongestionDetector>(config_.congestion_min_columns, config_.congestion_price_range);
    }

    void Indicators::configure(const IndicatorConfig& config) {
        config_ = config;
        sma_short_->set_period(config.sma_short_period);
        sma_medium_->set_period(config.sma_medium_period);
        sma_long_->set_period(config.sma_long_period);
        bollinger_->set_period(config.bollinger_period);
        bollinger_->set_std_devs(config.bollinger_std_devs);
        rsi_->set_period(config.rsi_period);
        rsi_->set_thresholds(config.rsi_overbought, config.rsi_oversold);
        bullish_percent_->set_thresholds(config.bullish_alert_threshold, config.bearish_alert_threshold);
        support_resistance_->set_threshold(config.support_resistance_threshold);
        congestion_->set_min_columns(config.congestion_min_columns);
        congestion_->set_threshold(config.congestion_price_range);
    }

    void Indicators::calculate(const Chart& chart) const
    {
        if (chart.column_count() == 0) return;

        sma_short_->calculate(chart);
        sma_medium_->calculate(chart);
        sma_long_->calculate(chart);
        bollinger_->calculate(chart);
        rsi_->calculate(chart);
        bullish_percent_->calculate(chart);
        signals_->detect(chart);
        patterns_->detect(chart);
        support_resistance_->identify(chart);
        objectives_->calculate_all(chart);
        congestion_->detect(chart);
    }

    void Indicators::calculate_with_volume(const Chart& chart, const std::vector<OHLC>& ohlc_data) const
    {
        calculate(chart);
        obv_->calculate(chart, ohlc_data);
    }

    IndicatorData Indicators::export_data() const {
        IndicatorData data;
        data.sma_short = sma_short_->values_copy();
        data.sma_medium = sma_medium_->values_copy();
        data.sma_long = sma_long_->values_copy();
        data.bollinger_middle = bollinger_->middle_copy();
        data.bollinger_upper = bollinger_->upper_copy();
        data.bollinger_lower = bollinger_->lower_copy();
        data.rsi = rsi_->values_copy();
        data.obv = obv_->values_copy();
        data.bullish_percent = bullish_percent_->value();
        data.signals = signals_->signals_copy();
        data.patterns = patterns_->patterns_copy();
        data.support_levels = support_resistance_->support_levels();
        data.resistance_levels = support_resistance_->resistance_levels();
        data.price_objectives = objectives_->objectives_copy();
        return data;
    }

    ChartData Indicators::export_chart_data(const Chart& chart) {
        ChartData data;
        data.box_size = chart.current_box_size();
        data.reversal = chart.config().reversal;
        data.method = chart.config().method;
        data.prices = chart.all_prices();

        for (size_t i = 0; i < chart.column_count(); i++) {
            const Column* col = chart.column(i);
            ColumnData cd;
            cd.index = static_cast<int>(i);
            cd.type = col->type();
            cd.high = col->highest_price();
            cd.low = col->lowest_price();
            cd.box_count = static_cast<int>(col->box_count());
            if (col->box_count() > 0) {
                if (const Box* first_box = col->get_box_at(0)) cd.marker = first_box->marker();
            }
            data.columns.push_back(cd);
        }
        return data;
    }

    std::string Indicators::summary() const {
        std::ostringstream oss;
        oss << "=== P&F INDICATORS SUMMARY ===\n\n";

        oss << "CURRENT SIGNAL: " << (signals_->has_buy_signal() ? "BUY" :
            signals_->has_sell_signal() ? "SELL" : "NONE") << "\n";

        const Pattern latest = patterns_->latest_pattern();
        oss << "\nLATEST PATTERN: "
            << (latest.type != PatternType::None ? pattern_type_to_string(latest.type) : "None") << "\n";

        oss << "\nBULLISH PATTERNS: " << patterns_->bullish_patterns().size() << "\n";
        oss << "BEARISH PATTERNS: " << patterns_->bearish_patterns().size() << "\n";
        oss << "\nSIGNIFICANT S/R LEVELS: " << support_resistance_->significant_levels(3).size() << "\n";

        if (const PriceObjective obj = objectives_->latest(); obj.base_column >= 0) {
            oss << "\nLATEST PRICE TARGET: " << obj.target_price
                << " (" << (obj.is_bullish ? "Bullish" : "Bearish") << ")\n";
        }

        return oss.str();
    }

    std::string Indicators::to_string() const {
        std::ostringstream oss;
        oss << "=== POINT & FIGURE INDICATORS ===\n\n";
        oss << "MOVING AVERAGES:\n" << sma_short_->to_string() << "\n" << sma_medium_->to_string() << "\n\n";
        oss << "BOLLINGER BANDS:\n" << bollinger_->to_string() << "\n\n";
        oss << "RSI:\n" << rsi_->to_string() << "\n\n";
        oss << "BULLISH PERCENT:\n" << bullish_percent_->to_string() << "\n\n";
        oss << "SIGNALS:\n" << signals_->to_string() << "\n\n";
        oss << "PATTERNS:\n" << patterns_->to_string() << "\n";
        oss << "SUPPORT & RESISTANCE:\n" << support_resistance_->to_string() << "\n";
        oss << "PRICE OBJECTIVES:\n" << objectives_->to_string() << "\n";
        oss << "CONGESTION:\n" << congestion_->to_string() << "\n";
        return oss.str();
    }
}
