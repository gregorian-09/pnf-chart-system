/// \file chart.cpp
/// \brief Chart implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include "pnf/chart.hpp"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cmath>

namespace pnf
{
    Chart::Chart(const ChartConfig& config) : config_(config), last_month_(-1), last_box_size_(config_.box_size) {
        last_time_ = std::chrono::system_clock::now();
        last_processed_time_ = std::chrono::system_clock::now();
        trend_manager_ = std::make_unique<TrendLineManager>(config_.box_size);
    }

    std::string Chart::get_month_marker(const int month) {
        if (month >= 1 && month <= 12)
            return month_markers_[month - 1];
        return "";
    }

    bool Chart::has_month_changed(const Timestamp current, std::string& marker) const
    {
        std::time_t current_tt = std::chrono::system_clock::to_time_t(current);
        std::tm current_tm{};
#ifdef _WIN32
        localtime_s(&current_tm, &current_tt);
#else
        localtime_r(&current_tt, &current_tm);
#endif

        if (last_processed_time_ == Timestamp{}) {
            marker = get_month_marker(current_tm.tm_mon + 1);
            return true;
        }

        std::time_t last_tt = std::chrono::system_clock::to_time_t(last_processed_time_);
        std::tm last_tm{};
#ifdef _WIN32
        localtime_s(&last_tm, &last_tt);
#else
        localtime_r(&last_tt, &last_tm);
#endif

        if (current_tm.tm_year != last_tm.tm_year || current_tm.tm_mon != last_tm.tm_mon) {
            marker = get_month_marker(current_tm.tm_mon + 1);
            return true;
        }

        marker = "";
        return false;
    }

    std::string Chart::get_month_marker_for_data(const Timestamp time) const
    {
        if (std::string marker; has_month_changed(time, marker))
            return marker;
        return "";
    }

    double Chart::calculate_box_size(const double price) {
        double box = 0.0;
        switch (config_.box_size_method) {
        case BoxSizeMethod::Fixed:
        case BoxSizeMethod::Points:
            box = config_.box_size;
            break;
        case BoxSizeMethod::Percentage:
            box = price * config_.box_size / 100.0;
            break;
        case BoxSizeMethod::Traditional:
        default:
            if (price < 0.25) { config_.box_size = 0.0625; box = config_.box_size; break; }
            if (price < 1.0) { config_.box_size = 0.125; box = config_.box_size; break; }
            if (price < 5.0) { config_.box_size = 0.25; box = config_.box_size; break; }
            if (price < 20.0) { config_.box_size = 0.5; box = config_.box_size; break; }
            if (price < 100.0) { config_.box_size = 1.0; box = config_.box_size; break; }
            if (price < 200.0) { config_.box_size = 2.0; box = config_.box_size; break; }
            if (price < 500.0) { config_.box_size = 4.0; box = config_.box_size; break; }
            if (price < 1000.0) { config_.box_size = 5.0; box = config_.box_size; break; }
            if (price < 25000.0) { config_.box_size = 50.0; box = config_.box_size; break; }
            config_.box_size = 500.0;
            box = config_.box_size;
            break;
        }
        last_box_size_ = box;
        if (trend_manager_) {
            trend_manager_->set_box_size(box);
        }
        return box;
    }

    double Chart::round_to_box_size(const double price, const bool round_up) {
        const double box = calculate_box_size(price);
        if (round_up)
            return std::ceil(price / box) * box;
        return std::floor(price / box) * box;
    }

    bool Chart::is_reversal(const double price, const Column* current, BoxType& new_type) {
        if (!current || current->box_count() == 0) return false;

        const double box = calculate_box_size(price);
        const double highest = current->highest_price();
        const double lowest = current->lowest_price();

        if (const ColumnType col_type = current->type(); col_type == ColumnType::X) {
            if (const double reversal_level = highest - (config_.reversal * box); price <= reversal_level) {
                new_type = BoxType::O;
                return true;
            }
        } else if (col_type == ColumnType::O) {
            if (const double reversal_level = lowest + (config_.reversal * box); price >= reversal_level) {
                new_type = BoxType::X;
                return true;
            }
        } else if (col_type == ColumnType::Mixed && config_.reversal == 1) {
            if (price > highest + box) {
                new_type = BoxType::X;
                return true;
            }
            if (price < lowest - box) {
                new_type = BoxType::O;
                return true;
            }
        }
        return false;
    }

