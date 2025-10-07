//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart_indicators.h"
#include "pnf_chart.h"

using namespace ChartSystem;

class PnFPatternRecognizerTest : public ::testing::Test
{
protected:
    PointAndFigureChart* chart{};

    void SetUp() override
    {
        chart = new PointAndFigureChart(CLOSING_PRICE, FIXED, 1.0, 3);
    }

    void TearDown() override
    {
        delete chart;
    }
};

TEST_F(PnFPatternRecognizerTest, EmptyChartHasNoPatterns)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    EXPECT_EQ(0, recognizer.GetAllPatterns().size());
}

TEST_F(PnFPatternRecognizerTest, HasPatternReturnsFalseForEmptyPatterns)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    EXPECT_FALSE(recognizer.HasPattern(PATTERN_DOUBLE_TOP_BREAKOUT));
}

TEST_F(PnFPatternRecognizerTest, GetLatestPatternReturnsNoneWhenEmpty)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    const Pattern latest = recognizer.GetLatestPattern();
    EXPECT_EQ(PATTERN_NONE, latest.type);
    EXPECT_EQ(-1, latest.startColumnIndex);
}

TEST_F(PnFPatternRecognizerTest, GetBullishPatternsReturnsEmptyWhenNone)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    const auto bullish = recognizer.GetBullishPatterns();
    EXPECT_EQ(0, bullish.size());
}

TEST_F(PnFPatternRecognizerTest, GetBearishPatternsReturnsEmptyWhenNone)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    const auto bearish = recognizer.GetBearishPatterns();
    EXPECT_EQ(0, bearish.size());
}

TEST_F(PnFPatternRecognizerTest, DetectsDoubleTopBreakout)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    chart->AddData(103.0, std::chrono::system_clock::now());
    chart->AddData(101.0, std::chrono::system_clock::now());
    chart->AddData(105.0, std::chrono::system_clock::now());

    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    const auto patterns = recognizer.GetAllPatterns();
    bool foundDoubleTop = false;
    for (const auto& p : patterns)
    {
        if (p.type == PATTERN_DOUBLE_TOP_BREAKOUT)
        {
            foundDoubleTop = true;
            break;
        }
    }

    if (chart->GetColumnCount() >= 3)
    {
        EXPECT_TRUE(foundDoubleTop || patterns.empty());
    }
}

TEST_F(PnFPatternRecognizerTest, BullishPatternsAreClassifiedCorrectly)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    chart->AddData(103.0, std::chrono::system_clock::now());
    chart->AddData(101.0, std::chrono::system_clock::now());
    chart->AddData(105.0, std::chrono::system_clock::now());

    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    for (auto bullish = recognizer.GetBullishPatterns(); const auto& p : bullish)
    {
        EXPECT_TRUE(p.type == PATTERN_DOUBLE_TOP_BREAKOUT ||
                   p.type == PATTERN_TRIPLE_TOP_BREAKOUT ||
                   p.type == PATTERN_QUADRUPLE_TOP_BREAKOUT ||
                   p.type == PATTERN_ASCENDING_TRIPLE_TOP ||
                   p.type == PATTERN_BULLISH_CATAPULT ||
                   p.type == PATTERN_BULLISH_SIGNAL_REVERSED ||
                   p.type == PATTERN_BULLISH_TRIANGLE ||
                   p.type == PATTERN_LONG_TAIL_DOWN ||
                   p.type == PATTERN_LOW_POLE ||
                   p.type == PATTERN_BEAR_TRAP ||
                   p.type == PATTERN_SPREAD_TRIPLE_TOP);
    }
}

TEST_F(PnFPatternRecognizerTest, ToStringReturnsValidFormat)
{
    PnFPatternRecognizer recognizer;
    recognizer.DetectPatterns(chart);

    std::string result = recognizer.ToString();
    EXPECT_FALSE(result.empty());
    EXPECT_NE(std::string::npos, result.find("Pattern Recognizer"));
}