/// \file box.cpp
/// \brief Box implementation.

//
// Created by gregorian-rayne on 12/01/2026.
//

#include "pnf/box.hpp"
#include <sstream>

namespace pnf {

    Box::Box(const double price, const BoxType type, std::string marker)
        : price_(price), type_(type), marker_(std::move(marker)) {}

    std::string Box::to_string() const {
        std::ostringstream oss;
        const char box_char = (type_ == BoxType::X) ? 'X' : 'O';
        const std::string display = marker_.empty() ? std::string(1, box_char) : marker_;
        oss << price_ << display;
        return oss.str();
    }

}