//
// Created by gregorian on 06/10/2025.
//

#include <gtest/gtest.h>
#include "pnf_column.h"

using namespace ChartSystem;

class PnFColumnTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PnFColumnTest, Constructor)
{
    const PnFColumn column(COLUMN_X);
    EXPECT_EQ(column.GetColumnType(), COLUMN_X);
    EXPECT_EQ(column.GetBoxCount(), 0);
}

TEST_F(PnFColumnTest, AddBox)
{
    PnFColumn column(COLUMN_X);
    EXPECT_TRUE(column.AddBox(100.0, X));
    EXPECT_EQ(column.GetBoxCount(), 1);
}

TEST_F(PnFColumnTest, AddDuplicateBox)
{
    PnFColumn column(COLUMN_X);
    column.AddBox(100.0, X);
    EXPECT_FALSE(column.AddBox(100.0, X));
    EXPECT_EQ(column.GetBoxCount(), 1);
}

TEST_F(PnFColumnTest, GetHighestPrice)
{
    PnFColumn column(COLUMN_X);
    column.AddBox(100.0, X);
    column.AddBox(101.0, X);
    column.AddBox(102.0, X);
    EXPECT_EQ(column.GetHighestPrice(), 102.0);
}

TEST_F(PnFColumnTest, GetLowestPrice)
{
    PnFColumn column(COLUMN_O);
    column.AddBox(100.0, O);
    column.AddBox(99.0, O);
    column.AddBox(98.0, O);
    EXPECT_EQ(column.GetLowestPrice(), 98.0);
}

TEST_F(PnFColumnTest, RemoveBox)
{
    PnFColumn column(COLUMN_X);
    column.AddBox(100.0, X);
    column.AddBox(101.0, X);
    EXPECT_TRUE(column.RemoveBox(100.0));
    EXPECT_EQ(column.GetBoxCount(), 1);
}

TEST_F(PnFColumnTest, Clear)
{
    PnFColumn column(COLUMN_X);
    column.AddBox(100.0, X);
    column.AddBox(101.0, X);
    column.Clear();
    EXPECT_EQ(column.GetBoxCount(), 0);
}