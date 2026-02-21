/// \file box.hpp
/// \brief Box model and helpers.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef BOX_HPP
#define BOX_HPP

#include "types.hpp"

namespace pnf {

    /**
     * @brief Represents a type of box with a price, type, and optional marker.
     */
    class Box {
    public:
        /**
         * @brief Constructs a new Box object.
         *
         * @param price The price of the box.
         * @param type The type of the box (BoxType enum expected).
         * @param marker An optional string marker for the box. Defaults to an empty string.
         */
        Box(double price, BoxType type, std::string marker = "");

        /**
         * @brief Gets the price of the box.
         *
         * @return The price as a double.
         */
        double price() const { return price_; }

        /**
         * @brief Gets the type of the box.
         *
         * @return The BoxType of the box.
         */
        BoxType type() const { return type_; }

        /**
         * @brief Gets the marker string of the box.
         *
         * @return A constant reference to the marker string.
         */
        const std::string& marker() const { return marker_; }

        /**
         * @brief Sets the marker string of the box.
         *
         * @param marker The new marker string.
         */
        void set_marker(std::string marker) { marker_ = std::move(marker); }

        /**
         * @brief Sets the type of the box.
         *
         * @param type The new BoxType.
         */
        void set_type(BoxType type) { type_ = type; }

        /**
         * @brief Returns a string representation of the box.
         *
         * @return A std::string containing box details.
         */
        std::string to_string() const;

    private:
        double price_;          /**< Price of the box */
        BoxType type_;          /**< Type of the box */
        std::string marker_;    /**< Optional marker for the box */
    };

} // namespace pnf


#endif //BOX_HPP
