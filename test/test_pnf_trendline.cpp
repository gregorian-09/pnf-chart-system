//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_chart.h"

using namespace ChartSystem;

class PnFTrendLineTest : public ::testing::Test
{
protected:
    PnFTrendLine* trendline = nullptr;

    void SetUp() override
    {
        trendline = new PnFTrendLine(BULLISH_SUPPORT, 0, 100.0, 0, 1.0);
    }

    void TearDown() override
    {
        delete trendline;
    }
};

TEST_F(PnFTrendLineTest, Constructor)
{
    EXPECT_EQ(trendline->GetTrendLineType(), BULLISH_SUPPORT);
    EXPECT_TRUE(trendline->IsActive());
    EXPECT_EQ(trendline->GetTouchCount(), 0);
}

TEST_F(PnFTrendLineTest, GetPriceAtColumn)
{
    EXPECT_EQ(trendline->GetPriceAtColumn(0), 100.0);
    EXPECT_EQ(trendline->GetPriceAtColumn(1), 101.0);
    EXPECT_EQ(trendline->GetPriceAtColumn(5), 105.0);
}

TEST_F(PnFTrendLineTest, SetActive)
{
    trendline->SetActive(false);
    EXPECT_FALSE(trendline->IsActive());
}

TEST_F(PnFTrendLineTest, TestTrendLine)
{
    EXPECT_TRUE(trendline->TestTrendLine(1, 101.0));
    EXPECT_EQ(trendline->GetTouchCount(), 1);
    EXPECT_TRUE(trendline->WasTouched());
}