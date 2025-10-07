//
// Created by gregorian on 03/10/2025.
//

#include "pnf_chart.h"
#include <cmath>
#include <filesystem>
#include <OpenXLSX.hpp>

ChartSystem::PnFTrendLine::PnFTrendLine(const TrendLineType trendLineType, const int startColumnIndex, const double startPrice, const int startBoxIndex, const double boxSize)
{
    this->trendLineType = trendLineType;
    startPoint.columnIndex = startColumnIndex;
    startPoint.price = startPrice;
    startPoint.boxIndex = startBoxIndex;
    this->boxSize = boxSize;
    isActive = true;
    wasTouched = false;
    touchCount = 0;
    endPoint = startPoint;
}

ChartSystem::PnFTrendLine::~PnFTrendLine() = default;

void ChartSystem::PnFTrendLine::UpdateEndPoint(const int endColumnIndex, const double endPrice, const int endBoxIndex)
{
    endPoint.columnIndex = endColumnIndex;
    endPoint.price = endPrice;
    endPoint.boxIndex = endBoxIndex;
}

double ChartSystem::PnFTrendLine::CalculatePriceAtColumn(const int columnIndex) const
{
    if (columnIndex < startPoint.columnIndex)
        return 0;

    const int columnDifference = columnIndex - startPoint.columnIndex;

    if (trendLineType == BULLISH_SUPPORT)
        return startPoint.price + (columnDifference * boxSize);

    return startPoint.price - (columnDifference * boxSize);
}

double ChartSystem::PnFTrendLine::GetPriceAtColumn(const int columnIndex) const
{
    return CalculatePriceAtColumn(columnIndex);
}

bool ChartSystem::PnFTrendLine::IsBroken(const int columnIndex, const double price) const
{
    if (!isActive || columnIndex <= startPoint.columnIndex)
        return false;

    const double trendLinePrice = CalculatePriceAtColumn(columnIndex);

    if (trendLineType == BULLISH_SUPPORT)
        return price < trendLinePrice - boxSize;

    return price > trendLinePrice + boxSize;
}

bool ChartSystem::PnFTrendLine::GetValue(const double price, const double trendlinePrice, bool& value1)
{
    if (const bool applicable = (trendLineType == BULLISH_SUPPORT) || (trendLineType == BEARISH_RESISTANCE); !applicable)
        return false;

    if (std::abs(price - trendlinePrice) < boxSize * 0.5)
    {
        wasTouched = true;
        touchCount++;
        value1 = true;
        return true;
    }
    return false;
}

bool ChartSystem::PnFTrendLine::TestTrendLine(const int columnIndex, const double price)
{
    if(!isActive || columnIndex <= startPoint.columnIndex)
        return false;

    const double trendlinePrice = CalculatePriceAtColumn(columnIndex);

    if (bool value1; GetValue(price, trendlinePrice, value1)) return value1;

    return false;
}

std::string ChartSystem::PnFTrendLine::ToString() const
{
    const std::string typeStr = (trendLineType == BULLISH_SUPPORT) ? "Bullish Support" : "Bearish Resistance";
    return std::format(
        "{} Line: Start(Col:{}, Price:{:.5f}) Active:{} Touched:{} times",
        typeStr,
        startPoint.columnIndex,
        startPoint.price,
        (isActive ? "Yes" : "No"),
        touchCount
    );
}

ChartSystem::PnFTrendLineManager::PnFTrendLineManager(const double boxSize)
{
    this->boxSize = boxSize;
    activeTrendLine = nullptr;
}

ChartSystem::PnFTrendLineManager::~PnFTrendLineManager()
{
    Clear();
}

void ChartSystem::PnFTrendLineManager::Clear()
{
    trendLines.clear();
    activeTrendLine = nullptr;
}

