/// \file test_indicators.cpp
/// \brief Test indicators implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

class IndicatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        const Timestamp now = std::chrono::system_clock::now();
        for (int i = 0; i < 50; i++) {
            const double price = 100.0 + (i % 10) + (i * 0.1);
            chart.add_data(price, now);
        }
    }

    Chart chart;
};

TEST_F(IndicatorTest, MovingAverage) {
    MovingAverage sma(5);
    sma.calculate(chart);

    EXPECT_FALSE(sma.has_value(0));
    EXPECT_TRUE(sma.has_value(4));

    const double val = sma.value(5);
    EXPECT_GT(val, 0.0);
}

TEST_F(IndicatorTest, BollingerBands) {
    BollingerBands bb(5, 2.0);
    bb.calculate(chart);

    if (constexpr int col = 10; bb.has_value(col)) {
        const double upper = bb.upper(col);
        const double middle = bb.middle(col);
        const double lower = bb.lower(col);

        EXPECT_GT(upper, middle);
        EXPECT_GT(middle, lower);
    }
}

TEST_F(IndicatorTest, RSI) {
    RSI rsi(14);
    rsi.calculate(chart);

    if (chart.column_count() > 15) {
        const double val = rsi.value(15);
        EXPECT_GE(val, 0.0);
        EXPECT_LE(val, 100.0);
    }
}

TEST_F(IndicatorTest, SignalDetector) {
    SignalDetector detector;
    detector.detect(chart);

    const SignalType current = detector.current_signal();
    EXPECT_TRUE(current == SignalType::None ||
                current == SignalType::Buy ||
                current == SignalType::Sell);
}

TEST_F(IndicatorTest, PatternRecognizer) {
    PatternRecognizer recognizer;
    recognizer.detect(chart);

    const auto bullish = recognizer.bullish_patterns();
    const auto bearish = recognizer.bearish_patterns();

    for (const auto& p : bullish) {
        EXPECT_TRUE(is_bullish_pattern(p.type));
    }
    for (const auto& p : bearish) {
        EXPECT_TRUE(is_bearish_pattern(p.type));
    }
}

TEST_F(IndicatorTest, SupportResistance) {
    SupportResistance sr;
    sr.identify(chart);

    const auto support = sr.support_levels();
    const auto resistance = sr.resistance_levels();

    for (const auto& level : support) {
        EXPECT_TRUE(level.is_support);
    }
    for (const auto& level : resistance) {
        EXPECT_FALSE(level.is_support);
    }
}

TEST_F(IndicatorTest, BullishPercent) {
    BullishPercent bp;
    bp.calculate(chart);

    const double val = bp.value();
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 100.0);
}

TEST_F(IndicatorTest, CongestionDetector) {
    CongestionDetector detector;
    detector.detect(chart);

    for (const auto& zones = detector.zones(); const auto& zone : zones) {
        EXPECT_LE(zone.start_column, zone.end_column);
        EXPECT_LE(zone.low_price, zone.high_price);
    }
}

TEST_F(IndicatorTest, IndicatorsAggregate) {
    Indicators ind;
    ind.calculate(chart);

    EXPECT_NE(ind.sma_short(), nullptr);
    EXPECT_NE(ind.sma_medium(), nullptr);
    EXPECT_NE(ind.sma_long(), nullptr);
    EXPECT_NE(ind.bollinger(), nullptr);
    EXPECT_NE(ind.rsi(), nullptr);
    EXPECT_NE(ind.signals(), nullptr);
    EXPECT_NE(ind.patterns(), nullptr);
    EXPECT_NE(ind.support_resistance(), nullptr);

    std::string summary = ind.summary();
    EXPECT_FALSE(summary.empty());
}

TEST(TypesTest, PatternTypeStrings) {
    EXPECT_STREQ(pattern_type_to_string(PatternType::None), "None");
    EXPECT_STREQ(pattern_type_to_string(PatternType::DoubleTopBreakout), "Double Top Breakout");
    EXPECT_STREQ(pattern_type_to_string(PatternType::BullTrap), "Bull Trap");
}

TEST(TypesTest, PatternClassification) {
    EXPECT_TRUE(is_bullish_pattern(PatternType::DoubleTopBreakout));
    EXPECT_TRUE(is_bearish_pattern(PatternType::DoubleBottomBreakdown));
    EXPECT_FALSE(is_bullish_pattern(PatternType::None));
    EXPECT_FALSE(is_bearish_pattern(PatternType::None));
}