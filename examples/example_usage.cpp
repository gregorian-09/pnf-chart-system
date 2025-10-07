//
// Created by gregorian on 07/10/2025.
//

/**
 * ============================================================================
 * Point and Figure Chart System - Complete Example Usage
 * ============================================================================
 *
 * This example demonstrates how to use the ChartSystem library to create,
 * populate, and analyze Point and Figure charts. It covers:
 *
 * 1. Basic chart creation with different construction types
 * 2. Adding price data (both OHLC and single price)
 * 3. Analyzing chart patterns and signals
 * 4. Working with trend lines
 * 5. Using technical indicators
 *
 * Requirements:
 * - ChartSystem library (pnf_chart.h, pnf_column.h, pnf_indicators.h)
 * - GLFW3 (for visualization)
 * - C++20 or later
 *
 * Compile with:
 * g++ -std=c++20 example_usage.cpp -lglfw -o pnf_example
 * ============================================================================
 */

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include "pnf_chart.h"
#include "pnf_chart_indicators.h"

using namespace ChartSystem;
using namespace std::chrono;


/**
 * ============================================================================
 * EXAMPLE 1: Basic Chart Creation and Price Data Addition
 * ============================================================================
 *
 * Demonstrates the fundamental operations of creating a Point and Figure
 * chart and adding price data using different construction methods.
 */
void Example1_BasicChartCreation()
{
    std::cout << "\n=== EXAMPLE 1: Basic Chart Creation ===\n\n";

    /*
     * Create a chart using CLOSING_PRICE construction type.
     * This means the chart will update based on closing prices only.
     *
     * Parameters:
     * - ConstructionType: CLOSING_PRICE or HIGH_LOW
     * - BoxSizeType: DEFAULT, FIXED, or ATR
     * - BoxSize: The size of each box (0.0 for auto-calculation)
     * - ReversalCount: Number of boxes needed for a reversal (typically 1 or 3)
     */
    PointAndFigureChart chart(
        CLOSING_PRICE,    // Construction type
        DEFAULT,          // Box size type (auto-calculated)
        0.0,              // Box size (0.0 = auto)
        3                 // Reversal count (3-box reversal)
    );

    /*
     * Generate sample price data for demonstration.
     * In a real application, you would fetch this from a data provider.
     */
    const std::vector<double> prices = {
        100.0, 102.0, 105.0, 103.0, 106.0,  // Uptrend
        104.0, 102.0, 100.0, 98.0,  96.0,   // Downtrend
        98.0,  100.0, 102.0, 105.0, 107.0   // Uptrend recovery
    };

    /*
     * Add price data to the chart.
     * Each data point needs a timestamp for proper chronological ordering.
     */
    const auto startTime = system_clock::now();

    for (size_t i = 0; i < prices.size(); ++i)
    {
        // Calculate timestamp (1 hour intervals for this example)
        // Add the price data point
        if (const auto timestamp = startTime + hours(i); chart.AddData(prices[i], timestamp))
        {
            std::cout << "Price " << std::fixed << std::setprecision(2)
                      << prices[i] << " - Chart updated\n";
        }
    }

    /*
     * Display basic chart statistics
     */
    std::cout << "\nChart Statistics:\n";
    std::cout << "Total Columns: " << chart.GetColumnCount() << "\n";
    std::cout << "X Columns (Bullish): " << chart.GetXColumnCount() << "\n";
    std::cout << "O Columns (Bearish): " << chart.GetOColumnCount() << "\n";
    std::cout << "Current Box Size: " << chart.GetBoxSize() << "\n";
}


/**
 * ============================================================================
 * EXAMPLE 2: High-Low Construction Method
 * ============================================================================
 *
 * Demonstrates using OHLC (Open-High-Low-Close) data for more accurate
 * chart construction. This method captures intraday volatility better than
 * closing prices alone.
 */
