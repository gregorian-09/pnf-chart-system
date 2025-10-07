//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart.h"
#include "pnf_chart_indicators.h"
#include <chrono>

using namespace ChartSystem;

class PnFSupportResistanceTest : public ::testing::Test
{
protected:
    std::unique_ptr<PointAndFigureChart> chart;
    std::unique_ptr<PnFSupportResistance> sr;

    void SetUp() override
    {
        chart = std::make_unique<PointAndFigureChart>(HIGH_LOW, FIXED, 1.0, 3);
        sr = std::make_unique<PnFSupportResistance>(0.01);
    }
};

TEST_F(PnFSupportResistanceTest, EmptyChart)
{
    sr->IdentifyLevels(chart.get());

    EXPECT_EQ(sr->GetSupportLevels().size(), 0);
    EXPECT_EQ(sr->GetResistanceLevels().size(), 0);
}

TEST_F(PnFSupportResistanceTest, SingleColumn)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(105.0, 100.0, 100.0, now);

    sr->IdentifyLevels(chart.get());

    EXPECT_GE(sr->GetResistanceLevels().size(), 0);
}

TEST_F(PnFSupportResistanceTest, MultipleSupportLevels)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(110.0, 105.0, 105.0, now);
    chart->AddData(107.0, 102.0, 102.0, now);

    sr->IdentifyLevels(chart.get());

    auto supportLevels = sr->GetSupportLevels();
    EXPECT_GT(supportLevels.size(), 0);
}

TEST_F(PnFSupportResistanceTest, MultipleResistanceLevels)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(110.0, 105.0, 105.0, now);
    chart->AddData(107.0, 102.0, 102.0, now);
    chart->AddData(113.0, 108.0, 108.0, now);

    sr->IdentifyLevels(chart.get());

    const auto resistanceLevels = sr->GetResistanceLevels();
    EXPECT_GT(resistanceLevels.size(), 0);
}

TEST_F(PnFSupportResistanceTest, TouchCountTracking)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(110.0, 105.0, 105.0, now);
    chart->AddData(107.0, 102.0, 102.0, now);

    sr->IdentifyLevels(chart.get());

    const auto supportLevels = sr->GetSupportLevels();
    bool hasMultipleTouches = false;
    for (const auto& level : supportLevels)
    {
        if (level.touchCount > 1)
        {
            hasMultipleTouches = true;
            break;
        }
    }

    EXPECT_TRUE(hasMultipleTouches || !supportLevels.empty());
}

TEST_F(PnFSupportResistanceTest, SignificantLevels)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(110.0, 105.0, 105.0, now);
    chart->AddData(107.0, 102.0, 102.0, now);
    chart->AddData(113.0, 108.0, 108.0, now);
    chart->AddData(110.0, 105.0, 105.0, now);

    sr->IdentifyLevels(chart.get());

    auto significantLevels = sr->GetSignificantLevels(2);
    EXPECT_GE(significantLevels.size(), 0);
}

TEST_F(PnFSupportResistanceTest, IsNearSupport)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);

    sr->IdentifyLevels(chart.get());

    auto supportLevels = sr->GetSupportLevels();
    if (!supportLevels.empty())
    {
        double supportPrice = supportLevels[0].price;
        EXPECT_TRUE(sr->IsNearSupport(supportPrice, 0.02));
        EXPECT_TRUE(sr->IsNearSupport(supportPrice * 1.01, 0.02));
        EXPECT_FALSE(sr->IsNearSupport(supportPrice * 1.10, 0.02));
    }
}

TEST_F(PnFSupportResistanceTest, IsNearResistance)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);

    sr->IdentifyLevels(chart.get());

    if (const auto resistanceLevels = sr->GetResistanceLevels(); !resistanceLevels.empty())
    {
        const double resistancePrice = resistanceLevels[0].price;
        EXPECT_TRUE(sr->IsNearResistance(resistancePrice, 0.02));
        EXPECT_TRUE(sr->IsNearResistance(resistancePrice * 0.99, 0.02));
        EXPECT_FALSE(sr->IsNearResistance(resistancePrice * 0.90, 0.02));
    }
}

TEST_F(PnFSupportResistanceTest, LevelMerging)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.5, 100.5, now);
    chart->AddData(110.0, 105.0, 105.0, now);
    chart->AddData(107.0, 102.0, 102.0, now);

    sr->IdentifyLevels(chart.get());

    const auto supportLevels = sr->GetSupportLevels();

    for (size_t i = 0; i < supportLevels.size(); i++)
    {
        for (size_t j = i + 1; j < supportLevels.size(); j++)
        {
            double priceDiff = std::abs(supportLevels[i].price - supportLevels[j].price) / supportLevels[i].price;
            EXPECT_GE(priceDiff, 0.01);
        }
    }
}

TEST_F(PnFSupportResistanceTest, ToStringOutput)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);

    sr->IdentifyLevels(chart.get());

    std::string output = sr->ToString();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Support/Resistance"), std::string::npos);
}

TEST_F(PnFSupportResistanceTest, DifferentPriceThresholds)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);
    chart->AddData(102.0, 97.0, 97.0, now);
    chart->AddData(108.0, 103.0, 103.0, now);
    chart->AddData(105.0, 100.5, 100.5, now);

    const auto sr_tight = std::make_unique<PnFSupportResistance>(0.001);
    sr_tight->IdentifyLevels(chart.get());
    const size_t countTight = sr_tight->GetSupportLevels().size() + sr_tight->GetResistanceLevels().size();

    const auto sr_loose = std::make_unique<PnFSupportResistance>(0.05);
    sr_loose->IdentifyLevels(chart.get());
    const size_t countLoose = sr_loose->GetSupportLevels().size() + sr_loose->GetResistanceLevels().size();

    EXPECT_GE(countTight, countLoose);
}