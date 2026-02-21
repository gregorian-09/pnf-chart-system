/// \file column.hpp
/// \brief Column model and helpers.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "box.hpp"
#include <vector>
#include <memory>

namespace pnf {
    /**
     * @brief Represents a column in a Point & Figure chart.
     *
     * A column contains a sequence of boxes and has a type (X or O).
     */
    class Column {
    public:
        /**
         * @brief Constructs a new Column object.
         *
         * @param type The type of the column (default is ColumnType::X)
         */
        explicit Column(ColumnType type = ColumnType::X);

        /**
         * @brief Adds a box to the column with the given price and type.
         *
         * @param price Price of the box
         * @param box_type Type of the box (X or O)
         * @return true if the box was successfully added, false otherwise
         */
        bool add_box(double price, BoxType box_type);

        /**
         * @brief Adds a box to the column with the given price, type, and marker.
         *
         * @param price Price of the box
         * @param box_type Type of the box (X or O)
         * @param marker Optional marker string
         * @return true if the box was successfully added, false otherwise
         */
        bool add_box(double price, BoxType box_type, const std::string& marker);

        /**
         * @brief Removes a box at the given price from the column.
         *
         * @param price Price of the box to remove
         * @return true if a box was removed, false if no box with that price exists
         */
        bool remove_box(double price);

        /**
         * @brief Checks if a box exists at the given price.
         *
         * @param price Price to check
         * @return true if a box with the given price exists, false otherwise
         */
        bool has_box(double price) const;

        /**
         * @brief Gets a pointer to the box at the given price.
         *
         * @param price Price of the box
         * @return Pointer to the Box if found, nullptr otherwise
         */
        Box* get_box(double price);
        const Box* get_box(double price) const;

        /**
         * @brief Gets a pointer to the box at the specified index.
         *
         * @param index Index of the box in the column
         * @return Pointer to the Box if index is valid, nullptr otherwise
         */
        Box* get_box_at(size_t index);
        const Box* get_box_at(size_t index) const;

        /**
         * @brief Gets the marker of the box at the given price.
         *
         * @param price Price of the box
         * @return Marker string if the box exists, empty string otherwise
         */
        std::string get_box_marker(double price) const;

        /**
         * @brief Sets the marker for the box at the given price.
         *
         * @param price Price of the box
         * @param marker New marker string
         * @return true if the marker was set, false if the box does not exist
         */
        bool set_box_marker(double price, const std::string& marker);

        /**
         * @brief Returns the number of boxes in the column.
         *
         * @return Number of boxes
         */
        size_t box_count() const { return boxes_.size(); }

        /**
         * @brief Gets the highest price in the column.
         *
         * @return Highest box price
         */
        double highest_price() const;

        /**
         * @brief Gets the lowest price in the column.
         *
         * @return Lowest box price
         */
        double lowest_price() const;

        /**
         * @brief Gets the type of the column.
         *
         * @return ColumnType (X, O, or Mixed)
         */
        ColumnType type() const { return type_; }

        /**
         * @brief Sets the type of the column.
         *
         * @param type New column type
         */
        void set_type(ColumnType type) { type_ = type; }

        /**
         * @brief Clears all boxes from the column.
         */
        void clear();

        /**
         * @brief Returns a string representation of the column.
         *
         * @return Column details as a string
         */
        std::string to_string() const;

    private:
        std::vector<std::unique_ptr<Box>> boxes_; /**< List of boxes in the column */
        ColumnType type_;                         /**< Type of the column */
    };
} // namespace pnf


#endif //COLUMN_HPP