void Example2_HighLowConstruction()
{
    std::cout << "\n=== EXAMPLE 2: High-Low Construction ===\n\n";

    /*
     * Create a chart using HIGH_LOW construction.
     * This method uses the high and low prices to determine box additions,
     * which can capture more price action than closing prices alone.
     */
    PointAndFigureChart chart(
        HIGH_LOW,         // Use high/low prices
        FIXED,            // Fixed box size
        1.0,              // Box size of 1.0
        3                 // 3-box reversal
    );

    /*
     * Sample OHLC data (Open, High, Low, Close)
     * Format: {High, Low, Close}
     */
    struct OHLCData {
        double high;
        double low;
        double close;
    };

    const std::vector<OHLCData> ohlcData = {
        {102.5, 99.5,  101.0},
        {103.0, 100.5, 102.5},
        {105.0, 102.0, 104.5},
        {104.0, 101.0, 101.5},
        {103.0, 99.0,  100.0},
        {101.0, 97.0,  98.0},
        {99.0,  95.0,  96.5},
        {98.0,  94.0,  95.0},
        {97.0,  95.0,  96.0},
        {100.0, 96.0,  99.0}
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < ohlcData.size(); ++i)
    {
        const auto timestamp = startTime + hours(i * 4); // 4-hour bars

        /*
         * Add OHLC data to the chart.
         * The chart will process both the high and low to capture
         * all significant price movements within the bar.
         */
        const bool wasUpdated = chart.AddData(
            ohlcData[i].high,
            ohlcData[i].low,
            ohlcData[i].close,
            timestamp
        );

        if (wasUpdated)
        {
            std::cout << "Bar " << i << ": H=" << ohlcData[i].high
                      << " L=" << ohlcData[i].low
                      << " C=" << ohlcData[i].close << " - Updated\n";
        }
    }

    /*
     * Get the last column to check current market direction
     */
    if (const PnFColumn* lastColumn = chart.GetLastColumn())
    {
        std::cout << "\nLast Column Info:\n";
        std::cout << "Type: " << (lastColumn->GetColumnType() == COLUMN_X ? "X (Bullish)" : "O (Bearish)") << "\n";
        std::cout << "Box Count: " << lastColumn->GetBoxCount() << "\n";
        std::cout << "Top Price: " << lastColumn->GetHighestPrice() << "\n";
        std::cout << "Bottom Price: " << lastColumn->GetLowestPrice() << "\n";
    }
}


/**
 * ============================================================================
 * EXAMPLE 3: Analyzing Trend Lines
 * ============================================================================
 *
 * Shows how to work with automatic trend line detection and analysis.
 * Trend lines are crucial for identifying support and resistance levels.
 */
void Example3_TrendLineAnalysis()
{
    std::cout << "\n=== EXAMPLE 3: Trend Line Analysis ===\n\n";

    /*
     * Create a chart with specific parameters for trend line detection
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 2.0, 3);

    // Generate a trending dataset
    const std::vector<double> trendingPrices = {
        100, 102, 104, 106, 108,  // Strong uptrend
        106, 108, 110, 112, 114,  // Continue uptrend
        112, 114, 116, 118, 120,  // More uptrend
        118, 116, 114, 116, 118   // Minor correction
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < trendingPrices.size(); ++i)
    {
        const auto timestamp = startTime + hours(i);
        chart.AddData(trendingPrices[i], timestamp);
    }

    /*
     * Access the trend line manager to analyze current market structure
     */
    if (const PnFTrendLineManager* trendLineManager = chart.GetTrendLineManager())
    {
        std::cout << "Trend Line Analysis:\n";

        // Check for bullish bias (price above support lines)
        if (trendLineManager->HasBullishBias())
        {
            std::cout << "✓ Chart shows BULLISH bias\n";

            // Get the active trend line
            if (const PnFTrendLine* activeTrendLine = trendLineManager->GetActiveTrendLine(); activeTrendLine && activeTrendLine->IsActive())
            {
                std::cout << "  Active trend line type: ";
                std::cout << (activeTrendLine->GetTrendLineType() == BULLISH_SUPPORT
                             ? "Bullish Support\n" : "Bearish Resistance\n");
                std::cout << "  Touch count: " << activeTrendLine->GetTouchCount() << "\n";
            }
        }

        // Check for bearish bias (price below resistance lines)
        if (trendLineManager->HasBearishBias())
        {
            std::cout << "✓ Chart shows BEARISH bias\n";
        }

        /*
         * Check if current price is above bullish support.
         * This is useful for confirming long entry signals.
         */
        const double currentPrice = trendingPrices.back();
        const int currentColumn = chart.GetColumnCount() - 1;

        if (trendLineManager->IsAboveBullishSupport(currentColumn, currentPrice))
        {
            std::cout << "\n✓ Current price (" << currentPrice
                      << ") is ABOVE bullish support\n";
            std::cout << "  → Favorable for long positions\n";
        }

        /*
         * Check if price is below bearish resistance.
         * This is useful for confirming short entry signals.
         */
        if (trendLineManager->IsBelowBearishResistance(currentColumn, currentPrice))
        {
            std::cout << "\n✓ Current price (" << currentPrice
                      << ") is BELOW bearish resistance\n";
            std::cout << "  → Favorable for short positions\n";
        }
    }

    /*
     * The chart provides convenience methods for trend bias
     */
    std::cout << "\nTrading Signal Recommendations:\n";
    std::cout << "Should take bullish signals: "
              << (chart.ShouldTakeBullishSignals() ? "YES" : "NO") << "\n";
    std::cout << "Should take bearish signals: "
              << (chart.ShouldTakeBearishSignals() ? "YES" : "NO") << "\n";
}


