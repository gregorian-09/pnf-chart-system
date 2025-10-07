//
// Created by gregorian on 03/10/2025.
//

#ifndef PNF_CHART_H
#define PNF_CHART_H

#include <algorithm>
#include "pnf_column.h"
#include <chrono>
#include <ctime>
#include <GLFW/glfw3.h>

namespace ChartSystem
{
    class PnFIndicators;

    enum TrendLineType { BULLISH_SUPPORT, BEARISH_RESISTANCE };

    struct TrendLinePoint
    {
        int columnIndex;
        double price;
        int boxIndex;
    };

    class PnFTrendLine
    {
        TrendLineType trendLineType;
        TrendLinePoint startPoint{};
        TrendLinePoint endPoint{};

        double boxSize;
        bool isActive;
        bool wasTouched;
        int touchCount;

        [[nodiscard]] double CalculatePriceAtColumn(int columnIndex) const;
        bool GetValue(double price, double trendlinePrice, bool& value1);

    public:
        PnFTrendLine(TrendLineType trendLineType, int startColumnIndex, double startPrice,
                    int startBoxIndex, double boxSize);
        ~PnFTrendLine();
        void UpdateEndPoint(int endColumnIndex, double endPrice, int endBoxIndex);
        [[nodiscard]] bool IsBroken(int columnIndex, double price) const;
        bool TestTrendLine(int columnIndex, double price);

        [[nodiscard]] double GetPriceAtColumn(int columnIndex) const;
        [[nodiscard]] bool IsActive() const { return isActive; }
        void SetActive(const bool isActive) { this->isActive = isActive; }
        [[nodiscard]] bool WasTouched() const { return wasTouched; }
        [[nodiscard]] int GetTouchCount() const { return touchCount; }

        [[nodiscard]] TrendLineType GetTrendLineType() const { return trendLineType; }
        [[nodiscard]] TrendLinePoint GetStartPoint() const { return startPoint; }
        [[nodiscard]] TrendLinePoint GetEndPoint() const { return endPoint; }

        [[nodiscard]] std::string ToString() const;
    };

    class PnFTrendLineManager
    {
        std::vector<std::unique_ptr<PnFTrendLine>> trendLines;
        PnFTrendLine* activeTrendLine;
        double boxSize;

        static int FindSignificantLow(const std::vector<std::unique_ptr<PnFColumn>> &columns, int fromColumn);
        static int FindSignificantHigh(const std::vector<std::unique_ptr<PnFColumn>> &columns, int fromColumn);
        static bool IsSignificantLow(const PnFColumn* column, const std::vector<std::unique_ptr<PnFColumn>> &allColumns, int columnIndex);
        static bool IsSignificantHigh(const PnFColumn* column, const std::vector<std::unique_ptr<PnFColumn>> &allColumns, int columnIndex);

    public:
        explicit PnFTrendLineManager(double boxSize);
        ~PnFTrendLineManager();

        void UpdateTrendLines(const std::vector<std::unique_ptr<PnFColumn>> &columns, int newColumnIndex);
        void ProcessNewColumn(const std::vector<std::unique_ptr<PnFColumn>> &columns, int columnIndex);
        void CheckTrendLineBreak(const std::vector<std::unique_ptr<PnFColumn>> &columns, int columnIndex) const;

        [[nodiscard]] PnFTrendLine* GetActiveTrendLine() const { return this->activeTrendLine; }
        std::vector<std::unique_ptr<PnFTrendLine>>* GetAllTrendLines() { return &trendLines; }

        [[nodiscard]] bool IsAboveBullishSupport(int columnIndex, double price) const;
        [[nodiscard]] bool IsBelowBearishResistance(int columnIndex, double price) const;
        [[nodiscard]] bool HasBullishBias() const;
        [[nodiscard]] bool HasBearishBias() const;

        void Clear();
        [[nodiscard]] std::string ToString() const;
    };

    class PointAndFigureChart
    {
        std::vector<std::unique_ptr<PnFColumn>> columns;
        std::unique_ptr<PnFTrendLineManager> trendLineManager;
        ConstructionType constructionType;
        BoxType boxType;
        BoxSizeType boxSizeType;
        double boxSize;
        int reversalCount;
        std::chrono::system_clock::time_point lastDateTime;
        std::chrono::system_clock::time_point lastProcessedTime;
        int lastMonth;
        MonthMarker monthMarker[12] = {
            {1, "1"}, {2, "2"}, {3, "3"}, {4, "4"},
            {5, "5"}, {6, "6"}, {7, "7"}, {8, "8"},
            {9, "9"}, {10, "A"}, {11, "B"}, {12, "C"}
        };