bool ChartSystem::PnFTrendLineManager::IsSignificantLow(const PnFColumn* column, const std::vector<std::unique_ptr<PnFColumn>>& allColumns, const int columnIndex)
{
    if (column->GetColumnType() != COLUMN_O)
        return false;

    if (columnIndex < 1)
        return false;

    const double currentLow = column->GetLowestPrice();

    const PnFColumn* prevColumn = allColumns[columnIndex - 1].get();
    if (prevColumn->GetColumnType() != COLUMN_X)
        return false;

    const double prevHigh = prevColumn->GetHighestPrice();

    if (currentLow >= prevHigh)
        return false;

    bool isLowest = true;
    const int lookback = std::min(3, columnIndex);

    for (int i = 1; i <= lookback; i++)
    {
        if (columnIndex - i >= 0)
        {
            const PnFColumn* checkCol = allColumns[columnIndex - i].get();
            if (checkCol->GetLowestPrice() < currentLow)
            {
                isLowest = false;
                break;
            }
        }
    }

    return isLowest;
}

bool ChartSystem::PnFTrendLineManager::IsSignificantHigh(const PnFColumn* column, const std::vector<std::unique_ptr<PnFColumn>>& allColumns, const int columnIndex)
{
    if (column->GetColumnType() != COLUMN_X)
        return false;

    if (columnIndex < 1)
        return false;

    const double currentHigh = column->GetHighestPrice();

    const PnFColumn* prevColumn = allColumns[columnIndex - 1].get();
    if (prevColumn->GetColumnType() != COLUMN_O)
        return false;

    if (const double prevLow = prevColumn->GetLowestPrice(); currentHigh <= prevLow)
        return false;

    bool isHighest = true;
    const int lookback = std::min(3, columnIndex);

    for (int i = 1; i <= lookback; i++)
    {
        if (columnIndex - i >= 0)
        {
            if (const PnFColumn* checkCol = allColumns[columnIndex - i].get(); checkCol->GetHighestPrice() > currentHigh)
            {
                isHighest = false;
                break;
            }
        }
    }

    return isHighest;
}

int ChartSystem::PnFTrendLineManager::FindSignificantLow(const std::vector<std::unique_ptr<PnFColumn>>& columns, const int fromColumn)
{
    for(int i = fromColumn; i >= 0; i--)
    {
        if (IsSignificantLow(columns[i].get(), columns, i))
            return i;
    }

    return -1;
}

int ChartSystem::PnFTrendLineManager::FindSignificantHigh(const std::vector<std::unique_ptr<PnFColumn>>& columns, const int fromColumn)
{
    for(int i = fromColumn; i >= 0; i--)
    {
        if (IsSignificantHigh(columns[i].get(), columns, i))
            return i;
    }

    return -1;
}