/**
 * ============================================================================
 * EXAMPLE 4: Working with Chart Columns
 * ============================================================================
 *
 * Demonstrates how to access and analyze individual columns to identify
 * patterns like double tops, double bottoms, and breakouts.
 */
void Example4_ColumnAnalysis()
{
    std::cout << "\n=== EXAMPLE 4: Column Analysis ===\n\n";

    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);

    // Create price data that will form identifiable patterns
    const std::vector<double> patternPrices = {
        100, 103, 106, 103, 100,  // Up-Down pattern
        103, 106, 109, 106, 103,  // Higher high pattern
        106, 109, 112, 109, 106   // Another higher high
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < patternPrices.size(); ++i)
    {
        chart.AddData(patternPrices[i], startTime + hours(i));
    }

    /*
     * Analyze all columns in the chart
     */
    const int columnCount = chart.GetColumnCount();
    std::cout << "Total columns formed: " << columnCount << "\n\n";

    /*
     * Iterate through columns to analyze the pattern
     */
    for (int i = 0; i < columnCount; ++i)
    {
        if (const PnFColumn* column = chart.GetColumn(i))
        {
            std::cout << "Column " << i << ":\n";
            std::cout << "  Type: " << (column->GetColumnType() == COLUMN_X ? "X" : "O") << "\n";
            std::cout << "  Boxes: " << column->GetBoxCount() << "\n";
            std::cout << "  Range: " << column->GetLowestPrice()
                      << " to " << column->GetHighestPrice() << "\n";

            // Check for month markers (useful for longer-term charts)
            if (!column->GetBoxAt(i)->GetMarker().empty())
            {
                std::cout << "  Marker: " << column->GetBoxAt(i)->GetMarker() << "\n";
            }

            std::cout << "\n";
        }
    }

    /*
     * Get specific column types for pattern recognition
     */
    const std::vector<int> xColumns = chart.GetXColumnIndices();
    const std::vector<int> oColumns = chart.GetOColumnIndices();

    std::cout << "X Column positions: ";
    for (const int idx : xColumns) std::cout << idx << " ";
    std::cout << "\n";

    std::cout << "O Column positions: ";
    for (const int idx : oColumns) std::cout << idx << " ";
    std::cout << "\n\n";

    /*
     * Simple pattern detection: Look for double top
     * (Two X columns with similar highs separated by an O column)
     */
    if (xColumns.size() >= 2)
    {
        for (size_t i = 0; i < xColumns.size() - 1; ++i)
        {
            const PnFColumn* col1 = chart.GetColumn(xColumns[i]);

            if (const PnFColumn* col2 = chart.GetColumn(xColumns[i + 1]); col1 && col2)
            {
                const double high1 = col1->GetHighestPrice();

                // Check if highs are within one box size
                if (const double high2 = col2->GetHighestPrice(); std::abs(high1 - high2) <= chart.GetBoxSize())
                {
                    std::cout << "✓ Potential DOUBLE TOP pattern detected at columns "
                              << xColumns[i] << " and " << xColumns[i + 1]
                              << " (price ~" << high1 << ")\n";
                }
            }
        }
    }
}


/**
 * ============================================================================
 * EXAMPLE 5: Technical Indicators Integration
 * ============================================================================
 *
 * Shows how to use the comprehensive PnFIndicators system which includes:
 * - Simple Moving Averages (SMA5, SMA10)
 * - Bollinger Bands
 * - Signal Detection
 * - Pattern Recognition
 * - Support/Resistance Levels
 * - Price Objectives
 */
