/// \file test_trendline.cpp
/// \brief Test trendline implementation.

//
// Created by gregorian-rayne on 15/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

TEST(TrendLineTest, BullishSupport) {
    const TrendLine line(TrendLineType::BullishSupport, 0, 100.0, 0, 1.0);

    EXPECT_EQ(line.type(), TrendLineType::BullishSupport);
    EXPECT_TRUE(line.is_active());
    EXPECT_DOUBLE_EQ(line.price_at_column(0), 100.0);
    EXPECT_DOUBLE_EQ(line.price_at_column(1), 101.0);
    EXPECT_DOUBLE_EQ(line.price_at_column(5), 105.0);
}

TEST(TrendLineTest, BearishResistance) {
    const TrendLine line(TrendLineType::BearishResistance, 0, 100.0, 0, 1.0);

    EXPECT_EQ(line.type(), TrendLineType::BearishResistance);
    EXPECT_DOUBLE_EQ(line.price_at_column(0), 100.0);
    EXPECT_DOUBLE_EQ(line.price_at_column(1), 99.0);
    EXPECT_DOUBLE_EQ(line.price_at_column(5), 95.0);
}

TEST(TrendLineTest, IsBroken) {
    const TrendLine bullish(TrendLineType::BullishSupport, 0, 100.0, 0, 1.0);

    EXPECT_FALSE(bullish.is_broken(0, 100.0));
    EXPECT_FALSE(bullish.is_broken(5, 105.0));
    EXPECT_TRUE(bullish.is_broken(5, 100.0));
}

TEST(TrendLineTest, TouchCount) {
    TrendLine line(TrendLineType::BullishSupport, 0, 100.0, 0, 1.0);

    EXPECT_EQ(line.touch_count(), 0);
    line.test(1, 101.0);
    EXPECT_EQ(line.touch_count(), 1);
    EXPECT_TRUE(line.was_touched());
}

TEST(TrendLineTest, SetActive) {
    TrendLine line(TrendLineType::BullishSupport, 0, 100.0, 0, 1.0);
    EXPECT_TRUE(line.is_active());
    line.set_active(false);
    EXPECT_FALSE(line.is_active());
}

TEST(TrendLineManagerTest, Creation) {
    const TrendLineManager manager(1.0);

    EXPECT_EQ(manager.active_trend_line(), nullptr);
    EXPECT_FALSE(manager.has_bullish_bias());
    EXPECT_FALSE(manager.has_bearish_bias());
}

TEST(TrendLineManagerTest, Clear) {
    TrendLineManager manager(1.0);
    manager.clear();
    EXPECT_EQ(manager.active_trend_line(), nullptr);
}