void ChartSystem::PnFTrendLineManager::ProcessNewColumn(const std::vector<std::unique_ptr<PnFColumn>>& columns, const int columnIndex)
{
    if (columnIndex < 1 || columnIndex >= columns.size())
        return;

    const PnFColumn* currentColumn = columns[columnIndex].get();

    if (const PnFColumn* prevColumn = columns[columnIndex - 1].get(); currentColumn->GetColumnType() == COLUMN_X && prevColumn->GetColumnType() == COLUMN_O)
    {
        if (activeTrendLine != nullptr && activeTrendLine->GetTrendLineType() == BEARISH_RESISTANCE)
        {
            if (const double highestPrice = currentColumn->GetHighestPrice(); activeTrendLine->IsBroken(columnIndex, highestPrice))
            {
                activeTrendLine->SetActive(false);

                if (const int lowColumnIndex = FindSignificantLow(columns, columnIndex - 1); lowColumnIndex >= 0)
                {
                    const PnFColumn* lowColumn = columns[lowColumnIndex].get();
                    auto newTrendline = std::make_unique<PnFTrendLine>(
                        BULLISH_SUPPORT,
                        lowColumnIndex,
                        lowColumn->GetLowestPrice(),
                        0, boxSize
                    );
                    trendLines.push_back(std::move(newTrendline));
                    activeTrendLine = trendLines.back().get();
                }
            }
        }
        else if (activeTrendLine == nullptr)
        {
            if (const int lowColumnIndex = FindSignificantLow(columns, columnIndex - 1); lowColumnIndex >= 0)
            {
                const PnFColumn* lowColumn = columns[lowColumnIndex].get();
                auto newTrendline = std::make_unique<PnFTrendLine>(
                    BULLISH_SUPPORT,
                    lowColumnIndex,
                    lowColumn->GetLowestPrice(),
                    0, boxSize
                );
                trendLines.push_back(std::move(newTrendline));
                activeTrendLine = trendLines.back().get();
            }
        }
    }
    else if (currentColumn->GetColumnType() == COLUMN_O && prevColumn->GetColumnType() == COLUMN_X)
    {
        if (activeTrendLine != nullptr && activeTrendLine->GetTrendLineType() == BULLISH_SUPPORT)
        {
            if (const double lowestPrice = currentColumn->GetLowestPrice(); activeTrendLine->IsBroken(columnIndex, lowestPrice))
            {
                activeTrendLine->SetActive(false);

                if (const int highColumnIndex = FindSignificantHigh(columns, columnIndex - 1); highColumnIndex >= 0)
                {
                    const PnFColumn* highColumn = columns[highColumnIndex].get();
                    auto newTrendline = std::make_unique<PnFTrendLine>(
                        BEARISH_RESISTANCE,
                        highColumnIndex,
                        highColumn->GetHighestPrice(),
                        0, boxSize
                    );
                    trendLines.push_back(std::move(newTrendline));
                    activeTrendLine = trendLines.back().get();
                }
            }
        }
        else if (activeTrendLine == nullptr)
        {
            if (const int highColumnIndex = FindSignificantHigh(columns, columnIndex - 1); highColumnIndex >= 0)
            {
                const PnFColumn* highColumn = columns[highColumnIndex].get();
                auto newTrendline = std::make_unique<PnFTrendLine>(
                    BEARISH_RESISTANCE,
                    highColumnIndex,
                    highColumn->GetHighestPrice(),
                    0, boxSize
                );
                trendLines.push_back(std::move(newTrendline));
                activeTrendLine = trendLines.back().get();
            }
        }
    }
}

void ChartSystem::PnFTrendLineManager::CheckTrendLineBreak(const std::vector<std::unique_ptr<PnFColumn>>& columns, const int columnIndex) const
{
    if (activeTrendLine == nullptr || !activeTrendLine->IsActive())
        return;

    if (columnIndex < 0 || columnIndex >= columns.size())
        return;

    const PnFColumn* currentColumn = columns[columnIndex].get();

    if (activeTrendLine->GetTrendLineType() == BULLISH_SUPPORT)
    {
        if (const double lowestPrice = currentColumn->GetLowestPrice(); activeTrendLine->IsBroken(columnIndex, lowestPrice))
        {
            activeTrendLine->SetActive(false);
        }
        else
        {
            activeTrendLine->TestTrendLine(columnIndex, lowestPrice);
        }
    }
    else
    {
        if (const double highestPrice = currentColumn->GetHighestPrice(); activeTrendLine->IsBroken(columnIndex, highestPrice))
        {
            activeTrendLine->SetActive(false);
        }
        else
        {
            activeTrendLine->TestTrendLine(columnIndex, highestPrice);
        }
    }
}

void ChartSystem::PnFTrendLineManager::UpdateTrendLines(const std::vector<std::unique_ptr<PnFColumn>>& columns, const int newColumnIndex)
{
    CheckTrendLineBreak(columns, newColumnIndex);
    ProcessNewColumn(columns, newColumnIndex);
}

bool ChartSystem::PnFTrendLineManager::IsAboveBullishSupport(const int columnIndex, const double price) const
{
    if(activeTrendLine == nullptr || !activeTrendLine->IsActive())
        return false;

    if(activeTrendLine->GetTrendLineType() != BULLISH_SUPPORT)
        return false;

    return price > activeTrendLine->GetPriceAtColumn(columnIndex);
}

bool ChartSystem::PnFTrendLineManager::IsBelowBearishResistance(const int columnIndex, const double price) const
{
    if(activeTrendLine == nullptr || !activeTrendLine->IsActive())
        return false;

    if(activeTrendLine->GetTrendLineType() != BEARISH_RESISTANCE)
        return false;

    return price < activeTrendLine->GetPriceAtColumn(columnIndex);
}

bool ChartSystem::PnFTrendLineManager::HasBullishBias() const
{
    return (activeTrendLine != nullptr && activeTrendLine->IsActive() &&
           activeTrendLine->GetTrendLineType() == BULLISH_SUPPORT);
}