void Example5_TechnicalIndicators()
{
    std::cout << "\n=== EXAMPLE 5: Technical Indicators ===\n\n";

    /*
     * Create chart with sufficient data for meaningful indicator calculations
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 0.5, 3);

    // Generate realistic price data with trends
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(0.0, 0.5);

    const auto startTime = system_clock::now();
    double currentPrice = 100.0;

    std::cout << "Generating price data...\n\n";

    for (int i = 0; i < 50; ++i)
    {
        // Simulate price movement with random walk
        currentPrice += dis(gen);
        currentPrice = std::max(90.0, std::min(110.0, currentPrice));

        chart.AddData(currentPrice, startTime + hours(i));
    }

    /*
     * Create the indicators system
     * This initializes all indicators: MA, BB, Signals, Patterns, SR, Objectives
     */
    const PnFIndicators indicators;

    /*
     * Calculate all indicators
     * This must be called before accessing any indicator values
     */
    indicators.Calculate(&chart);

    std::cout << "Chart columns created: " << chart.GetColumnCount() << "\n\n";

    // ========================================================================
    // MOVING AVERAGES
    // ========================================================================

    std::cout << "--- Moving Averages ---\n";

    /*
     * Access the 5-period Simple Moving Average
     */
    if (const PnFMovingAverage* sma5 = indicators.GetSMA5())
    {
        const int lastColumn = chart.GetColumnCount() - 1;

        std::cout << "SMA-5 (Period: " << sma5->GetPeriod() << "):\n";

        // Display last 5 values
        std::cout << "Latest values: ";
        for (int i = std::max(0, lastColumn - 4); i <= lastColumn; ++i)
        {
            if (sma5->HasValue(i))
            {
                std::cout << std::fixed << std::setprecision(2)
                          << sma5->GetValue(i) << " ";
            }
        }
        std::cout << "\n";
    }

    /*
     * Access the 10-period Simple Moving Average
     */
    if (const PnFMovingAverage* sma10 = indicators.GetSMA10())
    {
        const int lastColumn = chart.GetColumnCount() - 1;

        std::cout << "SMA-10 (Period: " << sma10->GetPeriod() << "):\n";
        std::cout << "Latest values: ";
        for (int i = std::max(0, lastColumn - 4); i <= lastColumn; ++i)
        {
            if (sma10->HasValue(i))
            {
                std::cout << std::fixed << std::setprecision(2)
                          << sma10->GetValue(i) << " ";
            }
        }
        std::cout << "\n\n";
    }

    // ========================================================================
    // BOLLINGER BANDS
    // ========================================================================

    /*
     * Access Bollinger Bands (default: 20-period, 2 standard deviations)
     */
    if (const PnFBollingerBands* bb = indicators.GetBollingerBands())
    {
        if (const int lastColumn = chart.GetColumnCount() - 1; bb->HasValue(lastColumn))
        {
            std::cout << "Latest Bollinger Bands:\n";
            std::cout << "  Upper Band: " << std::fixed << std::setprecision(2)
                      << bb->GetUpperBand(lastColumn) << "\n";
            std::cout << "  Middle Band: " << bb->GetMiddleBand(lastColumn) << "\n";
            std::cout << "  Lower Band: " << bb->GetLowerBand(lastColumn) << "\n";

            /*
             * Check if current price is outside the bands
             * This can indicate overbought/oversold conditions
             */
            if (const PnFColumn* lastCol = chart.GetLastColumn())
            {
                const double lastPrice = lastCol->GetColumnType() == COLUMN_X
                                   ? lastCol->GetHighestPrice()
                                   : lastCol->GetLowestPrice();

                std::cout << "  Current Price: " << lastPrice << "\n";

                if (bb->IsAboveUpperBand(lastColumn, lastPrice))
                {
                    std::cout << "\n⚠ Price is ABOVE upper band (Potential Overbought)\n";
                }
                else if (bb->IsBelowLowerBand(lastColumn, lastPrice))
                {
                    std::cout << "\n⚠ Price is BELOW lower band (Potential Oversold)\n";
                }
                else
                {
                    std::cout << "\n✓ Price is within bands (Normal range)\n";
                }
            }
        }
    }

    std::cout << "\n";
}


/**
 * ============================================================================
 * EXAMPLE 6: Signal Detection
 * ============================================================================
 *
 * Demonstrates the automatic signal detection system that identifies
 * buy and sell signals based on Point and Figure methodology.
 */
