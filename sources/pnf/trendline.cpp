/// \file trendline.cpp
/// \brief Trendline implementation.

//
// Created by gregorian-rayne on 12/01/2026.
//

#include "pnf/trendline.hpp"
#include <sstream>
#include <algorithm>

namespace pnf
{
    TrendLine::TrendLine(const TrendLineType type, const int start_column, const double start_price,
                         const int start_box_index, const double box_size)
        : type_(type), box_size_(box_size), active_(true), touched_(false), touch_count_(0) {
        start_.column_index = start_column;
        start_.price = start_price;
        start_.box_index = start_box_index;
        end_ = start_;
    }

    void TrendLine::update_end_point(const int column, const double price, const int box_index) {
        end_.column_index = column;
        end_.price = price;
        end_.box_index = box_index;
    }

    double TrendLine::price_at_column(const int column) const {
        if (column < start_.column_index) return 0;
        const int diff = column - start_.column_index;
        if (type_ == TrendLineType::BullishSupport)
            return start_.price + (diff * box_size_);
        return start_.price - (diff * box_size_);
    }

    bool TrendLine::is_broken(const int column, const double price) const {
        if (!active_ || column <= start_.column_index) return false;
        const double line_price = price_at_column(column);
        if (type_ == TrendLineType::BullishSupport)
            return price < line_price - box_size_;
        return price > line_price + box_size_;
    }

    bool TrendLine::test(const int column, const double price) {
        if (!active_ || column <= start_.column_index) return false;
        if (const double line_price = price_at_column(column); std::abs(price - line_price) < box_size_ * 0.5) {
            touched_ = true;
            touch_count_++;
            return true;
        }
        return false;
    }

    std::string TrendLine::to_string() const {
        std::ostringstream oss;
        const char* type_str = (type_ == TrendLineType::BullishSupport) ?
                               "Bullish Support" : "Bearish Resistance";
        oss << type_str << " Line: Start(Col:" << start_.column_index
            << ", Price:" << start_.price << ") Active:" << (active_ ? "Yes" : "No")
            << " Touched:" << touch_count_ << " times";
        return oss.str();
    }

    TrendLineManager::TrendLineManager(const double box_size)
        : active_(nullptr), box_size_(box_size) {}

    void TrendLineManager::clear() {
        lines_.clear();
        active_ = nullptr;
    }

    bool TrendLineManager::is_significant_low(const Column* col,
        const std::vector<std::unique_ptr<Column>>& all, int index) {
        if (col->type() != ColumnType::O || index < 1) return false;

        const double current_low = col->lowest_price();
        const Column* prev = all[index - 1].get();
        if (prev->type() != ColumnType::X) return false;
        if (current_low >= prev->highest_price()) return false;

        const int lookback = std::min(3, index);
        for (int i = 1; i <= lookback; i++) {
            if (index - i >= 0 && all[index - i]->lowest_price() < current_low)
                return false;
        }
        return true;
    }

    bool TrendLineManager::is_significant_high(const Column* col,
        const std::vector<std::unique_ptr<Column>>& all, int index) {
        if (col->type() != ColumnType::X || index < 1) return false;

        const double current_high = col->highest_price();
        const Column* prev = all[index - 1].get();
        if (prev->type() != ColumnType::O) return false;
        if (current_high <= prev->lowest_price()) return false;

        const int lookback = std::min(3, index);
        for (int i = 1; i <= lookback; i++) {
            if (index - i >= 0 && all[index - i]->highest_price() > current_high)
                return false;
        }
        return true;
    }

    int TrendLineManager::find_significant_low(
        const std::vector<std::unique_ptr<Column>>& columns, int from) {
        for (int i = from; i >= 0; i--) {
            if (is_significant_low(columns[i].get(), columns, i))
                return i;
        }
        return -1;
    }

    int TrendLineManager::find_significant_high(
        const std::vector<std::unique_ptr<Column>>& columns, int from) {
        for (int i = from; i >= 0; i--) {
            if (is_significant_high(columns[i].get(), columns, i))
                return i;
        }
        return -1;
    }