bool ChartSystem::PnFTrendLineManager::HasBearishBias() const
{
    return (activeTrendLine != nullptr && activeTrendLine->IsActive() &&
           activeTrendLine->GetTrendLineType() == BEARISH_RESISTANCE);
}

std::string ChartSystem::PnFTrendLineManager::ToString() const
{
    std::string result = std::format("P&F Trendline Manager - Total Lines: {}\n", trendLines.size());

    if (activeTrendLine != nullptr)
        result += "Active: " + activeTrendLine->ToString() + "\n";
    else
        result += "Active: None\n";

    result += std::format("Bias: {}\n", HasBullishBias() ? "Bullish" : HasBearishBias() ? "Bearish" : "None");

    return result;
}

ChartSystem::PointAndFigureChart::PointAndFigureChart(const ConstructionType constructionType, const BoxSizeType boxSizeType,
        const double boxSize, const int reversalCount): boxType()
{
    this->constructionType = constructionType;
    this->boxSizeType = boxSizeType;
    this->boxSize = boxSize;
    this->reversalCount = reversalCount;
    lastDateTime = std::chrono::system_clock::now();
    lastProcessedTime = std::chrono::system_clock::now();
    lastMonth = -1;
    this->boxType = boxType;
    this->trendLineManager = std::make_unique<PnFTrendLineManager>(this->boxSize);
}

ChartSystem::PointAndFigureChart::~PointAndFigureChart()
{
    Clear();
}

std::string ChartSystem::PointAndFigureChart::GetMonthMarker(const int month)
{
    if(month >= 1 && month <= 12)
        return monthMarker[month - 1].marker;
    return "";
}

double ChartSystem::PointAndFigureChart::CalculateBoxSize(const double price)
{
    switch(boxSizeType)
    {
    case FIXED:
        return boxSize;
    case PERCENTAGE:
        return price * boxSize / 100.0;
    case POINTS:
        return boxSize;
    case DEFAULT:
    default:
        {
            if(price < 0.25) { boxSize = 0.0625; return boxSize; }
            if(price < 1.0) { boxSize = 0.125; return boxSize; }
            if(price < 5.0) { boxSize = 0.25; return boxSize; }
            if(price < 20.0) { boxSize = 0.5; return boxSize; }
            if(price < 100.0) { boxSize = 1.0; return boxSize; }
            if(price < 200.0) { boxSize = 2.0; return boxSize; }
            if(price < 500.0) { boxSize = 4.0; return boxSize; }
            if(price < 1000.0) { boxSize = 5.0; return boxSize; }
            if(price < 25000.0) { boxSize = 50.0; return boxSize; }
            boxSize = 500.0; return boxSize;
        }
    }
}

double ChartSystem::PointAndFigureChart::RoundToBoxSize(const double price, const bool roundUp)
{
    const double boxSize = CalculateBoxSize(price);
    if(roundUp)
        return std::ceil(price / boxSize) * boxSize;

    return std::floor(price / boxSize) * boxSize;
}

bool ChartSystem::PointAndFigureChart::IsReversal(const double price, const PnFColumn* currentColumn, BoxType& newBoxType)
{
    if(currentColumn == nullptr || currentColumn->GetBoxCount() == 0)
        return false;

    const double boxSize = CalculateBoxSize(price);
    const double highestPrice = currentColumn->GetHighestPrice();
    const double lowestPrice = currentColumn->GetLowestPrice();

    if(const ColumnType columnType = currentColumn->GetColumnType(); columnType == COLUMN_X)
    {
        if(const double reversalLevel = highestPrice - (reversalCount * boxSize); price <= reversalLevel)
        {
            newBoxType = O;
            return true;
        }
    }
    else if(columnType == COLUMN_O)
    {
        if(const double reversalLevel = lowestPrice + (reversalCount * boxSize); price >= reversalLevel)
        {
            newBoxType = X;
            return true;
        }
    }
    else if(columnType == COLUMN_MIXED && reversalCount == 1)
    {
        if(price > highestPrice + boxSize)
        {
            newBoxType = X;
            return true;
        }
        if(price < lowestPrice - boxSize)
        {
            newBoxType = O;
            return true;
        }
    }

    return false;
}

