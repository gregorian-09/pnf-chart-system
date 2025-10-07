//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart_indicators.h"
#include "pnf_chart.h"

using namespace ChartSystem;

class PnFBollingerBandsTest : public ::testing::Test
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

TEST_F(PnFBollingerBandsTest, EmptyChartReturnsZero)
{
    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    EXPECT_EQ(0.0, bb.GetMiddleBand(0));
    EXPECT_EQ(0.0, bb.GetUpperBand(0));
    EXPECT_EQ(0.0, bb.GetLowerBand(0));
}

TEST_F(PnFBollingerBandsTest, InsufficientDataHasNoValue)
{
    chart->AddData(100.0, std::chrono::system_clock::now());

    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    EXPECT_FALSE(bb.HasValue(0));
}

TEST_F(PnFBollingerBandsTest, UpperBandGreaterThanMiddleBand)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    for (int i = 1; i < 50; i++)
    {
        if (i % 2 == 0)
            chart->AddData(100.0 + i * 0.5, std::chrono::system_clock::now());
        else
            chart->AddData(100.0 - i * 0.5, std::chrono::system_clock::now());
    }

    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    int colCount = chart->GetColumnCount();
    if (colCount >= 20)
    {
        int idx = colCount - 1;
        if (bb.HasValue(idx))
        {
            EXPECT_GT(bb.GetUpperBand(idx), bb.GetMiddleBand(idx));
        }
    }
}

TEST_F(PnFBollingerBandsTest, LowerBandLessThanMiddleBand)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    for (int i = 1; i < 50; i++)
    {
        if (i % 2 == 0)
            chart->AddData(100.0 + i * 0.5, std::chrono::system_clock::now());
        else
            chart->AddData(100.0 - i * 0.5, std::chrono::system_clock::now());
    }

    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    if (const int colCount = chart->GetColumnCount(); colCount >= 20)
    {
        if (const int idx = colCount - 1; bb.HasValue(idx))
        {
            EXPECT_LT(bb.GetLowerBand(idx), bb.GetMiddleBand(idx));
        }
    }
}

TEST_F(PnFBollingerBandsTest, IsAboveUpperBandDetectsCorrectly)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    for (int i = 1; i < 50; i++)
    {
        if (i % 2 == 0)
            chart->AddData(100.0 + i * 0.1, std::chrono::system_clock::now());
        else
            chart->AddData(100.0 - i * 0.1, std::chrono::system_clock::now());
    }

    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    if (const int colCount = chart->GetColumnCount(); colCount >= 20)
    {
        if (const int idx = colCount - 1; bb.HasValue(idx))
        {
            double upperBand = bb.GetUpperBand(idx);
            EXPECT_TRUE(bb.IsAboveUpperBand(idx, upperBand + 1.0));
            EXPECT_FALSE(bb.IsAboveUpperBand(idx, upperBand - 1.0));
        }
    }
}

TEST_F(PnFBollingerBandsTest, IsBelowLowerBandDetectsCorrectly)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    for (int i = 1; i < 50; i++)
    {
        if (i % 2 == 0)
            chart->AddData(100.0 + i * 0.1, std::chrono::system_clock::now());
        else
            chart->AddData(100.0 - i * 0.1, std::chrono::system_clock::now());
    }

    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    int colCount = chart->GetColumnCount();
    if (colCount >= 20)
    {
        int idx = colCount - 1;
        if (bb.HasValue(idx))
        {
            double lowerBand = bb.GetLowerBand(idx);
            EXPECT_TRUE(bb.IsBelowLowerBand(idx, lowerBand - 1.0));
            EXPECT_FALSE(bb.IsBelowLowerBand(idx, lowerBand + 1.0));
        }
    }
}

TEST_F(PnFBollingerBandsTest, ToStringReturnsValidFormat)
{
    PnFBollingerBands bb(20, 2.0);
    bb.Calculate(chart);

    std::string result = bb.ToString();
    EXPECT_FALSE(result.empty());
    EXPECT_NE(std::string::npos, result.find("Bollinger Bands"));
}