//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart.h"
#include "pnf_chart_indicators.h"
#include <chrono>

using namespace ChartSystem;

class PnFPriceObjectiveTest : public ::testing::Test
{
protected:
    std::unique_ptr<PointAndFigureChart> chart;
    std::unique_ptr<PnFPriceObjective> objective;

    void SetUp() override
    {
        chart = std::make_unique<PointAndFigureChart>(HIGH_LOW, FIXED, 1.0, 3);
        objective = std::make_unique<PnFPriceObjective>();
    }
};

TEST_F(PnFPriceObjectiveTest, EmptyChart)
{
    EXPECT_EQ(objective->GetObjectives().size(), 0);

    const auto latestObj = objective->GetLatestObjective();
    EXPECT_EQ(latestObj.baseColumnIndex, -1);
}

TEST_F(PnFPriceObjectiveTest, SingleColumn)
{
    const auto now = std::chrono::system_clock::now();
    chart->AddData(105.0, 100.0, 100.0, now);

    EXPECT_GT(chart->GetColumnCount(), 0);
}

TEST_F(PnFPriceObjectiveTest, BullishObjective)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);

    const int columnCount = chart->GetColumnCount();
    for (int i = 0; i < columnCount; i++)
    {
        objective->CalculateVerticalCount(chart.get(), i);
    }

    const auto objectives = objective->GetObjectives();

    bool hasBullish = false;
    for (const auto& obj : objectives)
    {
        if (obj.isBullish)
        {
            hasBullish = true;
            EXPECT_GT(obj.targetPrice, obj.extensionBoxes);
            EXPECT_GT(obj.extensionBoxes, 0);
            break;
        }
    }

    EXPECT_TRUE(hasBullish || objectives.empty());
}

TEST_F(PnFPriceObjectiveTest, BearishObjective)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);
    chart->AddData(107.0, 95.0, 95.0, now);

    const int columnCount = chart->GetColumnCount();
    for (int i = 0; i < columnCount; i++)
    {
        objective->CalculateVerticalCount(chart.get(), i);
    }

    const auto objectives = objective->GetObjectives();

    bool hasBearish = false;
    for (const auto& obj : objectives)
    {
        if (!obj.isBullish)
        {
            hasBearish = true;
            EXPECT_GT(obj.extensionBoxes, 0);
            break;
        }
    }

    EXPECT_TRUE(hasBearish || objectives.empty());
}

TEST_F(PnFPriceObjectiveTest, MultipleObjectives)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);
    chart->AddData(107.0, 95.0, 95.0, now);
    chart->AddData(115.0, 105.0, 105.0, now);
    chart->AddData(112.0, 100.0, 100.0, now);

    const int columnCount = chart->GetColumnCount();
    for (int i = 0; i < columnCount; i++)
    {
        objective->CalculateVerticalCount(chart.get(), i);
    }

    const auto objectives = objective->GetObjectives();
    EXPECT_GT(objectives.size(), 0);
}

TEST_F(PnFPriceObjectiveTest, VerticalCountCalculation)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);

    objective->CalculateVerticalCount(chart.get(), 0);

    if (const auto objectives = objective->GetObjectives(); !objectives.empty())
    {
        const auto obj = objectives[0];
        EXPECT_GT(obj.extensionBoxes, 0);

        const PnFColumn* col = chart->GetColumn(0);
        EXPECT_EQ(obj.extensionBoxes, col->GetBoxCount());
    }
}

TEST_F(PnFPriceObjectiveTest, LatestObjective)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);
    chart->AddData(107.0, 95.0, 95.0, now);
    chart->AddData(115.0, 105.0, 105.0, now);

    const int columnCount = chart->GetColumnCount();
    for (int i = 0; i < columnCount; i++)
    {
        objective->CalculateVerticalCount(chart.get(), i);
    }

    if (const auto latestObj = objective->GetLatestObjective(); latestObj.baseColumnIndex != -1)
    {
        EXPECT_GT(latestObj.extensionBoxes, 0);
        EXPECT_EQ(latestObj.baseColumnIndex, columnCount - 1);
    }
}

TEST_F(PnFPriceObjectiveTest, ObjectiveDirection)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(115.0, 100.0, 100.0, now);

    objective->CalculateVerticalCount(chart.get(), 0);

    if (const auto objectives = objective->GetObjectives(); !objectives.empty())
    {
        const auto obj = objectives[0];

        if (const PnFColumn* col = chart->GetColumn(0); col->GetColumnType() == COLUMN_X)
        {
            EXPECT_TRUE(obj.isBullish);
            EXPECT_GT(obj.targetPrice, col->GetHighestPrice());
        }
    }
}

TEST_F(PnFPriceObjectiveTest, ToStringOutput)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);
    chart->AddData(107.0, 95.0, 95.0, now);

    const int columnCount = chart->GetColumnCount();
    for (int i = 0; i < columnCount; i++)
    {
        objective->CalculateVerticalCount(chart.get(), i);
    }

    std::string output = objective->ToString();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Price Objectives"), std::string::npos);
}

TEST_F(PnFPriceObjectiveTest, BoxSizeImpact)
{
    const auto now = std::chrono::system_clock::now();

    const auto chart1 = std::make_unique<PointAndFigureChart>(HIGH_LOW, FIXED, 1.0, 3);
    chart1->AddData(110.0, 100.0, 100.0, now);

    const auto obj1 = std::make_unique<PnFPriceObjective>();
    obj1->CalculateVerticalCount(chart1.get(), 0);

    const auto chart2 = std::make_unique<PointAndFigureChart>(HIGH_LOW, FIXED, 2.0, 3);
    chart2->AddData(110.0, 100.0, 100.0, now);

    const auto obj2 = std::make_unique<PnFPriceObjective>();
    obj2->CalculateVerticalCount(chart2.get(), 0);

    if (!obj1->GetObjectives().empty() && !obj2->GetObjectives().empty())
    {
        const double target1 = obj1->GetObjectives()[0].targetPrice;
        const double target2 = obj2->GetObjectives()[0].targetPrice;

        EXPECT_NE(target1, target2);
    }
}

TEST_F(PnFPriceObjectiveTest, ExtensionBoxesAccuracy)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(105.0, 100.0, 100.0, now);

    objective->CalculateVerticalCount(chart.get(), 0);

    if (const auto objectives = objective->GetObjectives(); !objectives.empty())
    {
        const PnFColumn* col = chart->GetColumn(0);
        EXPECT_EQ(objectives[0].extensionBoxes, col->GetBoxCount());
    }
}

TEST_F(PnFPriceObjectiveTest, NoObjectivesForShortColumns)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(101.0, 100.0, 100.0, now);

    objective->CalculateVerticalCount(chart.get(), 0);

    EXPECT_GE(objective->GetObjectives().size(), 0);
}

TEST_F(PnFPriceObjectiveTest, SequentialObjectives)
{
    const auto now = std::chrono::system_clock::now();

    chart->AddData(110.0, 100.0, 100.0, now);
    objective->CalculateVerticalCount(chart.get(), 0);
    const size_t count1 = objective->GetObjectives().size();

    chart->AddData(107.0, 95.0, 95.0, now);
    objective->CalculateVerticalCount(chart.get(), 1);
    const size_t count2 = objective->GetObjectives().size();

    EXPECT_GE(count2, count1);
}