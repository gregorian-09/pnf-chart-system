/// \file test_column.cpp
/// \brief Test column implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

TEST(ColumnTest, Creation) {
    const Column col(ColumnType::X);
    EXPECT_EQ(col.type(), ColumnType::X);
    EXPECT_EQ(col.box_count(), 0u);
}

TEST(ColumnTest, AddBox) {
    Column col(ColumnType::X);
    EXPECT_TRUE(col.add_box(100.0, BoxType::X));
    EXPECT_EQ(col.box_count(), 1u);
    EXPECT_TRUE(col.has_box(100.0));
}

TEST(ColumnTest, AddDuplicateBox) {
    Column col(ColumnType::X);
    col.add_box(100.0, BoxType::X);
    EXPECT_FALSE(col.add_box(100.0, BoxType::X));
    EXPECT_EQ(col.box_count(), 1u);
}

TEST(ColumnTest, AddBoxWithMarker) {
    Column col(ColumnType::X);
    EXPECT_TRUE(col.add_box(100.0, BoxType::X, "1"));
    Box* box = col.get_box(100.0);
    ASSERT_NE(box, nullptr);
    EXPECT_EQ(box->marker(), "1");
}

TEST(ColumnTest, RemoveBox) {
    Column col(ColumnType::X);
    col.add_box(100.0, BoxType::X);
    col.add_box(101.0, BoxType::X);
    EXPECT_TRUE(col.remove_box(100.0));
    EXPECT_EQ(col.box_count(), 1u);
    EXPECT_FALSE(col.has_box(100.0));
}

TEST(ColumnTest, HighestLowestPrice) {
    Column col(ColumnType::X);
    col.add_box(100.0, BoxType::X);
    col.add_box(102.0, BoxType::X);
    col.add_box(101.0, BoxType::X);

    EXPECT_DOUBLE_EQ(col.highest_price(), 102.0);
    EXPECT_DOUBLE_EQ(col.lowest_price(), 100.0);
}

TEST(ColumnTest, Clear) {
    Column col(ColumnType::X);
    col.add_box(100.0, BoxType::X);
    col.add_box(101.0, BoxType::X);
    col.clear();
    EXPECT_EQ(col.box_count(), 0u);
}

TEST(ColumnTest, GetBoxAt) {
    Column col(ColumnType::X);
    col.add_box(100.0, BoxType::X);
    col.add_box(101.0, BoxType::X);

    Box* box = col.get_box_at(0);
    ASSERT_NE(box, nullptr);
    EXPECT_DOUBLE_EQ(box->price(), 100.0);
}