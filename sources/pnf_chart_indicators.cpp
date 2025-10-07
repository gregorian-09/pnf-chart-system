//
// Created by gregorian on 06/10/2025.
//

#include "pnf_chart_indicators.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <format>

namespace ChartSystem
{
    PnFMovingAverage::PnFMovingAverage(const int period) : period(period) {}

    double PnFMovingAverage::CalculateColumnAverage(const PnFColumn* column)
    {
        if (column == nullptr || column->GetBoxCount() == 0)
            return 0.0;

        return (column->GetHighestPrice() + column->GetLowestPrice()) / 2.0;
    }

    void PnFMovingAverage::Calculate(const PointAndFigureChart* chart)
    {
        values.clear();
        const int columnCount = chart->GetColumnCount();

        for (int i = 0; i < columnCount; i++)
        {
            if (i < period - 1)
            {
                values.push_back(0.0);
                continue;
            }

            double sum = 0.0;
            for (int j = 0; j < period; j++)
            {
                const PnFColumn* column = chart->GetColumn(i - j);
                sum += CalculateColumnAverage(column);
            }

            values.push_back(sum / period);
        }
    }

    double PnFMovingAverage::GetValue(const int columnIndex) const
    {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(values.size()))
            return 0.0;
        return values[columnIndex];
    }

    bool PnFMovingAverage::HasValue(const int columnIndex) const
    {
        return columnIndex >= period - 1 && columnIndex < static_cast<int>(values.size());
    }

    std::string PnFMovingAverage::ToString() const
    {
        return std::format("SMA({}): {} values calculated", period, values.size());
    }

    PnFBollingerBands::PnFBollingerBands(const int period, const double stdDev)
        : period(period), standardDeviations(stdDev) {}

    double PnFBollingerBands::CalculateStandardDeviation(const std::vector<double>& values, const double mean)
    {
        double sumSquaredDiff = 0.0;
        for (const double value : values)
        {
            const double diff = value - mean;
            sumSquaredDiff += diff * diff;
        }
        return std::sqrt(sumSquaredDiff / static_cast<int>(values.size()));
    }

    void PnFBollingerBands::Calculate(const PointAndFigureChart* chart)
    {
        middleBand.clear();
        upperBand.clear();
        lowerBand.clear();

        const int columnCount = chart->GetColumnCount();

        for (int i = 0; i < columnCount; i++)
        {
            if (i < period - 1)
            {
                middleBand.push_back(0.0);
                upperBand.push_back(0.0);
                lowerBand.push_back(0.0);
                continue;
            }

            std::vector<double> columnAverages;
            for (int j = 0; j < period; j++)
            {
                const PnFColumn* column = chart->GetColumn(i - j);
                columnAverages.push_back((column->GetHighestPrice() + column->GetLowestPrice()) / 2.0);
            }

            const double mean = std::accumulate(columnAverages.begin(), columnAverages.end(), 0.0) / period;
            const double stdDev = CalculateStandardDeviation(columnAverages, mean);

            middleBand.push_back(mean);
            upperBand.push_back(mean + (standardDeviations * stdDev));
            lowerBand.push_back(mean - (standardDeviations * stdDev));
        }
    }

    double PnFBollingerBands::GetMiddleBand(const int columnIndex) const
    {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(middleBand.size()))
            return 0.0;
        return middleBand[columnIndex];
    }

    double PnFBollingerBands::GetUpperBand(const int columnIndex) const
    {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(upperBand.size()))
            return 0.0;
        return upperBand[columnIndex];
    }

    double PnFBollingerBands::GetLowerBand(const int columnIndex) const
    {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(lowerBand.size()))
            return 0.0;
        return lowerBand[columnIndex];
    }

    bool PnFBollingerBands::HasValue(const int columnIndex) const
    {
        return columnIndex >= period - 1 && columnIndex < static_cast<int>(middleBand.size());
    }

    bool PnFBollingerBands::IsAboveUpperBand(const int columnIndex, const double price) const
    {
        if (!HasValue(columnIndex))
            return false;
        return price > upperBand[columnIndex];
    }

    bool PnFBollingerBands::IsBelowLowerBand(const int columnIndex, const double price) const
    {
        if (!HasValue(columnIndex))
            return false;
        return price < lowerBand[columnIndex];
    }

    std::string PnFBollingerBands::ToString() const
    {
        return std::format("Bollinger Bands({}, {}): {} values", period, standardDeviations, middleBand.size());
    }

    PnFSignalDetector::PnFSignalDetector() : currentSignal(SIGNAL_NONE) {}

    bool PnFSignalDetector::IsBuySignal(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        int previousXColIndex = -1;
        for (int i = columnIndex - 1; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
            {
                previousXColIndex = i;
                break;
            }
        }

        if (previousXColIndex == -1)
            return false;

        const PnFColumn* previousXCol = chart->GetColumn(previousXColIndex);
        return currentCol->GetHighestPrice() > previousXCol->GetHighestPrice();
    }

    bool PnFSignalDetector::IsSellSignal(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_O)
            return false;

        int previousOColIndex = -1;
        for (int i = columnIndex - 1; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
            {
                previousOColIndex = i;
                break;
            }
        }

        if (previousOColIndex == -1)
            return false;

        const PnFColumn* previousOCol = chart->GetColumn(previousOColIndex);
        return currentCol->GetLowestPrice() < previousOCol->GetLowestPrice();
    }

    void PnFSignalDetector::DetectSignals(const PointAndFigureChart* chart)
    {
        signals.clear();
        currentSignal = SIGNAL_NONE;

        const int columnCount = chart->GetColumnCount();
        for (int i = 0; i < columnCount; i++)
        {
            if (IsBuySignal(chart, i))
            {
                const PnFColumn* col = chart->GetColumn(i);
                signals.emplace_back(SIGNAL_BUY, i, col->GetHighestPrice(), std::chrono::system_clock::now());
                currentSignal = SIGNAL_BUY;
            }
            else if (IsSellSignal(chart, i))
            {
                const PnFColumn* col = chart->GetColumn(i);
                signals.emplace_back(SIGNAL_SELL, i, col->GetLowestPrice(), std::chrono::system_clock::now());
                currentSignal = SIGNAL_SELL;
            }
        }
    }

    Signal PnFSignalDetector::GetLastSignal() const
    {
        if (signals.empty())
            return Signal{SIGNAL_NONE, -1, 0.0, std::chrono::system_clock::now()};
        return signals.back();
    }

    std::string PnFSignalDetector::ToString() const
    {
        std::string result = std::format("Signal Detector: {} total signals\n", signals.size());
        result += "Current Signal: ";
        result += (currentSignal == SIGNAL_BUY) ? "BUY" : (currentSignal == SIGNAL_SELL) ? "SELL" : "NONE";
        return result;
    }

    PnFPatternRecognizer::PnFPatternRecognizer() = default;

    bool PnFPatternRecognizer::DetectDoubleTopBreakout(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        int prevXIndex = -1;
        for (int i = columnIndex - 1; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
            {
                prevXIndex = i;
                break;
            }
        }

        if (prevXIndex == -1)
            return false;

        const PnFColumn* prevXCol = chart->GetColumn(prevXIndex);
        const double prevHigh = prevXCol->GetHighestPrice();

        if (const double currentHigh = currentCol->GetHighestPrice(); currentHigh > prevHigh)
        {
            patterns.emplace_back(PATTERN_DOUBLE_TOP_BREAKOUT, prevXIndex, columnIndex, currentHigh);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectAscendingTripleTop(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_X)
            return false;

        std::vector<int> xIndices;
        for (int i = columnIndex; i >= 0 && xIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                xIndices.push_back(i);
        }

        if (xIndices.size() < 3)
            return false;

        const double high0 = chart->GetColumn(xIndices[0])->GetHighestPrice();
        const double high1 = chart->GetColumn(xIndices[1])->GetHighestPrice();

        if (const double high2 = chart->GetColumn(xIndices[2])->GetHighestPrice(); high0 > high1 && high1 > high2)
        {
            patterns.emplace_back(PATTERN_ASCENDING_TRIPLE_TOP, xIndices[2], columnIndex, high0);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectDescendingTripleBottom(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_O)
            return false;

        std::vector<int> oIndices;
        for (int i = columnIndex; i >= 0 && oIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                oIndices.push_back(i);
        }

        if (oIndices.size() < 3)
            return false;

        const double low0 = chart->GetColumn(oIndices[0])->GetLowestPrice();
        const double low1 = chart->GetColumn(oIndices[1])->GetLowestPrice();

        if (const double low2 = chart->GetColumn(oIndices[2])->GetLowestPrice(); low0 < low1 && low1 < low2)
        {
            patterns.emplace_back(PATTERN_DESCENDING_TRIPLE_BOTTOM, oIndices[2], columnIndex, low0);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBullishCatapult(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (patterns.size() < 2)
            return false;

        if (const Pattern& lastPattern = patterns[patterns.size() - 1]; lastPattern.type == PATTERN_DOUBLE_TOP_BREAKOUT && lastPattern.endColumnIndex == columnIndex)
        {
            if (const Pattern& secondLastPattern = patterns[patterns.size() - 2]; secondLastPattern.type == PATTERN_TRIPLE_TOP_BREAKOUT)
            {
                patterns.emplace_back(PATTERN_BULLISH_CATAPULT, secondLastPattern.startColumnIndex, columnIndex, lastPattern.price);
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBearishCatapult(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (patterns.size() < 2)
            return false;

        if (const Pattern& lastPattern = patterns[patterns.size() - 1]; lastPattern.type == PATTERN_DOUBLE_BOTTOM_BREAKDOWN && lastPattern.endColumnIndex == columnIndex)
        {
            if (const Pattern& secondLastPattern = patterns[patterns.size() - 2]; secondLastPattern.type == PATTERN_TRIPLE_BOTTOM_BREAKDOWN)
            {
                patterns.emplace_back(PATTERN_BEARISH_CATAPULT, secondLastPattern.startColumnIndex, columnIndex, lastPattern.price);
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBullishSignalReversed(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_O)
            return false;

        bool risingTopsBottoms = true;
        for (int i = columnIndex - 1; i >= columnIndex - 5 && i >= 0; i--)
        {
            const PnFColumn* col1 = chart->GetColumn(i);

            if (const PnFColumn* col2 = chart->GetColumn(i + 1); col1->GetHighestPrice() >= col2->GetHighestPrice() ||
                col1->GetLowestPrice() >= col2->GetLowestPrice())
            {
                risingTopsBottoms = false;
                break;
            }
        }

        if (risingTopsBottoms)
        {
            int prevOIndex = -1;
            for (int i = columnIndex - 1; i >= 0; i--)
            {
                if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                {
                    prevOIndex = i;
                    break;
                }
            }

            if (prevOIndex != -1 && currentCol->GetLowestPrice() < chart->GetColumn(prevOIndex)->GetLowestPrice())
            {
                patterns.emplace_back(PATTERN_BULLISH_SIGNAL_REVERSED, columnIndex - 5, columnIndex, currentCol->GetLowestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBearishSignalReversed(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        bool fallingTopsBottoms = true;
        for (int i = columnIndex - 1; i >= columnIndex - 5 && i >= 0; i--)
        {
            const PnFColumn* col1 = chart->GetColumn(i);

            if (const PnFColumn* col2 = chart->GetColumn(i + 1); col1->GetHighestPrice() <= col2->GetHighestPrice() ||
                col1->GetLowestPrice() <= col2->GetLowestPrice())
            {
                fallingTopsBottoms = false;
                break;
            }
        }

        if (fallingTopsBottoms)
        {
            int prevXIndex = -1;
            for (int i = columnIndex - 1; i >= 0; i--)
            {
                if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                {
                    prevXIndex = i;
                    break;
                }
            }

            if (prevXIndex != -1 && currentCol->GetHighestPrice() > chart->GetColumn(prevXIndex)->GetHighestPrice())
            {
                patterns.emplace_back(PATTERN_BEARISH_SIGNAL_REVERSED, columnIndex - 5, columnIndex, currentCol->GetHighestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBullishTriangle(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        bool risingBottoms = true;
        bool fallingTops = true;

        for (int i = columnIndex - 1; i >= columnIndex - 5 && i >= 0; i--)
        {
            const PnFColumn* col1 = chart->GetColumn(i);
            const PnFColumn* col2 = chart->GetColumn(i + 1);

            if (col1->GetLowestPrice() >= col2->GetLowestPrice())
                risingBottoms = false;
            if (col1->GetHighestPrice() <= col2->GetHighestPrice())
                fallingTops = false;
        }

        if (risingBottoms && fallingTops)
        {
            int prevXIndex = -1;
            for (int i = columnIndex - 1; i >= 0; i--)
            {
                if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                {
                    prevXIndex = i;
                    break;
                }
            }

            if (prevXIndex != -1 && currentCol->GetHighestPrice() > chart->GetColumn(prevXIndex)->GetHighestPrice())
            {
                patterns.emplace_back(PATTERN_BULLISH_TRIANGLE, columnIndex - 5, columnIndex, currentCol->GetHighestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBearishTriangle(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_O)
            return false;

        bool risingBottoms = true;
        bool fallingTops = true;

        for (int i = columnIndex - 1; i >= columnIndex - 5 && i >= 0; i--)
        {
            const PnFColumn* col1 = chart->GetColumn(i);
            const PnFColumn* col2 = chart->GetColumn(i + 1);

            if (col1->GetLowestPrice() >= col2->GetLowestPrice())
                risingBottoms = false;
            if (col1->GetHighestPrice() <= col2->GetHighestPrice())
                fallingTops = false;
        }

        if (risingBottoms && fallingTops)
        {
            int prevOIndex = -1;
            for (int i = columnIndex - 1; i >= 0; i--)
            {
                if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                {
                    prevOIndex = i;
                    break;
                }
            }

            if (prevOIndex != -1 && currentCol->GetLowestPrice() < chart->GetColumn(prevOIndex)->GetLowestPrice())
            {
                patterns.emplace_back(PATTERN_BEARISH_TRIANGLE, columnIndex - 5, columnIndex, currentCol->GetLowestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectLongTailDown(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 1)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        if (const PnFColumn* prevCol = chart->GetColumn(columnIndex - 1); prevCol->GetColumnType() != COLUMN_O || prevCol->GetBoxCount() < 20)
            return false;

        patterns.emplace_back(PATTERN_LONG_TAIL_DOWN, columnIndex - 1, columnIndex, currentCol->GetHighestPrice());
        return true;
    }

    bool PnFPatternRecognizer::DetectHighPole(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_O)
            return false;

        const PnFColumn* prevXCol = chart->GetColumn(columnIndex - 1);
        if (prevXCol->GetColumnType() != COLUMN_X)
            return false;

        double previousHigh = 0.0;
        for (int i = columnIndex - 2; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
            {
                previousHigh = col->GetHighestPrice();
                break;
            }
        }

        if (previousHigh > 0.0)
        {
            const double rise = prevXCol->GetHighestPrice() - previousHigh;
            const double boxSize = std::abs(prevXCol->GetBoxAt(prevXCol->GetBoxCount() - 1)->GetPrice() - prevXCol->GetBoxAt(prevXCol->GetBoxCount() - 2)->GetPrice());

            if (const double retracement = prevXCol->GetHighestPrice() - currentCol->GetLowestPrice(); rise >= 3 * boxSize && retracement >= rise * 0.5)
            {
                patterns.emplace_back(PATTERN_HIGH_POLE, columnIndex - 1, columnIndex, prevXCol->GetHighestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectLowPole(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_X)
            return false;

        const PnFColumn* prevOCol = chart->GetColumn(columnIndex - 1);
        if (prevOCol->GetColumnType() != COLUMN_O)
            return false;

        double previousLow = 0.0;
        for (int i = columnIndex - 2; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
            {
                previousLow = col->GetLowestPrice();
                break;
            }
        }

        if (previousLow > 0.0)
        {
            const double fall = previousLow - prevOCol->GetLowestPrice();
            const double boxSize = std::abs(prevOCol->GetBoxAt(prevOCol->GetBoxCount() - 2)->GetPrice() - prevOCol->GetBoxAt(prevOCol->GetBoxCount() - 1)->GetPrice());

            if (const double retracement = currentCol->GetHighestPrice() - prevOCol->GetLowestPrice(); fall >= 3 * boxSize && retracement >= fall * 0.5)
            {
                patterns.emplace_back(PATTERN_LOW_POLE, columnIndex - 1, columnIndex, prevOCol->GetLowestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBullTrap(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_O)
            return false;

        const PnFColumn* prevCol = chart->GetColumn(columnIndex - 1);
        if (prevCol->GetColumnType() != COLUMN_X || prevCol->GetBoxCount() != 1)
            return false;

        std::vector<int> xIndices;
        for (int i = columnIndex - 2; i >= 0 && xIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                xIndices.push_back(i);
        }

        if (xIndices.size() >= 2)
        {
            const double high0 = chart->GetColumn(xIndices[0])->GetHighestPrice();
            const double high1 = chart->GetColumn(xIndices[1])->GetHighestPrice();

            if (constexpr double tolerance = 0.0001; std::abs(high0 - high1) < tolerance && prevCol->GetHighestPrice() > high0)
            {
                patterns.emplace_back(PATTERN_BULL_TRAP, xIndices[1], columnIndex, prevCol->GetHighestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectBearTrap(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_X)
            return false;

        const PnFColumn* prevCol = chart->GetColumn(columnIndex - 1);
        if (prevCol->GetColumnType() != COLUMN_O || prevCol->GetBoxCount() != 1)
            return false;

        std::vector<int> oIndices;
        for (int i = columnIndex - 2; i >= 0 && oIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                oIndices.push_back(i);
        }

        if (oIndices.size() >= 2)
        {
            const double low0 = chart->GetColumn(oIndices[0])->GetLowestPrice();
            const double low1 = chart->GetColumn(oIndices[1])->GetLowestPrice();

            if (constexpr double tolerance = 0.0001; std::abs(low0 - low1) < tolerance && prevCol->GetLowestPrice() < low0)
            {
                patterns.emplace_back(PATTERN_BEAR_TRAP, oIndices[1], columnIndex, prevCol->GetLowestPrice());
                return true;
            }
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectSpreadTripleTop(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_X)
            return false;

        std::vector<int> xIndices;
        for (int i = columnIndex; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                xIndices.push_back(i);
        }

        if (xIndices.size() < 3)
            return false;

        const double currentHigh = chart->GetColumn(xIndices[0])->GetHighestPrice();

        int matchCount = 0;
        for (size_t i = 1; i < xIndices.size() && matchCount < 2; i++)
        {
            constexpr double tolerance = 0.0001;
            if (const double high = chart->GetColumn(xIndices[i])->GetHighestPrice(); std::abs(high - currentHigh) < tolerance)
                matchCount++;
        }

        if (matchCount >= 2)
        {
            patterns.emplace_back(PATTERN_SPREAD_TRIPLE_TOP, xIndices[2], columnIndex, currentHigh);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectSpreadTripleBottom(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_O)
            return false;

        std::vector<int> oIndices;
        for (int i = columnIndex; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                oIndices.push_back(i);
        }

        if (oIndices.size() < 3)
            return false;

        const double currentLow = chart->GetColumn(oIndices[0])->GetLowestPrice();

        int matchCount = 0;
        for (size_t i = 1; i < oIndices.size() && matchCount < 2; i++)
        {
            constexpr double tolerance = 0.0001;
            if (const double low = chart->GetColumn(oIndices[i])->GetLowestPrice(); std::abs(low - currentLow) < tolerance)
                matchCount++;
        }

        if (matchCount >= 2)
        {
            patterns.emplace_back(PATTERN_SPREAD_TRIPLE_BOTTOM, oIndices[2], columnIndex, currentLow);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectDoubleBottomBreakdown(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return false;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        if (currentCol->GetColumnType() != COLUMN_O)
            return false;

        int prevOIndex = -1;
        for (int i = columnIndex - 1; i >= 0; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
            {
                prevOIndex = i;
                break;
            }
        }

        if (prevOIndex == -1)
            return false;

        const PnFColumn* prevOCol = chart->GetColumn(prevOIndex);
        const double prevLow = prevOCol->GetLowestPrice();

        if (const double currentLow = currentCol->GetLowestPrice(); currentLow < prevLow)
        {
            patterns.emplace_back(PATTERN_DOUBLE_BOTTOM_BREAKDOWN, prevOIndex, columnIndex, currentLow);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectTripleTopBreakout(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_X)
            return false;

        std::vector<int> xIndices;
        for (int i = columnIndex; i >= 0 && xIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                xIndices.push_back(i);
        }

        if (xIndices.size() < 3)
            return false;

        const double high0 = chart->GetColumn(xIndices[0])->GetHighestPrice();
        const double high1 = chart->GetColumn(xIndices[1])->GetHighestPrice();
        const double high2 = chart->GetColumn(xIndices[2])->GetHighestPrice();

        if (constexpr double tolerance = 0.0001; std::abs(high1 - high2) < tolerance && high0 > high1)
        {
            patterns.emplace_back(PATTERN_TRIPLE_TOP_BREAKOUT, xIndices[2], columnIndex, high0);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectTripleBottomBreakdown(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 4)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_O)
            return false;

        std::vector<int> oIndices;
        for (int i = columnIndex; i >= 0 && oIndices.size() < 3; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                oIndices.push_back(i);
        }

        if (oIndices.size() < 3)
            return false;

        const double low0 = chart->GetColumn(oIndices[0])->GetLowestPrice();
        const double low1 = chart->GetColumn(oIndices[1])->GetLowestPrice();
        const double low2 = chart->GetColumn(oIndices[2])->GetLowestPrice();

        if (constexpr double tolerance = 0.0001; std::abs(low1 - low2) < tolerance && low0 < low1)
        {
            patterns.emplace_back(PATTERN_TRIPLE_BOTTOM_BREAKDOWN, oIndices[2], columnIndex, low0);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectQuadrupleTopBreakout(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_X)
            return false;

        std::vector<int> xIndices;
        for (int i = columnIndex; i >= 0 && xIndices.size() < 4; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_X)
                xIndices.push_back(i);
        }

        if (xIndices.size() < 4)
            return false;

        const double high0 = chart->GetColumn(xIndices[0])->GetHighestPrice();
        const double high1 = chart->GetColumn(xIndices[1])->GetHighestPrice();
        const double high2 = chart->GetColumn(xIndices[2])->GetHighestPrice();
        const double high3 = chart->GetColumn(xIndices[3])->GetHighestPrice();

        if (constexpr double tolerance = 0.0001; std::abs(high1 - high2) < tolerance && std::abs(high2 - high3) < tolerance && high0 > high1)
        {
            patterns.emplace_back(PATTERN_QUADRUPLE_TOP_BREAKOUT, xIndices[3], columnIndex, high0);
            return true;
        }

        return false;
    }

    bool PnFPatternRecognizer::DetectQuadrupleBottomBreakdown(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 6)
            return false;

        if (const PnFColumn* currentCol = chart->GetColumn(columnIndex); currentCol->GetColumnType() != COLUMN_O)
            return false;

        std::vector<int> oIndices;
        for (int i = columnIndex; i >= 0 && oIndices.size() < 4; i--)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
                oIndices.push_back(i);
        }

        if (oIndices.size() < 4)
            return false;

        const double low0 = chart->GetColumn(oIndices[0])->GetLowestPrice();
        const double low1 = chart->GetColumn(oIndices[1])->GetLowestPrice();
        const double low2 = chart->GetColumn(oIndices[2])->GetLowestPrice();
        const double low3 = chart->GetColumn(oIndices[3])->GetLowestPrice();

        if (constexpr double tolerance = 0.0001; std::abs(low1 - low2) < tolerance && std::abs(low2 - low3) < tolerance && low0 < low1)
        {
            patterns.emplace_back(PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN, oIndices[3], columnIndex, low0);
            return true;
        }

        return false;
    }

    std::string PnFPatternRecognizer::PatternTypeToString(const PatternType type)
    {
        switch (type)
        {
            case PATTERN_NONE: return "None";
            case PATTERN_DOUBLE_TOP_BREAKOUT: return "Double Top Breakout";
            case PATTERN_DOUBLE_BOTTOM_BREAKDOWN: return "Double Bottom Breakdown";
            case PATTERN_TRIPLE_TOP_BREAKOUT: return "Triple Top Breakout";
            case PATTERN_TRIPLE_BOTTOM_BREAKDOWN: return "Triple Bottom Breakdown";
            case PATTERN_QUADRUPLE_TOP_BREAKOUT: return "Quadruple Top Breakout";
            case PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN: return "Quadruple Bottom Breakdown";
            case PATTERN_ASCENDING_TRIPLE_TOP: return "Ascending Triple Top";
            case PATTERN_DESCENDING_TRIPLE_BOTTOM: return "Descending Triple Bottom";
            case PATTERN_BULLISH_CATAPULT: return "Bullish Catapult";
            case PATTERN_BEARISH_CATAPULT: return "Bearish Catapult";
            case PATTERN_BULLISH_SIGNAL_REVERSED: return "Bullish Signal Reversed";
            case PATTERN_BEARISH_SIGNAL_REVERSED: return "Bearish Signal Reversed";
            case PATTERN_BULLISH_TRIANGLE: return "Bullish Triangle";
            case PATTERN_BEARISH_TRIANGLE: return "Bearish Triangle";
            case PATTERN_LONG_TAIL_DOWN: return "Long Tail Down";
            case PATTERN_HIGH_POLE: return "High Pole";
            case PATTERN_LOW_POLE: return "Low Pole";
            case PATTERN_BULL_TRAP: return "Bull Trap";
            case PATTERN_BEAR_TRAP: return "Bear Trap";
            case PATTERN_SPREAD_TRIPLE_TOP: return "Spread Triple Top";
            case PATTERN_SPREAD_TRIPLE_BOTTOM: return "Spread Triple Bottom";
            default: return "Unknown";
        }
    }

    bool PnFPatternRecognizer::IsBullishPattern(const PatternType type)
    {
        return type == PATTERN_DOUBLE_TOP_BREAKOUT ||
               type == PATTERN_TRIPLE_TOP_BREAKOUT ||
               type == PATTERN_QUADRUPLE_TOP_BREAKOUT ||
               type == PATTERN_ASCENDING_TRIPLE_TOP ||
               type == PATTERN_BULLISH_CATAPULT ||
               type == PATTERN_BULLISH_SIGNAL_REVERSED ||
               type == PATTERN_BULLISH_TRIANGLE ||
               type == PATTERN_LONG_TAIL_DOWN ||
               type == PATTERN_LOW_POLE ||
               type == PATTERN_BEAR_TRAP ||
               type == PATTERN_SPREAD_TRIPLE_TOP;
    }

    bool PnFPatternRecognizer::IsBearishPattern(const PatternType type)
    {
        return type == PATTERN_DOUBLE_BOTTOM_BREAKDOWN ||
               type == PATTERN_TRIPLE_BOTTOM_BREAKDOWN ||
               type == PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN ||
               type == PATTERN_DESCENDING_TRIPLE_BOTTOM ||
               type == PATTERN_BEARISH_CATAPULT ||
               type == PATTERN_BEARISH_SIGNAL_REVERSED ||
               type == PATTERN_BEARISH_TRIANGLE ||
               type == PATTERN_HIGH_POLE ||
               type == PATTERN_BULL_TRAP ||
               type == PATTERN_SPREAD_TRIPLE_BOTTOM;
    }

    void PnFPatternRecognizer::DetectPatterns(const PointAndFigureChart* chart)
    {
        patterns.clear();
        const int columnCount = chart->GetColumnCount();

        for (int i = 0; i < columnCount; i++)
        {
            DetectDoubleTopBreakout(chart, i);
            DetectDoubleBottomBreakdown(chart, i);
            DetectTripleTopBreakout(chart, i);
            DetectTripleBottomBreakdown(chart, i);
            DetectQuadrupleTopBreakout(chart, i);
            DetectQuadrupleBottomBreakdown(chart, i);
            DetectAscendingTripleTop(chart, i);
            DetectDescendingTripleBottom(chart, i);
            DetectBullishCatapult(chart, i);
            DetectBearishCatapult(chart, i);
            DetectBullishSignalReversed(chart, i);
            DetectBearishSignalReversed(chart, i);
            DetectBullishTriangle(chart, i);
            DetectBearishTriangle(chart, i);
            DetectLongTailDown(chart, i);
            DetectHighPole(chart, i);
            DetectLowPole(chart, i);
            DetectBullTrap(chart, i);
            DetectBearTrap(chart, i);
            DetectSpreadTripleTop(chart, i);
            DetectSpreadTripleBottom(chart, i);
        }
    }

    std::vector<Pattern> PnFPatternRecognizer::GetBullishPatterns() const
    {
        std::vector<Pattern> bullishPatterns;
        for (const auto& pattern : patterns)
        {
            if (IsBullishPattern(pattern.type))
                bullishPatterns.push_back(pattern);
        }
        return bullishPatterns;
    }

    std::vector<Pattern> PnFPatternRecognizer::GetBearishPatterns() const
    {
        std::vector<Pattern> bearishPatterns;
        for (const auto& pattern : patterns)
        {
            if (IsBearishPattern(pattern.type))
                bearishPatterns.push_back(pattern);
        }
        return bearishPatterns;
    }

    Pattern PnFPatternRecognizer::GetLatestPattern() const
    {
        if (patterns.empty())
            return Pattern{PATTERN_NONE, -1, -1, 0.0};
        return patterns.back();
    }

    bool PnFPatternRecognizer::HasPattern(PatternType type) const
    {
        return std::ranges::any_of(patterns,
                                   [type](const Pattern& p) { return p.type == type; });
    }

    std::string PnFPatternRecognizer::ToString() const
    {
        std::string result = std::format("Pattern Recognizer: {} patterns detected\n", patterns.size());
        for (const auto& pattern : patterns)
        {
            result += std::format("  {}: columns {}-{}, price {:.2f}\n",
                                PatternTypeToString(pattern.type),
                                pattern.startColumnIndex,
                                pattern.endColumnIndex,
                                pattern.price);
        }
        return result;
    }

    PnFSupportResistance::PnFSupportResistance(const double priceThreshold)
    : priceThreshold(priceThreshold) {}

    void PnFSupportResistance::IdentifyLevels(const PointAndFigureChart* chart)
    {
        levels.clear();

        const int columnCount = chart->GetColumnCount();
        for (int i = 0; i < columnCount; i++)
        {
            if (const PnFColumn* col = chart->GetColumn(i); col->GetColumnType() == COLUMN_O)
            {
                const double lowPrice = col->GetLowestPrice();
                bool found = false;

                for (auto& level : levels)
                {
                    if (level.isSupport && std::abs(level.price - lowPrice) / lowPrice < priceThreshold)
                    {
                        level.touchCount++;
                        level.lastColumnIndex = i;
                        found = true;
                        break;
                    }
                }

                if (!found)
                    levels.emplace_back(lowPrice, true, i);
            }
            else if (col->GetColumnType() == COLUMN_X)
            {
                const double highPrice = col->GetHighestPrice();
                bool found = false;

                for (auto& level : levels)
                {
                    if (!level.isSupport && std::abs(level.price - highPrice) / highPrice < priceThreshold)
                    {
                        level.touchCount++;
                        level.lastColumnIndex = i;
                        found = true;
                        break;
                    }
                }

                if (!found)
                    levels.emplace_back(highPrice, false, i);
            }
        }

        MergeSimilarLevels();
    }

    void PnFSupportResistance::MergeSimilarLevels()
    {
        using diff_t = std::vector<SupportResistanceLevel>::difference_type;

        const auto n = static_cast<diff_t>(levels.size());
        for (diff_t i = 0; i < n; ++i)
        {
            for (diff_t j = i + 1; j < static_cast<diff_t>(levels.size()); )
            {
                if (levels[i].isSupport == levels[j].isSupport &&
                    std::abs(levels[i].price - levels[j].price) / levels[i].price < priceThreshold)
                {
                    levels[i].touchCount += levels[j].touchCount;
                    levels[i].price = (levels[i].price * levels[i].touchCount + levels[j].price * levels[j].touchCount)
                                     / (levels[i].touchCount + levels[j].touchCount);

                    levels[i].lastColumnIndex = std::max(levels[i].lastColumnIndex, levels[j].lastColumnIndex);

                    auto it = levels.begin() + j;
                    levels.erase(it);
                }
                else
                {
                    ++j;
                }
            }
        }
    }

    std::vector<SupportResistanceLevel> PnFSupportResistance::GetSupportLevels() const
    {
        std::vector<SupportResistanceLevel> support;
        for (const auto& level : levels)
        {
            if (level.isSupport)
                support.push_back(level);
        }
        return support;
    }

    std::vector<SupportResistanceLevel> PnFSupportResistance::GetResistanceLevels() const
    {
        std::vector<SupportResistanceLevel> resistance;
        for (const auto& level : levels)
        {
            if (!level.isSupport)
                resistance.push_back(level);
        }
        return resistance;
    }

    std::vector<SupportResistanceLevel> PnFSupportResistance::GetSignificantLevels(const int minTouches) const
    {
        std::vector<SupportResistanceLevel> significant;
        for (const auto& level : levels)
        {
            if (level.touchCount >= minTouches)
                significant.push_back(level);
        }
        return significant;
    }

    bool PnFSupportResistance::IsNearSupport(const double price, const double tolerance) const
    {
        return std::ranges::any_of(levels,
        [price, tolerance](auto const& level) {
            return level.isSupport
                   && std::abs(price - level.price) / level.price < tolerance;
        });
    }

    bool PnFSupportResistance::IsNearResistance(const double price, const double tolerance) const
    {
        return std::ranges::any_of(levels,
        [price, tolerance](auto const& level) {
            return !level.isSupport
                   && std::abs(price - level.price) / level.price < tolerance;
        });
    }

    std::string PnFSupportResistance::ToString() const
    {
        std::string result = std::format("Support/Resistance: {} levels identified\n", levels.size());

        result += "Support Levels:\n";
        for (const auto& level : levels)
        {
            if (level.isSupport)
                result += std::format("  Price: {:.5f}, Touches: {}\n", level.price, level.touchCount);
        }

        result += "Resistance Levels:\n";
        for (const auto& level : levels)
        {
            if (!level.isSupport)
                result += std::format("  Price: {:.5f}, Touches: {}\n", level.price, level.touchCount);
        }

        return result;
    }

    PnFPriceObjective::PnFPriceObjective() = default;

    void PnFPriceObjective::CalculateVerticalCount(const PointAndFigureChart* chart, const int columnIndex)
    {
        if (columnIndex < 2)
            return;

        const PnFColumn* currentCol = chart->GetColumn(columnIndex);
        const int boxCount = currentCol->GetBoxCount();

        const double boxSize = (currentCol->GetColumnType() == COLUMN_X) ?
            std::abs(currentCol->GetBoxAt(currentCol->GetBoxCount() - 1)->GetPrice() - currentCol->GetBoxAt(currentCol->GetBoxCount() - 2)->GetPrice())
            : std::abs(currentCol->GetBoxAt(currentCol->GetBoxCount() - 2)->GetPrice() - currentCol->GetBoxAt(currentCol->GetBoxCount() - 1)->GetPrice());

        if (currentCol->GetColumnType() == COLUMN_X)
        {
            const double targetPrice = currentCol->GetHighestPrice() + (boxCount * boxSize);
            objectives.emplace_back(targetPrice, columnIndex, boxCount, true);
        }
        else if (currentCol->GetColumnType() == COLUMN_O)
        {
            const double targetPrice = currentCol->GetLowestPrice() - (boxCount * boxSize);
            objectives.emplace_back(targetPrice, columnIndex, boxCount, false);
        }
    }

    PnFPriceObjective::Objective PnFPriceObjective::GetLatestObjective() const
    {
        if (objectives.empty())
            return Objective{0.0, -1, 0, true};
        return objectives.back();
    }

    std::string PnFPriceObjective::ToString() const
    {
        std::string result = std::format("Price Objectives: {} calculated\n", objectives.size());

        for (const auto& obj : objectives)
        {
            result += std::format("  {} Target: {:.5f} (Extension: {} boxes)\n",
                obj.isBullish ? "Bullish" : "Bearish", obj.targetPrice, obj.extensionBoxes);
        }

        return result;
    }

    PnFIndicators::PnFIndicators()
    {
        sma5 = std::make_unique<PnFMovingAverage>(5);
        sma10 = std::make_unique<PnFMovingAverage>(10);
        bollingerBands = std::make_unique<PnFBollingerBands>(5, 2.0);
        signalDetector = std::make_unique<PnFSignalDetector>();
        patternRecognizer = std::make_unique<PnFPatternRecognizer>();
        supportResistance = std::make_unique<PnFSupportResistance>();
        priceObjective = std::make_unique<PnFPriceObjective>();
    }

    PnFIndicators::~PnFIndicators() = default;

    void PnFIndicators::Calculate(const PointAndFigureChart* chart) const
    {
        if (chart == nullptr || chart->GetColumnCount() == 0)
            return;

        sma5->Calculate(chart);
        sma10->Calculate(chart);
        bollingerBands->Calculate(chart);
        signalDetector->DetectSignals(chart);
        patternRecognizer->DetectPatterns(chart);
        supportResistance->IdentifyLevels(chart);

        const int columnCount = chart->GetColumnCount();
        for (int i = 0; i < columnCount; i++)
        {
            priceObjective->CalculateVerticalCount(chart, i);
        }
    }

    std::string PnFIndicators::GetSummary() const
    {
        std::string summary = "=== P&F INDICATORS SUMMARY ===\n\n";

        summary += "CURRENT SIGNAL: ";
        summary += signalDetector->HasBuySignal() ? "BUY\n" :
                   signalDetector->HasSellSignal() ? "SELL\n" : "NONE\n";

        summary += "\nLATEST PATTERN: ";
        if (const Pattern latest = patternRecognizer->GetLatestPattern(); latest.type != PATTERN_NONE)
        {
            summary += PnFPatternRecognizer::PatternTypeToString(latest.type) + "\n";
        }
        else
        {
            summary += "None detected\n";
        }

        const auto bullishPatterns = patternRecognizer->GetBullishPatterns();
        const auto bearishPatterns = patternRecognizer->GetBearishPatterns();
        summary += std::format("\nBULLISH PATTERNS: {}\n", bullishPatterns.size());
        summary += std::format("BEARISH PATTERNS: {}\n", bearishPatterns.size());

        const auto significantLevels = supportResistance->GetSignificantLevels(3);
        summary += std::format("\nSIGNIFICANT S/R LEVELS: {}\n", significantLevels.size());

        if (!priceObjective->GetObjectives().empty())
        {
            if (const auto latestObj = priceObjective->GetLatestObjective(); latestObj.baseColumnIndex != -1)
            {
                summary += std::format("\nLATEST PRICE TARGET: {:.5f} ({})\n",
                    latestObj.targetPrice, latestObj.isBullish ? "Bullish" : "Bearish");
            }
        }

        return summary;
    }

    std::string PnFIndicators::ToString() const
    {
        std::string result = "=== POINT & FIGURE INDICATORS ===\n\n";

        result += "MOVING AVERAGES:\n";
        result += sma5->ToString() + "\n";
        result += sma10->ToString() + "\n\n";

        result += "BOLLINGER BANDS:\n";
        result += bollingerBands->ToString() + "\n\n";

        result += "SIGNALS:\n";
        result += signalDetector->ToString() + "\n\n";

        result += "PATTERNS:\n";
        result += patternRecognizer->ToString() + "\n";

        result += "SUPPORT & RESISTANCE:\n";
        result += supportResistance->ToString() + "\n";

        result += "PRICE OBJECTIVES:\n";
        result += priceObjective->ToString() + "\n";

        return result;
    }
}
