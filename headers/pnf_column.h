//
// Created by gregorian on 03/10/2025.
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ChartSystem
{
    enum BoxType { X, O };
    enum ColumnType { COLUMN_X, COLUMN_O, COLUMN_MIXED };
    enum ConstructionType { CLOSING_PRICE, HIGH_LOW };
    enum BoxSizeType { FIXED, DEFAULT, POINTS, PERCENTAGE };

    struct MonthMarker
    {
        int month;
        std::string marker;

        MonthMarker(const int month, std::string marker)
            : month(month), marker(std::move(marker)) {}
    };

    class PnFBox
    {
        double price;
        std::string marker;
        BoxType boxType;

    public:
        PnFBox(double price, BoxType boxType, std::string marker = "");
        ~PnFBox();

        [[nodiscard]] double GetPrice() const { return price; }
        [[nodiscard]] std::string GetMarker() const { return marker; }
        [[nodiscard]] BoxType GetBoxType() const { return boxType; }

        void SetMarker(std::string marker) { this->marker = std::move(marker); }
        void SetBoxType(const BoxType boxType) { this->boxType = boxType; }

        [[nodiscard]] std::string ToString() const;
    };

    class PnFColumn
    {
    private:
        std::vector<std::unique_ptr<PnFBox>> boxes;
        ColumnType columnType;

    public:
        explicit PnFColumn(ColumnType columnType = COLUMN_X);
        ~PnFColumn();

        bool AddBox(double price, BoxType boxType);
        bool AddBox(double price, BoxType boxType, const std::string& marker);
        bool RemoveBox(double price);
        [[nodiscard]] bool HasBox(double price) const;

        [[nodiscard]] PnFBox* GetBox(double price) const;
        [[nodiscard]] PnFBox* GetBoxAt(int index) const;
        [[nodiscard]] std::string GetBoxMarker(double price) const;
        [[nodiscard]] bool SetBoxMarker(double price, const std::string& marker) const;

        [[nodiscard]] int GetBoxCount() const;
        [[nodiscard]] double GetHighestPrice() const;
        [[nodiscard]] double GetLowestPrice() const;

        [[nodiscard]] ColumnType GetColumnType() const { return columnType; }
        void SetColumnType(const ColumnType columnType) { this->columnType = columnType; }

        void Clear();
        [[nodiscard]] std::string ToString() const;
        void PrintColumn() const;
    };
}
