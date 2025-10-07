//
// Created by gregorian on 06/10/2025.
//

#ifndef PNF_CHART_INDICATORS_H
#define PNF_CHART_INDICATORS_H

#include <vector>
#include "pnf_chart.h"
#include <chrono>
#include <string>
#include <memory>

namespace ChartSystem
{
    enum SignalType
    {
        SIGNAL_NONE,
        SIGNAL_BUY,
        SIGNAL_SELL
    };

    enum PatternType
    {
        PATTERN_NONE,
        PATTERN_DOUBLE_TOP_BREAKOUT,
        PATTERN_DOUBLE_BOTTOM_BREAKDOWN,
        PATTERN_TRIPLE_TOP_BREAKOUT,
        PATTERN_TRIPLE_BOTTOM_BREAKDOWN,
        PATTERN_QUADRUPLE_TOP_BREAKOUT,
        PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN,
        PATTERN_ASCENDING_TRIPLE_TOP,
        PATTERN_DESCENDING_TRIPLE_BOTTOM,
        PATTERN_BULLISH_CATAPULT,
        PATTERN_BEARISH_CATAPULT,
        PATTERN_BULLISH_SIGNAL_REVERSED,
        PATTERN_BEARISH_SIGNAL_REVERSED,
        PATTERN_BULLISH_TRIANGLE,
        PATTERN_BEARISH_TRIANGLE,
        PATTERN_LONG_TAIL_DOWN,
        PATTERN_HIGH_POLE,
        PATTERN_LOW_POLE,
        PATTERN_BULL_TRAP,
        PATTERN_BEAR_TRAP,
        PATTERN_SPREAD_TRIPLE_TOP,
        PATTERN_SPREAD_TRIPLE_BOTTOM
    };

    struct Signal
    {
        SignalType type;
        int columnIndex;
        double price;
        std::chrono::system_clock::time_point timestamp;

        Signal(const SignalType type, const int columnIndex, const double price, const std::chrono::system_clock::time_point timestamp)
            : type(type), columnIndex(columnIndex), price(price), timestamp(timestamp) {}
    };

    struct Pattern
    {
        PatternType type;
        int startColumnIndex;
        int endColumnIndex;
        double price;

        Pattern(const PatternType type, const int startCol, const int endCol, const double price)
            : type(type), startColumnIndex(startCol), endColumnIndex(endCol), price(price) {}
    };

    struct SupportResistanceLevel
    {
        double price;
        int touchCount;
        bool isSupport;
        int firstColumnIndex;
        int lastColumnIndex;

        SupportResistanceLevel(const double price, const bool isSupport, const int firstCol)
            : price(price), touchCount(1), isSupport(isSupport),
              firstColumnIndex(firstCol), lastColumnIndex(firstCol) {}
    };

    class PnFMovingAverage
    {
        int period;
        std::vector<double> values;

    public:
        explicit PnFMovingAverage(int period);
        void Calculate(const PointAndFigureChart* chart);
        [[nodiscard]] double GetValue(int columnIndex) const;
        [[nodiscard]] int GetPeriod() const { return period; }
        [[nodiscard]] bool HasValue(int columnIndex) const;
        [[nodiscard]] std::string ToString() const;
        [[nodiscard]] std::vector<double> GetValues() const { return this->values; }

    private:
        static double CalculateColumnAverage(const PnFColumn* column);
    };

    class PnFBollingerBands
    {
        int period;
        double standardDeviations;
        std::vector<double> middleBand;
        std::vector<double> upperBand;
        std::vector<double> lowerBand;

    public:
        PnFBollingerBands(int period, double stdDev);
        void Calculate(const PointAndFigureChart* chart);
        [[nodiscard]] double GetMiddleBand(int columnIndex) const;
        [[nodiscard]] double GetUpperBand(int columnIndex) const;
        [[nodiscard]] double GetLowerBand(int columnIndex) const;
        [[nodiscard]] std::vector<double> GetMiddleBandValues() const { return this->middleBand; }
        [[nodiscard]] std::vector<double> GetUpperBandValues() const { return this->upperBand; }
        [[nodiscard]] std::vector<double> GetLowerBandValues() const { return this->lowerBand; }
        [[nodiscard]] bool HasValue(int columnIndex) const;
        [[nodiscard]] bool IsAboveUpperBand(int columnIndex, double price) const;
        [[nodiscard]] bool IsBelowLowerBand(int columnIndex, double price) const;
        [[nodiscard]] std::string ToString() const;

    private:
        static double CalculateStandardDeviation(const std::vector<double>& values, double mean);
    };

    class PnFSignalDetector
    {
        std::vector<Signal> signals;
        SignalType currentSignal;

    public:
        PnFSignalDetector();
        void DetectSignals(const PointAndFigureChart* chart);
        [[nodiscard]] SignalType GetCurrentSignal() const { return currentSignal; }
        [[nodiscard]] const std::vector<Signal>& GetAllSignals() const { return signals; }
        [[nodiscard]] Signal GetLastSignal() const;
        [[nodiscard]] bool HasBuySignal() const { return currentSignal == SIGNAL_BUY; }
        [[nodiscard]] bool HasSellSignal() const { return currentSignal == SIGNAL_SELL; }
        [[nodiscard]] std::string ToString() const;

    private:
        static bool IsBuySignal(const PointAndFigureChart* chart, int columnIndex) ;
        static bool IsSellSignal(const PointAndFigureChart* chart, int columnIndex) ;
    };