void Example6_SignalDetection()
{
    std::cout << "\n=== EXAMPLE 6: Signal Detection ===\n\n";

    /*
     * Create chart with price data that will generate signals
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);

    // Create data with clear signals
    const std::vector<double> prices = {
        100, 102, 104, 106, 104, 102,  // Up then down (potential sell)
        104, 106, 108, 110, 112,       // Strong uptrend (buy signal)
        110, 108, 106, 104, 102,       // Downtrend (sell signal)
        104, 106, 108, 110             // Recovery (buy signal)
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < prices.size(); ++i)
    {
        chart.AddData(prices[i], startTime + hours(i));
    }

    /*
     * Create indicators and detect signals
     */
    const PnFIndicators indicators;
    indicators.Calculate(&chart);

    /*
     * Access the signal detector
     */
    if (const PnFSignalDetector* signalDetector = indicators.GetSignalDetector())
    {
        std::cout << "Signal Analysis:\n\n";

        /*
         * Check current signal state
         */
        const SignalType currentSignal = signalDetector->GetCurrentSignal();

        std::cout << "Current Signal: ";
        switch (currentSignal)
        {
            case SIGNAL_BUY:
                std::cout << "BUY\n";
                break;
            case SIGNAL_SELL:
                std::cout << "SELL\n";
                break;
            case SIGNAL_NONE:
                std::cout << "NONE\n";
                break;
            default: ;
        }

        /*
         * Check for specific signal types
         */
        if (signalDetector->HasBuySignal())
        {
            std::cout << "✓ Active BUY signal detected\n";
        }

        if (signalDetector->HasSellSignal())
        {
            std::cout << "✓ Active SELL signal detected\n";
        }

        /*
         * Get all detected signals
         */
        if (const std::vector<Signal>& allSignals = signalDetector->GetAllSignals(); !allSignals.empty())
        {
            std::cout << "\nAll Signals (" << allSignals.size() << " total):\n";

            // Display last 5 signals
            const size_t startIdx = allSignals.size() > 5 ? allSignals.size() - 5 : 0;

            for (size_t i = startIdx; i < allSignals.size(); ++i)
            {
                const Signal& sig = allSignals[i];

                std::cout << "  Column " << sig.columnIndex << ": ";
                std::cout << (sig.type == SIGNAL_BUY ? "BUY " : "SELL");
                std::cout << " at " << std::fixed << std::setprecision(2)
                          << sig.price << "\n";
            }

            /*
             * Get the most recent signal
             */
            const Signal lastSignal = signalDetector->GetLastSignal();
            std::cout << "\nLast Signal: ";
            std::cout << (lastSignal.type == SIGNAL_BUY ? "BUY" : "SELL");
            std::cout << " at column " << lastSignal.columnIndex;
            std::cout << ", price " << lastSignal.price << "\n";
        }
    }
}


/**
 * ============================================================================
 * EXAMPLE 7: Pattern Recognition
 * ============================================================================
 *
 * Demonstrates the comprehensive pattern recognition system that identifies
 * 21 different Point and Figure chart patterns.
 */
void Example7_PatternRecognition()
{
    std::cout << "\n=== EXAMPLE 7: Pattern Recognition ===\n\n";

    /*
     * Create chart with data that forms recognizable patterns
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);

    // Create price data with patterns
    const std::vector<double> patternPrices = {
        100, 103, 106, 103, 100,       // Formation
        103, 106, 109, 106, 103,       // Double top setup
        106, 109, 112,                 // Breakout (Double Top Breakout pattern)
        110, 107, 104, 107, 110,       // Retracement
        113, 116, 119                  // Continuation
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < patternPrices.size(); ++i)
    {
        chart.AddData(patternPrices[i], startTime + hours(i));
    }

    /*
     * Calculate indicators and detect patterns
     */
    PnFIndicators indicators;
    indicators.Calculate(&chart);

    /*
     * Access the pattern recognizer
     */
    if (const PnFPatternRecognizer* patternRecognizer = indicators.GetPatternRecognizer())
    {
        std::cout << "Pattern Recognition Analysis:\n\n";

        /*
         * Get all detected patterns
         */
        if (const std::vector<Pattern>& allPatterns = patternRecognizer->GetAllPatterns(); !allPatterns.empty())
        {
            std::cout << "Detected Patterns (" << allPatterns.size() << " total):\n\n";

            for (const auto& pattern : allPatterns)
            {
                std::cout << "  Pattern: "
                          << PnFPatternRecognizer::PatternTypeToString(pattern.type) << "\n";
                std::cout << "  Columns: " << pattern.startColumnIndex
                          << " to " << pattern.endColumnIndex << "\n";
                std::cout << "  Price Level: " << std::fixed << std::setprecision(2)
                          << pattern.price << "\n\n";
            }

            /*
             * Get the latest pattern
             */
            const Pattern latestPattern = patternRecognizer->GetLatestPattern();
            std::cout << "Latest Pattern: "
                      << PnFPatternRecognizer::PatternTypeToString(latestPattern.type) << "\n";
        }
        else
        {
            std::cout << "No patterns detected yet.\n";
        }

        /*
         * Get bullish patterns only
         */
        const std::vector<Pattern> bullishPatterns = patternRecognizer->GetBullishPatterns();
        std::cout << "\nBullish Patterns: " << bullishPatterns.size() << "\n";

        /*
         * Get bearish patterns only
         */
        const std::vector<Pattern> bearishPatterns = patternRecognizer->GetBearishPatterns();
        std::cout << "Bearish Patterns: " << bearishPatterns.size() << "\n";

        /*
         * Check for specific pattern types
         */
        std::cout << "\nSpecific Pattern Checks:\n";
        std::cout << "  Double Top Breakout: "
                  << (patternRecognizer->HasPattern(PATTERN_DOUBLE_TOP_BREAKOUT) ? "YES" : "NO") << "\n";
        std::cout << "  Triple Top Breakout: "
                  << (patternRecognizer->HasPattern(PATTERN_TRIPLE_TOP_BREAKOUT) ? "YES" : "NO") << "\n";
        std::cout << "  Bullish Catapult: "
                  << (patternRecognizer->HasPattern(PATTERN_BULLISH_CATAPULT) ? "YES" : "NO") << "\n";
        std::cout << "  High Pole: "
                  << (patternRecognizer->HasPattern(PATTERN_HIGH_POLE) ? "YES" : "NO") << "\n";
    }
}