    bool Chart::process_high_low(const double high, const double low, const Timestamp time)
    {
        last_time_ = time;
        bool changed = false;
        const std::string month_marker = get_month_marker_for_data(time);
        const bool month_changed = !month_marker.empty();
        Column* last = last_column();
        const double box = calculate_box_size(high);

        if (!last) {
            auto col = std::make_unique<Column>(ColumnType::X);
            const double start_price = round_to_box_size(high, false);
            if (month_changed)
                col->add_box(start_price, BoxType::X, month_marker);
            else
                col->add_box(start_price, BoxType::X);
            columns_.push_back(std::move(col));
            last_processed_time_ = time;
            return true;
        }

        BoxType reversal_type;
        const bool reversal_high = is_reversal(high, last, reversal_type);

        if (const bool reversal_low = is_reversal(low, last, reversal_type); reversal_high || reversal_low) {
            changed = true;
            const double reversal_price = reversal_high ? high : low;
            ColumnType new_col_type = (reversal_type == BoxType::X) ? ColumnType::X : ColumnType::O;
            if (config_.reversal == 1) new_col_type = ColumnType::Mixed;
            auto col = std::make_unique<Column>(new_col_type);
            if (reversal_type == BoxType::X) {
                double current_price = last->lowest_price() + box;
                bool marker_applied = false;
                while (current_price <= round_to_box_size(reversal_price, true)) {
                    if (month_changed && !marker_applied) {
                        col->add_box(current_price, BoxType::X, month_marker);
                        marker_applied = true;
                    } else {
                        col->add_box(current_price, BoxType::X);
                    }
                    current_price += box;
                }
            } else {
                double current_price = last->highest_price() - box;
                bool marker_applied = false;
                while (current_price >= round_to_box_size(reversal_price, false)) {
                    if (month_changed && !marker_applied) {
                        col->add_box(current_price, BoxType::O, month_marker);
                        marker_applied = true;
                    } else {
                        col->add_box(current_price, BoxType::O);
                    }
                    current_price -= box;
                }
            }
            columns_.push_back(std::move(col));
            if (trend_manager_)
                trend_manager_->update(columns_, static_cast<int>(columns_.size()) - 1);
        } else {
            const ColumnType col_type = last->type();
            if (col_type == ColumnType::X && high > last->highest_price()) {
                changed = true;
                double current_price = last->highest_price() + box;
                bool marker_applied = false;
                while (current_price <= round_to_box_size(high, true)) {
                    if (month_changed && !marker_applied) {
                        last->add_box(current_price, BoxType::X, month_marker);
                        marker_applied = true;
                    } else {
                        last->add_box(current_price, BoxType::X);
                    }
                    current_price += box;
                }
            } else if (col_type == ColumnType::O && low < last->lowest_price()) {
                changed = true;
                double current_price = last->lowest_price() - box;
                bool marker_applied = false;
                while (current_price >= round_to_box_size(low, false)) {
                    if (month_changed && !marker_applied) {
                        last->add_box(current_price, BoxType::O, month_marker);
                        marker_applied = true;
                    } else {
                        last->add_box(current_price, BoxType::O);
                    }
                    current_price -= box;
                }
            }
        }

        last_processed_time_ = time;
        return changed;
    }