    class PnFPatternRecognizer
    {
        std::vector<Pattern> patterns;

    public:
        PnFPatternRecognizer();
        void DetectPatterns(const PointAndFigureChart* chart);
        [[nodiscard]] const std::vector<Pattern>& GetAllPatterns() const { return patterns; }
        [[nodiscard]] std::vector<Pattern> GetBullishPatterns() const;
        [[nodiscard]] std::vector<Pattern> GetBearishPatterns() const;
        [[nodiscard]] Pattern GetLatestPattern() const;
        [[nodiscard]] bool HasPattern(PatternType type) const;
        [[nodiscard]] std::string ToString() const;
        static std::string PatternTypeToString(PatternType type);

    private:
        bool DetectDoubleTopBreakout(const PointAndFigureChart* chart, int columnIndex);
        bool DetectDoubleBottomBreakdown(const PointAndFigureChart* chart, int columnIndex);
        bool DetectTripleTopBreakout(const PointAndFigureChart* chart, int columnIndex);
        bool DetectTripleBottomBreakdown(const PointAndFigureChart* chart, int columnIndex);
        bool DetectQuadrupleTopBreakout(const PointAndFigureChart* chart, int columnIndex);
        bool DetectQuadrupleBottomBreakdown(const PointAndFigureChart* chart, int columnIndex);
        bool DetectAscendingTripleTop(const PointAndFigureChart* chart, int columnIndex);
        bool DetectDescendingTripleBottom(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBullishCatapult(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBearishCatapult(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBullishSignalReversed(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBearishSignalReversed(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBullishTriangle(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBearishTriangle(const PointAndFigureChart* chart, int columnIndex);
        bool DetectLongTailDown(const PointAndFigureChart* chart, int columnIndex);
        bool DetectHighPole(const PointAndFigureChart* chart, int columnIndex);
        bool DetectLowPole(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBullTrap(const PointAndFigureChart* chart, int columnIndex);
        bool DetectBearTrap(const PointAndFigureChart* chart, int columnIndex);
        bool DetectSpreadTripleTop(const PointAndFigureChart* chart, int columnIndex);
        bool DetectSpreadTripleBottom(const PointAndFigureChart* chart, int columnIndex);

        static bool IsBullishPattern(PatternType type);
        static bool IsBearishPattern(PatternType type);
    };

    class PnFSupportResistance
    {
        std::vector<SupportResistanceLevel> levels;
        double priceThreshold;

    public:
        explicit PnFSupportResistance(double priceThreshold = 0.01);
        void IdentifyLevels(const PointAndFigureChart* chart);
        [[nodiscard]] std::vector<SupportResistanceLevel> GetSupportLevels() const;
        [[nodiscard]] std::vector<SupportResistanceLevel> GetResistanceLevels() const;
        [[nodiscard]] std::vector<SupportResistanceLevel> GetSignificantLevels(int minTouches = 3) const;
        [[nodiscard]] bool IsNearSupport(double price, double tolerance = 0.02) const;
        [[nodiscard]] bool IsNearResistance(double price, double tolerance = 0.02) const;
        [[nodiscard]] std::string ToString() const;

    private:
        void MergeSimilarLevels();
    };

    class PnFPriceObjective
    {
    public:
        struct Objective
        {
            double targetPrice;
            int baseColumnIndex;
            int extensionBoxes;
            bool isBullish;

            Objective(const double target, const int baseCol, const int extension, const bool bullish)
                : targetPrice(target), baseColumnIndex(baseCol),
                  extensionBoxes(extension), isBullish(bullish) {}
        };

    private:
        std::vector<Objective> objectives;

    public:
        PnFPriceObjective();
        void CalculateVerticalCount(const PointAndFigureChart* chart, int columnIndex);
        [[nodiscard]] const std::vector<Objective>& GetObjectives() const { return objectives; }
        [[nodiscard]] Objective GetLatestObjective() const;
        [[nodiscard]] std::string ToString() const;
    };

    class PnFIndicators
    {
        std::unique_ptr<PnFMovingAverage> sma5;
        std::unique_ptr<PnFMovingAverage> sma10;
        std::unique_ptr<PnFBollingerBands> bollingerBands;
        std::unique_ptr<PnFSignalDetector> signalDetector;
        std::unique_ptr<PnFPatternRecognizer> patternRecognizer;
        std::unique_ptr<PnFSupportResistance> supportResistance;
        std::unique_ptr<PnFPriceObjective> priceObjective;

    public:
        PnFIndicators();
        ~PnFIndicators();

        void Calculate(const PointAndFigureChart* chart) const;

        [[nodiscard]] PnFMovingAverage* GetSMA5() const { return sma5.get(); }
        [[nodiscard]] PnFMovingAverage* GetSMA10() const { return sma10.get(); }
        [[nodiscard]] PnFBollingerBands* GetBollingerBands() const { return bollingerBands.get(); }
        [[nodiscard]] PnFSignalDetector* GetSignalDetector() const { return signalDetector.get(); }
        [[nodiscard]] PnFPatternRecognizer* GetPatternRecognizer() const { return patternRecognizer.get(); }
        [[nodiscard]] PnFSupportResistance* GetSupportResistance() const { return supportResistance.get(); }
        [[nodiscard]] PnFPriceObjective* GetPriceObjective() const { return priceObjective.get(); }

        [[nodiscard]] std::string GetSummary() const;
        [[nodiscard]] std::string ToString() const;
    };
}

#endif
