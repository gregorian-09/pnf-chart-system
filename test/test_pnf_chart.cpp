//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart.h"

using namespace ChartSystem;

class PointAndFigureChartTest : public ::testing::Test
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

TEST_F(PointAndFigureChartTest, Constructor)
{
    EXPECT_EQ(chart->GetConstructionType(), CLOSING_PRICE);
    EXPECT_EQ(chart->GetBoxSize(), 1.0);
    EXPECT_EQ(chart->GetReversalCount(), 3);
    EXPECT_EQ(chart->GetColumnCount(), 0);
}

TEST_F(PointAndFigureChartTest, AddFirstData)
{
    const auto now = std::chrono::system_clock::now();
    EXPECT_TRUE(chart->AddData(100.0, now));
    EXPECT_EQ(chart->GetColumnCount(), 1);
}

TEST_F(PointAndFigureChartTest, AddDataContinuation)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(100.0, now);
    chart->AddData(101.0, now);
    chart->AddData(102.0, now);
    EXPECT_EQ(chart->GetColumnCount(), 1);
}

TEST_F(PointAndFigureChartTest, AddDataReversal)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(100.0, now);
    chart->AddData(101.0, now);
    chart->AddData(102.0, now);
    chart->AddData(99.0, now);
    EXPECT_EQ(chart->GetColumnCount(), 2);
}

TEST_F(PointAndFigureChartTest, GetAllPrices)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(100.0, now);
    chart->AddData(101.0, now);
    chart->AddData(102.0, now);

    const auto prices = chart->GetAllPrices();
    EXPECT_EQ(prices.size(), 3);
    EXPECT_EQ(prices[0], 102.0);
    EXPECT_EQ(prices[2], 100.0);
}

TEST_F(PointAndFigureChartTest, Clear)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(100.0, now);
    chart->AddData(101.0, now);
    chart->Clear();
    EXPECT_EQ(chart->GetColumnCount(), 0);
}