/// \file test_chart.cpp
/// \brief Test chart implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

class ChartTest : public ::testing::Test {
protected:
    Chart chart;
    Timestamp now = std::chrono::system_clock::now();
};

TEST_F(ChartTest, DefaultCreation) {
    EXPECT_EQ(chart.column_count(), 0u);
}

TEST_F(ChartTest, ConfigCreation) {
    ChartConfig cfg;
    cfg.method = ConstructionMethod::HighLow;
    cfg.box_size_method = BoxSizeMethod::Percentage;
    cfg.box_size = 2.0;
    cfg.reversal = 3;

    const Chart configured(cfg);
    EXPECT_EQ(configured.column_count(), 0u);
}

TEST_F(ChartTest, AddFirstData) {
    chart.add_data(100.0, 95.0, 98.0, now);
    EXPECT_EQ(chart.column_count(), 1u);
}

TEST_F(ChartTest, AddSinglePrice) {
    chart.add_data(100.0, now);
    EXPECT_EQ(chart.column_count(), 1u);
}

TEST_F(ChartTest, ContinueColumn) {
    chart.add_data(100.0, now);
    chart.add_data(101.0, now);
    chart.add_data(102.0, now);
    EXPECT_EQ(chart.column_count(), 1u);
}

TEST_F(ChartTest, CreateReversal) {
    ChartConfig cfg;
    cfg.reversal = 3;
    Chart c(cfg);

    c.add_data(100.0, now);
    c.add_data(105.0, now);
    c.add_data(110.0, now);
    c.add_data(100.0, now);

    EXPECT_GT(c.column_count(), 1u);
}

TEST_F(ChartTest, XOColumnCount) {
    ChartConfig cfg;
    cfg.reversal = 3;
    Chart c(cfg);

    c.add_data(100.0, now);
    c.add_data(110.0, now);
    c.add_data(90.0, now);
    c.add_data(100.0, now);

    const size_t x_count = c.x_column_count();
    const size_t o_count = c.o_column_count();
    EXPECT_EQ(x_count + o_count + c.mixed_column_count(), c.column_count());
}

TEST_F(ChartTest, AllPrices) {
    chart.add_data(100.0, now);
    chart.add_data(101.0, now);

    const auto prices = chart.all_prices();
    EXPECT_FALSE(prices.empty());
}

TEST_F(ChartTest, Clear) {
    chart.add_data(100.0, now);
    chart.clear();
    EXPECT_EQ(chart.column_count(), 0u);
}

TEST_F(ChartTest, LastColumn) {
    EXPECT_EQ(chart.last_column(), nullptr);
    chart.add_data(100.0, now);
    EXPECT_NE(chart.last_column(), nullptr);
}

TEST_F(ChartTest, TrendBias) {
    ChartConfig cfg;
    cfg.reversal = 3;
    Chart c(cfg);

    for (int i = 0; i < 50; i++) {
        const double price = 100.0 + (i % 10);
        c.add_data(price, now);
    }

    const bool bullish = c.has_bullish_bias();
    const bool bearish = c.has_bearish_bias();
    EXPECT_FALSE(bullish && bearish);
}

TEST_F(ChartTest, ToString) {
    chart.add_data(100.0, now);
    std::string str = chart.to_string();
    EXPECT_FALSE(str.empty());
    EXPECT_NE(str.find("Point & Figure"), std::string::npos);
}