/**
 * ============================================================================
 * EXAMPLE 8: Support and Resistance Levels
 * ============================================================================
 *
 * Shows how to identify and work with support and resistance levels
 * automatically detected by the system.
 */
void Example8_SupportResistance()
{
    std::cout << "\n=== EXAMPLE 8: Support and Resistance Levels ===\n\n";

    /*
     * Create chart with price bouncing between levels
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 0.5, 3);

    // Create data that forms clear support/resistance
    const std::vector<double> prices = {
        100, 102, 105, 103, 101, 99,    // Down to support
        101, 103, 105, 107, 105, 103,   // Up to resistance
        101, 99, 97, 99, 101,           // Down to support again
        103, 105, 107, 109, 107, 105,   // Up to resistance again
        103, 105, 107, 109, 111         // Break through resistance
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < prices.size(); ++i)
    {
        chart.AddData(prices[i], startTime + hours(i));
    }

    /*
     * Calculate indicators
     */
    const PnFIndicators indicators;
    indicators.Calculate(&chart);

    /*
     * Access support/resistance analyzer
     */
    if (const PnFSupportResistance* sr = indicators.GetSupportResistance())
    {
        std::cout << "Support and Resistance Analysis:\n\n";

        /*
         * Get all support levels
         */
        if (std::vector<SupportResistanceLevel> supportLevels = sr->GetSupportLevels(); !supportLevels.empty())
        {
            std::cout << "Support Levels (" << supportLevels.size() << " found):\n";

            for (const auto& level : supportLevels)
            {
                std::cout << "  Price: " << std::fixed << std::setprecision(2)
                          << level.price;
                std::cout << " | Touches: " << level.touchCount;
                std::cout << " | Columns: " << level.firstColumnIndex
                          << " to " << level.lastColumnIndex << "\n";
            }
            std::cout << "\n";
        }

        /*
         * Get all resistance levels
         */
        if (const std::vector<SupportResistanceLevel> resistanceLevels = sr->GetResistanceLevels(); !resistanceLevels.empty())
        {
            std::cout << "Resistance Levels (" << resistanceLevels.size() << " found):\n";

            for (const auto& level : resistanceLevels)
            {
                std::cout << "  Price: " << std::fixed << std::setprecision(2)
                          << level.price;
                std::cout << " | Touches: " << level.touchCount;
                std::cout << " | Columns: " << level.firstColumnIndex
                          << " to " << level.lastColumnIndex << "\n";
            }
            std::cout << "\n";
        }

        /*
         * Get significant levels (3+ touches)
         */
        const std::vector<SupportResistanceLevel> significantLevels =
            sr->GetSignificantLevels(3);

        std::cout << "Significant Levels (3+ touches): "
                  << significantLevels.size() << "\n\n";

        /*
         * Check if current price is near key levels
         */
        if (const PnFColumn* lastColumn = chart.GetLastColumn())
        {
            const double currentPrice = lastColumn->GetColumnType() == COLUMN_X
                                  ? lastColumn->GetHighestPrice()
                                  : lastColumn->GetLowestPrice();

            std::cout << "Current Price: " << currentPrice << "\n";

            // Check proximity to support
            if (sr->IsNearSupport(currentPrice, 0.02))  // Within 2%
            {
                std::cout << "✓ Price is NEAR SUPPORT (potential bounce zone)\n";
            }

            // Check proximity to resistance
            if (sr->IsNearResistance(currentPrice, 0.02))  // Within 2%
            {
                std::cout << "✓ Price is NEAR RESISTANCE (potential rejection zone)\n";
            }
        }
    }
}


