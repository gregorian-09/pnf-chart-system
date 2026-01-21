//
// Created by gregorian-rayne on 12/01/2026.
//

#include "pnf/column.hpp"
#include <algorithm>
#include <sstream>

namespace pnf
{
    Column::Column(const ColumnType type) : type_(type) {}

    bool Column::add_box(double price, BoxType box_type) {
        if (has_box(price)) return false;
        boxes_.push_back(std::make_unique<Box>(price, box_type));
        return true;
    }

    bool Column::add_box(double price, BoxType box_type, const std::string& marker) {
        if (has_box(price)) return false;
        boxes_.push_back(std::make_unique<Box>(price, box_type, marker));
        return true;
    }

    bool Column::remove_box(double price) {
        const auto it = std::ranges::find_if(boxes_,
                                             [price](const auto& box) { return box && box->price() == price; });
        if (it != boxes_.end()) {
            boxes_.erase(it);
            return true;
        }
        return false;
    }

    bool Column::has_box(double price) const {
        return std::ranges::any_of(boxes_,
                                   [price](const auto& box) { return box && box->price() == price; });
    }

    Box* Column::get_box(double price) const {
        const auto it = std::ranges::find_if(boxes_,
                                             [price](const auto& box) { return box && box->price() == price; });
        return (it != boxes_.end()) ? it->get() : nullptr;
    }

    Box* Column::get_box_at(const size_t index) const {
        return (index < boxes_.size()) ? boxes_[index].get() : nullptr;
    }

    std::string Column::get_box_marker(const double price) const {
        const Box* box = get_box(price);
        return box ? box->marker() : std::string();
    }

    bool Column::set_box_marker(const double price, const std::string& marker) const {
        if (Box* box = get_box(price)) {
            box->set_marker(marker);
            return true;
        }
        return false;
    }

    double Column::highest_price() const {
        if (boxes_.empty()) return 0.0;
        const auto it = std::ranges::max_element(boxes_,
                                                 [](const auto& a, const auto& b) { return a->price() < b->price(); });
        return (*it)->price();
    }

    double Column::lowest_price() const {
        if (boxes_.empty()) return 0.0;
        const auto it = std::ranges::min_element(boxes_,
                                                 [](const auto& a, const auto& b) { return a->price() < b->price(); });
        return (*it)->price();
    }

    void Column::clear() {
        boxes_.clear();
    }

    std::string Column::to_string() const {
        const char* type_str = (type_ == ColumnType::X) ? "X" :
                              (type_ == ColumnType::O) ? "O" : "Mixed";
        std::ostringstream oss;
        oss << "Column Type: " << type_str << ", Boxes: " << boxes_.size() << "\n";
        for (const auto& box : boxes_) {
            if (box) oss << box->to_string() << "\n";
        }
        return oss.str();
    }
}