bool ChartSystem::PointAndFigureChart::ProcessHighLowData(const double high, const double low, const std::chrono::system_clock::time_point time)
{
    lastDateTime = time;
    const std::string monthMarker = GetMonthMarkerForCurrentData(time);
    const bool monthChanged = (!monthMarker.empty());

    PnFColumn* lastColumn = GetLastColumn();
    const double boxSize = CalculateBoxSize(high);

    if (lastColumn == nullptr)
    {
        auto newColumn = std::make_unique<PnFColumn>(COLUMN_X);

        const double startPrice = RoundToBoxSize(high, false);
        if (monthChanged)
            newColumn->AddBox(startPrice, X, monthMarker);
        else
            newColumn->AddBox(startPrice, X);

        columns.push_back(std::move(newColumn));
        lastProcessedTime = time;
        return true;
    }

    BoxType reversalBoxType;
    const bool isReversalHigh = IsReversal(high, lastColumn, reversalBoxType);

    if (const bool isReversalLow = IsReversal(low, lastColumn, reversalBoxType); isReversalHigh || isReversalLow)
    {
        const double reversalPrice = isReversalHigh ? high : low;
        ColumnType newColumnType = (reversalBoxType == X) ? COLUMN_X : COLUMN_O;

        if (reversalCount == 1)
            newColumnType = COLUMN_MIXED;

        auto newColumn = std::make_unique<PnFColumn>(newColumnType);

        if (reversalBoxType == X)
        {
            double currentPrice = lastColumn->GetLowestPrice() + boxSize;
            bool markerApplied = false;
            while (currentPrice <= RoundToBoxSize(reversalPrice, true))
            {
                if (monthChanged && !markerApplied)
                {
                    newColumn->AddBox(currentPrice, X, monthMarker);
                    markerApplied = true;
                }
                else
                    newColumn->AddBox(currentPrice, X);
                currentPrice += boxSize;
            }
        }
        else
        {
            double currentPrice = lastColumn->GetHighestPrice() - boxSize;
            bool markerApplied = false;
            while (currentPrice >= RoundToBoxSize(reversalPrice, false))
            {
                if (monthChanged && !markerApplied)
                {
                    newColumn->AddBox(currentPrice, O, monthMarker);
                    markerApplied = true;
                }
                else
                    newColumn->AddBox(currentPrice, O);
                currentPrice -= boxSize;
            }
        }

        columns.push_back(std::move(newColumn));

        if (trendLineManager == nullptr)
        {
            lastProcessedTime = time;
            return false;
        }

        trendLineManager->UpdateTrendLines(columns, static_cast<int>(columns.size()) - 1);
    }
    else
    {
        if (const ColumnType columnType = lastColumn->GetColumnType(); columnType == COLUMN_X && high > lastColumn->GetHighestPrice())
        {
            double currentPrice = lastColumn->GetHighestPrice() + boxSize;
            bool markerApplied = false;
            while (currentPrice <= RoundToBoxSize(high, true))
            {
                if (monthChanged && !markerApplied)
                {
                    lastColumn->AddBox(currentPrice, X, monthMarker);
                    markerApplied = true;
                }
                else
                    lastColumn->AddBox(currentPrice, X);
                currentPrice += boxSize;
            }
        }
        else if (columnType == COLUMN_O && low < lastColumn->GetLowestPrice())
        {
            double currentPrice = lastColumn->GetLowestPrice() - boxSize;
            bool markerApplied = false;
            while (currentPrice >= RoundToBoxSize(low, false))
            {
                if (monthChanged && !markerApplied)
                {
                    lastColumn->AddBox(currentPrice, O, monthMarker);
                    markerApplied = true;
                }
                else
                    lastColumn->AddBox(currentPrice, O);
                currentPrice -= boxSize;
            }
        }
    }

    lastProcessedTime = time;
    return true;
}

