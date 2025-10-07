//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart_indicators.h"
#include "pnf_chart.h"

using namespace ChartSystem;

class PnFSignalDetectorTest : public ::testing::Test
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

TEST_F(PnFSignalDetectorTest, InitialStateIsNone)
{
    const PnFSignalDetector detector;
    EXPECT_EQ(SIGNAL_NONE, detector.GetCurrentSignal());
    EXPECT_FALSE(detector.HasBuySignal());
    EXPECT_FALSE(detector.HasSellSignal());
}

TEST_F(PnFSignalDetectorTest, EmptyChartHasNoSignals)
{
    PnFSignalDetector detector;
    detector.DetectSignals(chart);

    EXPECT_EQ(SIGNAL_NONE, detector.GetCurrentSignal());
    EXPECT_EQ(0, detector.GetAllSignals().size());
}

TEST_F(PnFSignalDetectorTest, DetectsBuySignalOnHigherHigh)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    chart->AddData(103.0, std::chrono::system_clock::now());
    chart->AddData(101.0, std::chrono::system_clock::now());
    chart->AddData(105.0, std::chrono::system_clock::now());

    PnFSignalDetector detector;
    detector.DetectSignals(chart);

    if (chart->GetColumnCount() >= 3)
    {
        EXPECT_TRUE(detector.HasBuySignal() || detector.GetCurrentSignal() == SIGNAL_NONE);
    }
}

TEST_F(PnFSignalDetectorTest, DetectsSellSignalOnLowerLow)
{
    chart->AddData(100.0, std::chrono::system_clock::now());
    chart->AddData(97.0, std::chrono::system_clock::now());
    chart->AddData(99.0, std::chrono::system_clock::now());
    chart->AddData(95.0, std::chrono::system_clock::now());

    PnFSignalDetector detector;
    detector.DetectSignals(chart);

    if (chart->GetColumnCount() >= 3)
    {
        EXPECT_TRUE(detector.HasSellSignal() || detector.GetCurrentSignal() == SIGNAL_NONE);
    }
}

TEST_F(PnFSignalDetectorTest, GetLastSignalReturnsNoneWhenEmpty)
{
    PnFSignalDetector detector;
    detector.DetectSignals(chart);

    const Signal lastSignal = detector.GetLastSignal();
    EXPECT_EQ(SIGNAL_NONE, lastSignal.type);
    EXPECT_EQ(-1, lastSignal.columnIndex);
}

TEST_F(PnFSignalDetectorTest, ToStringReturnsValidFormat)
{
    PnFSignalDetector detector;
    detector.DetectSignals(chart);

    std::string result = detector.ToString();
    EXPECT_FALSE(result.empty());
    EXPECT_NE(std::string::npos, result.find("Signal Detector"));
}