/**
 * ============================================================================
 * EXAMPLE 9: Price Objectives (Vertical Count)
 * ============================================================================
 *
 * Demonstrates how to calculate price objectives using the vertical count
 * method, which is a key feature of Point and Figure analysis.
 */
void Example9_PriceObjectives()
{
    std::cout << "\n=== EXAMPLE 9: Price Objectives ===\n\n";

    /*
     * Create chart with strong trend
     */
    PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);

    // Create trending data
    const std::vector<double> prices = {
        100, 103, 106, 109, 107, 104,  // Up column with reversal
        107, 110, 113, 116, 119, 122   // Strong up column (price objective)
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < prices.size(); ++i)
    {
        chart.AddData(prices[i], startTime + hours(i));
    }

    /*
     * Calculate indicators including price objectives
     */
    const PnFIndicators indicators;
    indicators.Calculate(&chart);

    /*
     * Access price objective calculator
     */
    if (const PnFPriceObjective* priceObj = indicators.GetPriceObjective())
    {
        std::cout << "Price Objective Analysis:\n\n";

        /*
         * Get all calculated price objectives
         */
        const std::vector<PnFPriceObjective::Objective>& objectives =
            priceObj->GetObjectives();

        if (!objectives.empty())
        {
            std::cout << "Calculated Price Objectives (" << objectives.size() << " found):\n\n";

            for (const auto& obj : objectives)
            {
                std::cout << "  " << (obj.isBullish ? "BULLISH" : "BEARISH")
                          << " Objective:\n";
                std::cout << "  Target Price: " << std::fixed << std::setprecision(2)
                          << obj.targetPrice << "\n";
                std::cout << "  Base Column: " << obj.baseColumnIndex << "\n";
                std::cout << "  Extension Boxes: " << obj.extensionBoxes << "\n\n";
            }

            /*
             * Get the latest price objective
             */
            const PnFPriceObjective::Objective latestObj = priceObj->GetLatestObjective();

            std::cout << "Latest Price Objective:\n";
            std::cout << "  Direction: " << (latestObj.isBullish ? "UP" : "DOWN") << "\n";
            std::cout << "  Target: " << latestObj.targetPrice << "\n";

            /*
             * Compare with current price
             */
            if (const PnFColumn* lastColumn = chart.GetLastColumn())
            {
                const double currentPrice = lastColumn->GetColumnType() == COLUMN_X
                                      ? lastColumn->GetHighestPrice()
                                      : lastColumn->GetLowestPrice();

                const double priceDistance = latestObj.targetPrice - currentPrice;
                const double percentToTarget = (priceDistance / currentPrice) * 100.0;

                std::cout << "  Current Price: " << currentPrice << "\n";
                std::cout << "  Distance to Target: " << std::abs(priceDistance)
                          << " (" << std::abs(percentToTarget) << "%)\n";
            }
        }
        else
        {
            std::cout << "No price objectives calculated yet.\n";
            std::cout << "(Requires sufficient column formation)\n";
        }
    }
}


/**
 * ============================================================================
 * EXAMPLE 10: Complete Indicator Summary
 * ============================================================================
 *
 * Shows how to get a comprehensive summary of all indicators at once,
 * useful for displaying an analysis dashboard.
 */