    void TrendLineManager::process_new_column(
        const std::vector<std::unique_ptr<Column>>& columns, int col_index) {
        if (col_index < 1 || col_index >= static_cast<int>(columns.size())) return;

        const Column* current = columns[col_index].get();

        if (const Column* prev = columns[col_index - 1].get(); current->type() == ColumnType::X && prev->type() == ColumnType::O) {
            if (active_ && active_->type() == TrendLineType::BearishResistance) {
                if (const double high = current->highest_price(); active_->is_broken(col_index, high)) {
                    active_->set_active(false);
                    if (int low_col = find_significant_low(columns, col_index - 1); low_col >= 0) {
                        const Column* low_column = columns[low_col].get();
                        auto line = std::make_unique<TrendLine>(
                            TrendLineType::BullishSupport, low_col,
                            low_column->lowest_price(), 0, box_size_);
                        lines_.push_back(std::move(line));
                        active_ = lines_.back().get();
                    }
                }
            } else if (!active_) {
                if (int low_col = find_significant_low(columns, col_index - 1); low_col >= 0) {
                    const Column* low_column = columns[low_col].get();
                    auto line = std::make_unique<TrendLine>(
                        TrendLineType::BullishSupport, low_col,
                        low_column->lowest_price(), 0, box_size_);
                    lines_.push_back(std::move(line));
                    active_ = lines_.back().get();
                }
            }
        } else if (current->type() == ColumnType::O && prev->type() == ColumnType::X) {
            if (active_ && active_->type() == TrendLineType::BullishSupport) {
                if (const double low = current->lowest_price(); active_->is_broken(col_index, low)) {
                    active_->set_active(false);
                    if (int high_col = find_significant_high(columns, col_index - 1); high_col >= 0) {
                        const Column* high_column = columns[high_col].get();
                        auto line = std::make_unique<TrendLine>(
                            TrendLineType::BearishResistance, high_col,
                            high_column->highest_price(), 0, box_size_);
                        lines_.push_back(std::move(line));
                        active_ = lines_.back().get();
                    }
                }
            } else if (!active_) {
                if (int high_col = find_significant_high(columns, col_index - 1); high_col >= 0) {
                    const Column* high_column = columns[high_col].get();
                    auto line = std::make_unique<TrendLine>(
                        TrendLineType::BearishResistance, high_col,
                        high_column->highest_price(), 0, box_size_);
                    lines_.push_back(std::move(line));
                    active_ = lines_.back().get();
                }
            }
        }
    }

    void TrendLineManager::check_break(
        const std::vector<std::unique_ptr<Column>>& columns, int col_index) const {
        if (!active_ || !active_->is_active()) return;
        if (col_index < 0 || col_index >= static_cast<int>(columns.size())) return;

        const Column* current = columns[col_index].get();
        if (active_->type() == TrendLineType::BullishSupport) {
            if (const double low = current->lowest_price(); active_->is_broken(col_index, low))
                active_->set_active(false);
            else
                active_->test(col_index, low);
        } else {
            if (const double high = current->highest_price(); active_->is_broken(col_index, high))
                active_->set_active(false);
            else
                active_->test(col_index, high);
        }
    }

    void TrendLineManager::update(
        const std::vector<std::unique_ptr<Column>>& columns, const int new_column_index) {
        check_break(columns, new_column_index);
        process_new_column(columns, new_column_index);
    }

    bool TrendLineManager::is_above_bullish_support(const int column, const double price) const {
        if (!active_ || !active_->is_active()) return false;
        if (active_->type() != TrendLineType::BullishSupport) return false;
        return price > active_->price_at_column(column);
    }

    bool TrendLineManager::is_below_bearish_resistance(const int column, const double price) const {
        if (!active_ || !active_->is_active()) return false;
        if (active_->type() != TrendLineType::BearishResistance) return false;
        return price < active_->price_at_column(column);
    }

    bool TrendLineManager::has_bullish_bias() const {
        return active_ && active_->is_active() &&
               active_->type() == TrendLineType::BullishSupport;
    }

    bool TrendLineManager::has_bearish_bias() const {
        return active_ && active_->is_active() &&
               active_->type() == TrendLineType::BearishResistance;
    }

    std::string TrendLineManager::to_string() const {
        std::ostringstream oss;
        oss << "TrendLine Manager - Total Lines: " << lines_.size() << "\n";
        if (active_)
            oss << "Active: " << active_->to_string() << "\n";
        else
            oss << "Active: None\n";
        oss << "Bias: " << (has_bullish_bias() ? "Bullish" :
                            has_bearish_bias() ? "Bearish" : "None") << "\n";
        return oss.str();
    }
}