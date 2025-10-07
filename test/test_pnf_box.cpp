//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_column.h"

using namespace ChartSystem;

class PnFBoxTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PnFBoxTest, Constructor)
{
    const PnFBox box(100.0, X);
    EXPECT_EQ(box.GetPrice(), 100.0);
    EXPECT_EQ(box.GetBoxType(), X);
    EXPECT_TRUE(box.GetMarker().empty());
}

TEST_F(PnFBoxTest, ConstructorWithMarker)
{
    const PnFBox box(100.0, X, "A");
    EXPECT_EQ(box.GetPrice(), 100.0);
    EXPECT_EQ(box.GetBoxType(), X);
    EXPECT_EQ(box.GetMarker(), "A");
}

TEST_F(PnFBoxTest, SetMarker)
{
    PnFBox box(100.0, O);
    box.SetMarker("B");
    EXPECT_EQ(box.GetMarker(), "B");
}