void Example10_CompleteSummary()
{
    std::cout << "\n=== EXAMPLE 10: Complete Indicator Summary ===\n\n";

    /*
     * Create a well-populated chart
     */
    PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);

    // Realistic market data with various patterns
    struct OHLC { double high, low, close; };

    const std::vector<OHLC> marketData = {
        {101, 99, 100}, {103, 100, 102}, {105, 102, 104},
        {106, 103, 105}, {108, 105, 107}, {107, 104, 105},
        {109, 106, 108}, {111, 108, 110}, {113, 110, 112},
        {112, 109, 110}, {114, 111, 113}, {116, 113, 115},
        {115, 112, 113}, {117, 114, 116}, {119, 116, 118},
        {118, 115, 116}, {120, 117, 119}, {122, 119, 121}
    };

    const auto startTime = system_clock::now();

    for (size_t i = 0; i < marketData.size(); ++i)
    {
        chart.AddData(
            marketData[i].high,
            marketData[i].low,
            marketData[i].close,
            startTime + hours(i * 4)
        );
    }

    /*
     * Calculate all indicators
     */
    const PnFIndicators indicators;
    indicators.Calculate(&chart);

    /*
     * Get complete summary
     * This returns a formatted string with all indicator information
     */
    std::cout << "COMPLETE ANALYSIS DASHBOARD\n";
    std::cout << "============================\n\n";

    const std::string summary = indicators.GetSummary();
    std::cout << summary << "\n";

    /*
     * You can also get detailed string representation
     */
    std::cout << "\nDETAILED INDICATOR DATA\n";
    std::cout << "=======================\n\n";

    const std::string detailed = indicators.ToString();
    std::cout << detailed << "\n";

    /*
     * Demonstrate combining multiple indicators for decision-making
     */
    std::cout << "\nTRADING DECISION ANALYSIS\n";
    std::cout << "=========================\n\n";

    const PnFSignalDetector* signals = indicators.GetSignalDetector();
    const PnFPatternRecognizer* patterns = indicators.GetPatternRecognizer();

    // Check for bullish confluence
    int bullishScore = 0;
    int bearishScore = 0;

    if (signals && signals->HasBuySignal())
    {
        bullishScore++;
        std::cout << "Buy Signal Active (+1 bullish)\n";
    }

    if (signals && signals->HasSellSignal())
    {
        bearishScore++;
        std::cout << "Sell Signal Active (+1 bearish)\n";
    }

    if (patterns)
    {
        const int bullishPatterns = patterns->GetBullishPatterns().size();
        const int bearishPatterns = patterns->GetBearishPatterns().size();

        if (bullishPatterns > 0)
        {
            bullishScore += bullishPatterns;
            std::cout << bullishPatterns << " Bullish Pattern(s) (+"
                      << bullishPatterns << " bullish)\n";
        }

        if (bearishPatterns > 0)
        {
            bearishScore += bearishPatterns;
            std::cout << bearishPatterns << " Bearish Pattern(s) (+"
                      << bearishPatterns << " bearish)\n";
        }
    }

    if (chart.HasBullishBias())
    {
        bullishScore += 2;
        std::cout << "Bullish Trend Bias (+2 bullish)\n";
    }

    if (chart.HasBearishBias())
    {
        bearishScore += 2;
        std::cout << "Bearish Trend Bias (+2 bearish)\n";
    }

    std::cout << "\nFINAL SCORE:\n";
    std::cout << "  Bullish: " << bullishScore << "\n";
    std::cout << "  Bearish: " << bearishScore << "\n";

    if (bullishScore > bearishScore)
    {
        std::cout << "\nRECOMMENDATION: BULLISH OUTLOOK\n";
        std::cout << "   Consider long positions or holding existing longs\n";
    }
    else if (bearishScore > bullishScore)
    {
        std::cout << "\nRECOMMENDATION: BEARISH OUTLOOK\n";
        std::cout << "   Consider short positions or reducing long exposure\n";
    }
    else
    {
        std::cout << "\nRECOMMENDATION: NEUTRAL\n";
        std::cout << "   Wait for clearer signals before taking positions\n";
    }
}


void execute()
{
    std::cout << "========================================================\n";
    std::cout << "   Point and Figure Indicators - Example Usage         \n";
    std::cout << "========================================================\n";

    try
    {
        /*
         * Run all indicator examples
         */
        Example1_BasicChartCreation();
        Example2_HighLowConstruction();
        Example3_TrendLineAnalysis();
        Example4_ColumnAnalysis();
        Example5_TechnicalIndicators();
        Example6_SignalDetection();
        Example7_PatternRecognition();
        Example8_SupportResistance();
        Example9_PriceObjectives();
        Example10_CompleteSummary();

        std::cout << "\n\n========================================================\n";
        std::cout << "   All indicator examples completed successfully!      \n";
        std::cout << "========================================================\n\n";

    }
    catch (const std::exception& e)
    {
        std::cerr << "\nError: " << e.what() << "\n";
    }
}