bool ChartSystem::PointAndFigureChart::ProcessClosingPriceData(const double close, const std::chrono::system_clock::time_point time)
{
    lastDateTime = time;
    const std::string monthMarker = GetMonthMarkerForCurrentData(time);
    const bool monthChanged = (!monthMarker.empty());

    PnFColumn* lastColumn = GetLastColumn();
    const double boxSize = CalculateBoxSize(close);

    if (lastColumn == nullptr)
    {
        auto newColumn = std::make_unique<PnFColumn>(COLUMN_X);

        const double startPrice = RoundToBoxSize(close, false);
        if (monthChanged)
            newColumn->AddBox(startPrice, X, monthMarker);
        else
            newColumn->AddBox(startPrice, X);

        columns.push_back(std::move(newColumn));
        lastProcessedTime = time;
        return true;
    }

    if (BoxType reversalBoxType; IsReversal(close, lastColumn, reversalBoxType))
    {
        ColumnType newColumnType = (reversalBoxType == X) ? COLUMN_X : COLUMN_O;

        if (reversalCount == 1)
            newColumnType = COLUMN_MIXED;

        auto newColumn = std::make_unique<PnFColumn>(newColumnType);

        if (reversalBoxType == X)
        {
            double currentPrice = lastColumn->GetLowestPrice() + boxSize;
            bool markerApplied = false;
            while (currentPrice <= RoundToBoxSize(close, true))
            {
                if (monthChanged && !markerApplied)
                {
                    newColumn->AddBox(currentPrice, X, monthMarker);
                    markerApplied = true;
                }
                else
                    newColumn->AddBox(currentPrice, X);
                currentPrice += boxSize;
            }
        }
        else
        {
            double currentPrice = lastColumn->GetHighestPrice() - boxSize;
            bool markerApplied = false;
            while (currentPrice >= RoundToBoxSize(close, false))
            {
                if (monthChanged && !markerApplied)
                {
                    newColumn->AddBox(currentPrice, O, monthMarker);
                    markerApplied = true;
                }
                else
                    newColumn->AddBox(currentPrice, O);
                currentPrice -= boxSize;
            }
        }

        columns.push_back(std::move(newColumn));

        if (trendLineManager == nullptr)
        {
            lastProcessedTime = time;
            return false;
        }

        trendLineManager->UpdateTrendLines(columns, static_cast<int>(columns.size()) - 1);
    }
    else
    {
        if (const ColumnType columnType = lastColumn->GetColumnType(); (columnType == COLUMN_X || columnType == COLUMN_MIXED) && close > lastColumn->GetHighestPrice())
        {
            double currentPrice = lastColumn->GetHighestPrice() + boxSize;
            bool markerApplied = false;
            while (currentPrice <= RoundToBoxSize(close, true))
            {
                const BoxType boxType = (columnType == COLUMN_MIXED) ?
                                  ((currentPrice > lastColumn->GetLowestPrice()) ? X : O) : X;

                if (monthChanged && !markerApplied)
                {
                    lastColumn->AddBox(currentPrice, boxType, monthMarker);
                    markerApplied = true;
                }
                else
                    lastColumn->AddBox(currentPrice, boxType);
                currentPrice += boxSize;
            }
        }
        else if ((columnType == COLUMN_O || columnType == COLUMN_MIXED) && close < lastColumn->GetLowestPrice())
        {
            double currentPrice = lastColumn->GetLowestPrice() - boxSize;
            bool markerApplied = false;
            while (currentPrice >= RoundToBoxSize(close, false))
            {
                const BoxType boxType = (columnType == COLUMN_MIXED) ?
                                  ((currentPrice < lastColumn->GetHighestPrice()) ? O : X) :
                                  O;

                if (monthChanged && !markerApplied)
                {
                    lastColumn->AddBox(currentPrice, boxType, monthMarker);
                    markerApplied = true;
                }
                else
                    lastColumn->AddBox(currentPrice, boxType);
                currentPrice -= boxSize;
            }
        }
    }

    lastProcessedTime = time;
    return true;
}

ChartSystem::PnFColumn* ChartSystem::PointAndFigureChart::GetLastColumn() const
{
    if (columns.empty())
        return nullptr;

    PnFColumn* column = columns.back().get();
    return column;
}