    bool Chart::process_close(const double close, const Timestamp time)
    {
        last_time_ = time;
        bool changed = false;
        const std::string month_marker = get_month_marker_for_data(time);
        const bool month_changed = !month_marker.empty();
        Column* last = last_column();
        const double box = calculate_box_size(close);

        if (!last) {
            auto col = std::make_unique<Column>(ColumnType::X);
            const double start_price = round_to_box_size(close, false);
            if (month_changed)
                col->add_box(start_price, BoxType::X, month_marker);
            else
                col->add_box(start_price, BoxType::X);
            columns_.push_back(std::move(col));
            last_processed_time_ = time;
            return true;
        }

        if (BoxType reversal_type; is_reversal(close, last, reversal_type)) {
            changed = true;
            ColumnType new_col_type = (reversal_type == BoxType::X) ? ColumnType::X : ColumnType::O;
            if (config_.reversal == 1) new_col_type = ColumnType::Mixed;
            auto col = std::make_unique<Column>(new_col_type);

            if (reversal_type == BoxType::X) {
                double current_price = last->lowest_price() + box;
                bool marker_applied = false;
                while (current_price <= round_to_box_size(close, true)) {
                    if (month_changed && !marker_applied) {
                        col->add_box(current_price, BoxType::X, month_marker);
                        marker_applied = true;
                    } else {
                        col->add_box(current_price, BoxType::X);
                    }
                    current_price += box;
                }
            } else {
                double current_price = last->highest_price() - box;
                bool marker_applied = false;
                while (current_price >= round_to_box_size(close, false)) {
                    if (month_changed && !marker_applied) {
                        col->add_box(current_price, BoxType::O, month_marker);
                        marker_applied = true;
                    } else {
                        col->add_box(current_price, BoxType::O);
                    }
                    current_price -= box;
                }
            }

            columns_.push_back(std::move(col));
            if (trend_manager_)
                trend_manager_->update(columns_, static_cast<int>(columns_.size()) - 1);

        } else {
            if (const ColumnType col_type = last->type(); (col_type == ColumnType::X || col_type == ColumnType::Mixed) && close > last->highest_price()) {
                changed = true;
                double current_price = last->highest_price() + box;
                bool marker_applied = false;
                while (current_price <= round_to_box_size(close, true)) {
                    const BoxType bt = (col_type == ColumnType::Mixed) ?
                        ((current_price > last->lowest_price()) ? BoxType::X : BoxType::O) : BoxType::X;
                    if (month_changed && !marker_applied) {
                        last->add_box(current_price, bt, month_marker);
                        marker_applied = true;
                    } else {
                        last->add_box(current_price, bt);
                    }
                    current_price += box;
                }

            } else if ((col_type == ColumnType::O || col_type == ColumnType::Mixed) && close < last->lowest_price()) {
                changed = true;
                double current_price = last->lowest_price() - box;
                bool marker_applied = false;
                while (current_price >= round_to_box_size(close, false)) {
                    const BoxType bt = (col_type == ColumnType::Mixed) ?
                        ((current_price < last->highest_price()) ? BoxType::O : BoxType::X) : BoxType::O;
                    if (month_changed && !marker_applied) {
                        last->add_box(current_price, bt, month_marker);
                        marker_applied = true;
                    } else {
                        last->add_box(current_price, bt);
                    }
                    current_price -= box;
                }
            }
        }

        last_processed_time_ = time;
        return changed;
    }

    bool Chart::add_data(const double high, const double low, const double close, const Timestamp time) {
        if (config_.method == ConstructionMethod::HighLow)
            return process_high_low(high, low, time);
        return process_close(close, time);
    }

    bool Chart::add_data(const double price, const Timestamp time) {
        return add_data(price, price, price, time);
    }

    bool Chart::add_ohlc(const OHLC& ohlc) {
        return add_data(ohlc.high, ohlc.low, ohlc.close, ohlc.time);
    }

    Column* Chart::column(const size_t index) {
        return (index < columns_.size()) ? columns_[index].get() : nullptr;
    }

    const Column* Chart::column(const size_t index) const {
        return (index < columns_.size()) ? columns_[index].get() : nullptr;
    }

    Column* Chart::last_column() {
        return columns_.empty() ? nullptr : columns_.back().get();
    }

    const Column* Chart::last_column() const {
        return columns_.empty() ? nullptr : columns_.back().get();
    }

    size_t Chart::x_column_count() const {
        return std::ranges::count_if(columns_,
                                     [](const auto& c) { return c->type() == ColumnType::X; });
    }

