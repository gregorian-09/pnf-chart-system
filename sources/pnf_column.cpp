//
// Created by gregorian on 03/10/2025.
//

#include "pnf_column.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>

ChartSystem::PnFBox::PnFBox(const double price, const BoxType boxType, std::string marker)
{
    this->price = price;
    this->boxType = boxType;
    this->marker = std::move(marker);
}

ChartSystem::PnFBox::~PnFBox() = default;

std::string ChartSystem::PnFBox::ToString() const
{
    const char boxChar = (boxType == X) ? 'X' : 'O';
    const std::string display = (!marker.empty()) ? marker : std::string(1, boxChar);
    return std::to_string(price) + display;
}

ChartSystem::PnFColumn::~PnFColumn() = default;

ChartSystem::PnFColumn::PnFColumn(const ColumnType columnType)
{
    this->columnType = columnType;
}

bool ChartSystem::PnFColumn::AddBox(double price, BoxType boxType)
{
    if (HasBox(price))
        return false;

    boxes.push_back(std::make_unique<PnFBox>(price, boxType));
    return true;
}

bool ChartSystem::PnFColumn::AddBox(const double price, const BoxType boxType, const std::string& marker)
{
    if (HasBox(price))
        return false;

    boxes.push_back(std::make_unique<PnFBox>(price, boxType, marker));
    return true;
}

bool ChartSystem::PnFColumn::RemoveBox(const double price)
{
    int index = -1;
    for (int i = 0; i < boxes.size(); i++)
    {
        if(i < boxes.size() && boxes[i] != nullptr && boxes[i]->GetPrice() == price)
        {
            index = i;
            break;
        }
    }

    if (index >= 0)
    {
        if (index < boxes.size())
        {
            const PnFBox* box = boxes[index].get();
            delete box;
        }
        boxes.erase(boxes.begin() + index);
    }

    return true;
}

bool ChartSystem::PnFColumn::HasBox(const double price) const
{
    for (const auto & box : boxes)
    {
        if (box != nullptr && box->GetPrice() == price)
            return true;
    }
    return false;
}

ChartSystem::PnFBox* ChartSystem::PnFColumn::GetBox(const double price) const
{
    for (const auto & box : boxes)
    {
        if (box != nullptr && box->GetPrice() == price)
            return box.get();
    }
    return nullptr;
}

ChartSystem::PnFBox* ChartSystem::PnFColumn::GetBoxAt(const int index) const
{
    return boxes[index].get();
}

std::string ChartSystem::PnFColumn::GetBoxMarker(const double price) const
{
    const PnFBox* box = GetBox(price);
    return (box != nullptr) ? box->GetMarker() : std::string();
}

bool ChartSystem::PnFColumn::SetBoxMarker(const double price, const std::string& marker) const
{
    if (PnFBox* box = GetBox(price); box != nullptr)
    {
        box->SetMarker(marker);
        return true;
    }
    return false;
}

int ChartSystem::PnFColumn::GetBoxCount() const
{
    return static_cast<int>(boxes.size());
}

double ChartSystem::PnFColumn::GetHighestPrice() const
{
    if (boxes.empty())
        return 0.0;

    const auto maxIt = std::ranges::max_element(boxes,
        [](const std::unique_ptr<PnFBox>& a, const std::unique_ptr<PnFBox>& b) {
            return a->GetPrice() < b->GetPrice();
    });
    return (*maxIt)->GetPrice();
}

double ChartSystem::PnFColumn::GetLowestPrice() const
{
    if (boxes.empty())
        return 0.0;

    const auto minIt = std::ranges::min_element(boxes,
        [](const std::unique_ptr<PnFBox>& a, const std::unique_ptr<PnFBox>& b) {
            return a->GetPrice() < b->GetPrice();
    });

    return (*minIt)->GetPrice();
}

void ChartSystem::PnFColumn::Clear()
{
    boxes.clear();
}

std::string ChartSystem::PnFColumn::ToString() const
{
    const std::string columnTypeStr = (columnType == COLUMN_X) ? "X" :
                                (columnType == COLUMN_O) ? "O" : "Mixed";

    std::ostringstream oss;
    oss << "Column Type: " << columnTypeStr
        << ", Boxes: " << boxes.size() << "\n";

    for (const auto& box : boxes)
    {
        if (box != nullptr)
            oss << box->ToString() << "\n";
    }

    return oss.str();
}

void ChartSystem::PnFColumn::PrintColumn() const
{
    std::cout << ToString() << std::endl;
}