        std::string GetMonthMarker(int month);
        double CalculateBoxSize(double price);
        double RoundToBoxSize(double price, bool roundUp);
        bool IsReversal(double price, const PnFColumn* currentColumn, BoxType &newBoxType);
        bool ProcessHighLowData(double high, double low, std::chrono::system_clock::time_point time);
        bool ProcessClosingPriceData(double close, std::chrono::system_clock::time_point time);
        bool HasMonthChanged(const std::chrono::system_clock::time_point currentTime, std::string& monthMarker)
        {
            std::time_t currentTt = std::chrono::system_clock::to_time_t(currentTime);
            std::tm currentDt{};
#ifdef _WIN32
            localtime_s(&currentDt, &currentTt);
#else
            localtime_r(&currentTt, &currentDt);
#endif

            if (lastProcessedTime == std::chrono::system_clock::time_point{})
            {
                monthMarker = GetMonthMarker(currentDt.tm_mon + 1);
                return true;
            }

            std::time_t lastTt = std::chrono::system_clock::to_time_t(lastProcessedTime);
            std::tm lastDt{};
#ifdef _WIN32
            localtime_s(&lastDt, &lastTt);
#else
            localtime_r(&lastTt, &lastDt);
#endif

            if (currentDt.tm_year != lastDt.tm_year || currentDt.tm_mon != lastDt.tm_mon)
            {
                monthMarker = GetMonthMarker(currentDt.tm_mon + 1);
                return true;
            }

            monthMarker = "";
            return false;
        }

        std::string GetMonthMarkerForCurrentData(const std::chrono::system_clock::time_point dataTime)
        {
            if (std::string marker; HasMonthChanged(dataTime, marker))
            {
                return marker;
            }
            return "";
        }

    public:
        explicit PointAndFigureChart(ConstructionType constructionType = CLOSING_PRICE, BoxSizeType boxSizeType = DEFAULT,
            double boxSize = 0.0, int reversalCount = 3);
        ~PointAndFigureChart();

        bool AddData(double high, double low, double close, std::chrono::system_clock::time_point time);
        [[nodiscard]] std::vector<double> GetAllPrices() const;
        bool AddData(double price, std::chrono::system_clock::time_point time);
        [[nodiscard]] bool ExportToExcel(const std::string& filename) const;

        [[nodiscard]] bool IsMonthMarker(const std::string& marker) const
        {
            return std::ranges::any_of(monthMarker,
            [&marker](auto const& mm) {
                return mm.marker == marker;
            });
        }

        [[nodiscard]] int GetColumnCount() const;
        [[nodiscard]] PnFColumn* GetColumn(int columnIndex) const;
        [[nodiscard]] PnFColumn* GetLastColumn() const;

        [[nodiscard]] int GetXColumnCount() const;
        [[nodiscard]] int GetOColumnCount() const;
        [[nodiscard]] int GetMixedColumnCount() const;

        [[nodiscard]] std::vector<int> GetXColumnIndices() const;
        [[nodiscard]] std::vector<int> GetOColumnIndices() const;
        [[nodiscard]] std::vector<int> GetMixedColumnIndices() const;

        void SetConstructionType(const ConstructionType constructionType) { this->constructionType = constructionType; }
        void SetBoxSize(const BoxSizeType boxSizeType) { this->boxSize = boxSizeType; }
        void SetBoxSizeType(const BoxSizeType boxSizeType) { this->boxSize = boxSizeType; }
        void SetReversalCount(const int reversalCount) { this->reversalCount = reversalCount; }

        [[nodiscard]] ConstructionType GetConstructionType() const { return this->constructionType; }
        [[nodiscard]] BoxSizeType GetBoxSizeType() const { return this->boxSizeType; }
        [[nodiscard]] double GetBoxSize() const { return this->boxSize; }
        [[nodiscard]] int GetReversalCount() const { return this->reversalCount; }

        [[nodiscard]] PnFTrendLineManager* GetTrendLineManager() const { return trendLineManager.get(); }
        [[nodiscard]] bool HasBullishBias() const;
        [[nodiscard]] bool HasBearishBias() const;
        [[nodiscard]] bool ShouldTakeBullishSignals() const;
        [[nodiscard]] bool ShouldTakeBearishSignals() const;
        [[nodiscard]] bool IsAboveBullishSupport(double price) const;
        [[nodiscard]] bool IsBelowBearishResistance(double price) const;

        void Clear();
        [[nodiscard]] std::string ToString() const;
    };

    class PnFChartVisualizer
    {
        PointAndFigureChart* chart;
        PnFIndicators* indicators{};
        GLFWwindow* window{};
        bool initialized;
        bool showMovingAverages;
        bool showBollingerBands;
        bool showSupportResistance;

        void Initialize();
        void Cleanup();
        void RenderFrame() const;
        void DrawTrendlines() const;
        void DrawMovingAverages() const;
        void DrawBollingerBands() const;
        void DrawSupportResistance() const;

    public:
        explicit PnFChartVisualizer(PointAndFigureChart* chart, PnFIndicators* indicators = nullptr);
        ~PnFChartVisualizer();

        void Show();
        void SetShowMovingAverages(const bool show) { showMovingAverages = show; }
        void SetShowBollingerBands(const bool show) { showBollingerBands = show; }
        void SetShowSupportResistance(const bool show) { showSupportResistance = show; }
    };
}

#endif