bool ChartSystem::PointAndFigureChart::AddData(const double high, const double low, const double close, const std::chrono::system_clock::time_point time)
{
    if(constructionType == HIGH_LOW)
        return ProcessHighLowData(high, low, time);

    return ProcessClosingPriceData(close, time);
}

bool ChartSystem::PointAndFigureChart::AddData(const double price, const std::chrono::system_clock::time_point time)
{
    return AddData(price, price, price, time);
}

int ChartSystem::PointAndFigureChart::GetColumnCount() const
{
    return static_cast<int>(columns.size());
}

ChartSystem::PnFColumn* ChartSystem::PointAndFigureChart::GetColumn(const int columnIndex) const
{
    PnFColumn* column = columns[columnIndex].get();
    return column;
}

bool ChartSystem::PointAndFigureChart::HasBullishBias() const
{
    return (trendLineManager != nullptr) ? trendLineManager->HasBullishBias() : false;
}

bool ChartSystem::PointAndFigureChart::HasBearishBias() const
{
    return (trendLineManager != nullptr) ? trendLineManager->HasBearishBias() : false;
}

bool ChartSystem::PointAndFigureChart::ShouldTakeBullishSignals() const
{
    return HasBullishBias() || (!HasBullishBias() && !HasBearishBias());
}

bool ChartSystem::PointAndFigureChart::ShouldTakeBearishSignals() const
{
    return HasBearishBias() || (!HasBullishBias() && !HasBearishBias());
}

bool ChartSystem::PointAndFigureChart::IsAboveBullishSupport(const double price) const
{
    if (trendLineManager == nullptr)
        return false;

    const int currentColumnIndex = static_cast<int>(columns.size()) - 1;
    return trendLineManager->IsAboveBullishSupport(currentColumnIndex, price);
}

bool ChartSystem::PointAndFigureChart::IsBelowBearishResistance(const double price) const
{
    if (trendLineManager == nullptr)
        return false;

    const int currentColumnIndex = static_cast<int>(columns.size()) - 1;
    return trendLineManager->IsBelowBearishResistance(currentColumnIndex, price);
}

void ChartSystem::PointAndFigureChart::Clear()
{
    columns.clear();
    lastMonth = -1;
    lastProcessedTime = std::chrono::system_clock::now();
}

int ChartSystem::PointAndFigureChart::GetXColumnCount() const
{
    int count = 0;
    for (const auto & i : columns)
    {
        if (const PnFColumn* column = i.get(); column->GetColumnType() == COLUMN_X)
            count++;
    }

    return count;
}

int ChartSystem::PointAndFigureChart::GetOColumnCount() const
{
    int count = 0;
    for (const auto & i : columns)
    {
        if (const PnFColumn* column = i.get(); column->GetColumnType() == COLUMN_O)
            count++;
    }

    return count;
}

int ChartSystem::PointAndFigureChart::GetMixedColumnCount() const
{
    int count = 0;
    for (const auto & i : columns)
    {
        if (const PnFColumn* column = i.get(); column->GetColumnType() == COLUMN_MIXED)
            count++;
    }

    return count;
}