    size_t Chart::o_column_count() const {
        return std::ranges::count_if(columns_,
                                     [](const auto& c) { return c->type() == ColumnType::O; });
    }

    size_t Chart::mixed_column_count() const {
        return std::ranges::count_if(columns_,
                                     [](const auto& c) { return c->type() == ColumnType::Mixed; });
    }

    std::vector<size_t> Chart::x_column_indices() const {
        std::vector<size_t> indices;
        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i]->type() == ColumnType::X)
                indices.push_back(i);
        }
        return indices;
    }

    std::vector<size_t> Chart::o_column_indices() const {
        std::vector<size_t> indices;
        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i]->type() == ColumnType::O)
                indices.push_back(i);
        }
        return indices;
    }

    std::vector<size_t> Chart::mixed_column_indices() const {
        std::vector<size_t> indices;
        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i]->type() == ColumnType::Mixed)
                indices.push_back(i);
        }
        return indices;
    }

    std::vector<double> Chart::all_prices() const {
        std::vector<double> prices;
        for (const auto& col : columns_) {
            for (size_t j = 0; j < col->box_count(); j++) {
                const Box* box = col->get_box_at(j);
                if (!box) continue;
                double price = box->price();
                bool found = false;
                for (const double existing : prices) {
                    if (std::abs(existing - price) < 0.00001) {
                        found = true;
                        break;
                    }
                }
                if (!found) prices.push_back(price);
            }
        }
        std::ranges::sort(prices, std::greater<>());
        return prices;
    }

    bool Chart::has_bullish_bias() const {
        return trend_manager_ ? trend_manager_->has_bullish_bias() : false;
    }

    bool Chart::has_bearish_bias() const {
        return trend_manager_ ? trend_manager_->has_bearish_bias() : false;
    }

    bool Chart::should_take_bullish_signals() const {
        return has_bullish_bias() || (!has_bullish_bias() && !has_bearish_bias());
    }

    bool Chart::should_take_bearish_signals() const {
        return has_bearish_bias() || (!has_bullish_bias() && !has_bearish_bias());
    }

    bool Chart::is_above_bullish_support(const double price) const {
        if (!trend_manager_) return false;
        const int col_index = static_cast<int>(columns_.size()) - 1;
        return trend_manager_->is_above_bullish_support(col_index, price);
    }

    bool Chart::is_below_bearish_resistance(const double price) const {
        if (!trend_manager_) return false;
        const int col_index = static_cast<int>(columns_.size()) - 1;
        return trend_manager_->is_below_bearish_resistance(col_index, price);
    }

    void Chart::clear() {
        columns_.clear();
        last_month_ = -1;
        last_processed_time_ = std::chrono::system_clock::now();
    }

    std::string Chart::to_string() const {
        const char* method_str = (config_.method == ConstructionMethod::Close) ?
                                 "Close" : "High/Low";
        const char* box_method_str;
        switch (config_.box_size_method) {
        case BoxSizeMethod::Fixed: box_method_str = "Fixed"; break;
        case BoxSizeMethod::Percentage: box_method_str = "Percentage"; break;
        case BoxSizeMethod::Points: box_method_str = "Points"; break;
        default: box_method_str = "Traditional"; break;
        }

        std::ostringstream oss;
        oss << "Point & Figure Chart\n"
            << "Construction: " << method_str
            << ", Box Size: " << box_method_str << " (" << config_.box_size << ")"
            << ", Reversal: " << config_.reversal << "\n"
            << "Columns: " << columns_.size() << "\n";

        if (trend_manager_) {
            oss << "Trend Bias: "
                << (has_bullish_bias() ? "BULLISH" :
                    has_bearish_bias() ? "BEARISH" : "NONE") << "\n";
        }
        oss << "\n";

        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i]) {
                oss << "Column " << (i + 1) << ":\n" << columns_[i]->to_string() << "\n";
            }
        }

        if (trend_manager_)
            oss << trend_manager_->to_string();

        return oss.str();
    }
}
