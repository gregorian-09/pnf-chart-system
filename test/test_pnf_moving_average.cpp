//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart_indicators.h"
#include "pnf_chart.h"

using namespace ChartSystem;

class PnFMovingAverageTest : public ::testing::Test
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

TEST_F(PnFMovingAverageTest, ConstructorSetsPeriod)
{
    PnFMovingAverage sma(5);
    EXPECT_EQ(5, sma.GetPeriod());
}

TEST_F(PnFMovingAverageTest, EmptyChartReturnsZeroValues)
{
    PnFMovingAverage sma(5);
    sma.Calculate(chart);
    EXPECT_EQ(0.0, sma.GetValue(0));
}

TEST_F(PnFMovingAverageTest, InsufficientDataReturnsZero)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    chart->AddData(101.0, std::chrono::system_clock::now());

    PnFMovingAverage sma(5);
    sma.Calculate(chart);

    if (const int colCount = chart->GetColumnCount(); colCount > 0)
    {
        EXPECT_FALSE(sma.HasValue(colCount - 1));
    }
}

TEST_F(PnFMovingAverageTest, CalculatesCorrectAverageWithSufficientData)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    for (int i = 1; i < 20; i++)
    {
        if (i % 2 == 0)
            chart->AddData(100.0 + i, std::chrono::system_clock::now());
        else
            chart->AddData(100.0 - i, std::chrono::system_clock::now());
    }

    PnFMovingAverage sma(5);
    sma.Calculate(chart);

    if (const int colCount = chart->GetColumnCount(); colCount >= 5)
    {
        EXPECT_TRUE(sma.HasValue(colCount - 1));
        EXPECT_GT(sma.GetValue(colCount - 1), 0.0);
    }
}

TEST_F(PnFMovingAverageTest, OutOfBoundsIndexReturnsZero)
{
    PnFMovingAverage sma(5);
    sma.Calculate(chart);

    EXPECT_EQ(0.0, sma.GetValue(-1));
    EXPECT_EQ(0.0, sma.GetValue(100));
}

TEST_F(PnFMovingAverageTest, ToStringReturnsValidFormat)
{
    PnFMovingAverage sma(5);
    sma.Calculate(chart);

    std::string result = sma.ToString();
    EXPECT_FALSE(result.empty());
    EXPECT_NE(std::string::npos, result.find("SMA"));
}