std::vector<int> ChartSystem::PointAndFigureChart::GetXColumnIndices() const
{
    std::vector<int> indices;
    indices.reserve(columns.size());

    for (int i = 0; i < static_cast<int>(columns.size()); ++i) {
        if (const PnFColumn* column = columns[i].get(); column->GetColumnType() == COLUMN_X) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::vector<int> ChartSystem::PointAndFigureChart::GetOColumnIndices() const
{
    std::vector<int> indices;
    indices.reserve(columns.size());

    for (int i = 0; i < static_cast<int>(columns.size()); ++i) {
        if (const PnFColumn* column = columns[i].get(); column->GetColumnType() == COLUMN_O) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::vector<int> ChartSystem::PointAndFigureChart::GetMixedColumnIndices() const
{
    std::vector<int> indices;
    indices.reserve(columns.size());

    for (int i = 0; i < static_cast<int>(columns.size()); ++i) {
        if (const PnFColumn* column = columns[i].get(); column->GetColumnType() == COLUMN_MIXED) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::vector<double> ChartSystem::PointAndFigureChart::GetAllPrices() const
{
    std::vector<double> allPrices;
    allPrices.reserve(columns.size());

    for (const auto &colPtr : columns) {
        const PnFColumn* column = colPtr.get();
        if (!column) continue;

        const int boxCount = column->GetBoxCount();
        for (int j = 0; j < boxCount; ++j) {
            const PnFBox* box = column->GetBoxAt(j);
            if (!box) continue;

            double price = box->GetPrice();
            bool found = false;
            for (const double existing : allPrices) {
                if (constexpr double eps = 0.00001; std::fabs(existing - price) < eps) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allPrices.push_back(price);
            }
        }
    }

    std::ranges::sort(allPrices, std::greater<>());

    return allPrices;
}

std::string ChartSystem::PointAndFigureChart::ToString() const
{
    std::string constructionStr = (constructionType == CLOSING_PRICE) ? "Closing Price" : "High/Low";
    std::string boxSizeStr;

    switch(boxSizeType)
    {
    case FIXED: boxSizeStr = "Fixed"; break;
    case PERCENTAGE: boxSizeStr = "Percentage"; break;
    case POINTS: boxSizeStr = "Points"; break;
    default: boxSizeStr = "Default"; break;
    }

    std::string result = "Point & Figure Chart\n";
    result += std::format("Construction: {}, Box Size: {} ({:.5f}), Reversal: {}\n",
                         constructionStr, boxSizeStr, boxSize, reversalCount);
    result += std::format("Columns: {}\n", columns.size());

    if (trendLineManager != nullptr)
    {
        result += std::format("Trend Bias: {}\n",
                    HasBullishBias() ? "BULLISH" : HasBearishBias() ? "BEARISH" : "NONE");
    }
    result += "\n";

    for (size_t i = 0; i < columns.size(); i++)
    {
        if (columns[i] != nullptr)
        {
            result += std::format("Column {}:\n", i + 1);
            result += columns[i]->ToString() + "\n";
        }
    }

    if (trendLineManager != nullptr)
        result += trendLineManager->ToString();

    return result;
}

bool ChartSystem::PointAndFigureChart::ExportToExcel(const std::string& filename) const
{
    using namespace OpenXLSX;

    const std::filesystem::path excelsDir = "excels";
    if (!std::filesystem::exists(excelsDir))
    {
        std::filesystem::create_directories(excelsDir);
    }

    const std::filesystem::path fullPath = excelsDir / filename;

    XLDocument doc;
    doc.create(fullPath.string(), true);
    auto wks = doc.workbook().worksheet("Sheet1");
    wks.setName("P&F Chart");

    const std::vector<double> allPrices = GetAllPrices();

    wks.cell("A1").value() = "Price";
    for (int col = 0; col < static_cast<int>(columns.size()); col++)
    {
        wks.cell(1, col + 2).value() = col + 1;
    }
    wks.cell(1, static_cast<int>(columns.size()) + 2).value() = "Price";

    for (int row = 0; row < static_cast<int>(allPrices.size()); row++)
    {
        const double price = allPrices[row];

        wks.cell(row + 2, 1).value() = price;
        wks.cell(row + 2, static_cast<int>(columns.size()) + 2).value() = price;

        for (int col = 0; col < static_cast<int>(columns.size()); col++)
        {
            const PnFColumn* column = columns[col].get();
            if (column == nullptr) continue;

            const PnFBox* box = nullptr;
            for (int b = 0; b < column->GetBoxCount(); b++)
            {
                if (const PnFBox* tempBox = column->GetBoxAt(b); tempBox != nullptr && std::abs(tempBox->GetPrice() - price) < 0.00001)
                {
                    box = tempBox;
                    break;
                }
            }

            if (box != nullptr)
            {
                auto cell = wks.cell(row + 2, col + 2);

                if (const std::string marker = box->GetMarker(); !marker.empty())
                {
                    cell.value() = marker;
                }
                else
                {
                    if (const BoxType boxType = box->GetBoxType(); boxType == X)
                        cell.value() = "X";
                    else if (boxType == O)
                        cell.value() = "O";
                }
            }
        }
    }

    doc.save();
    doc.close();

    return true;
}