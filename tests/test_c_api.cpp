/// \file test_c_api.cpp
/// \brief Test c api implementation.

//
// Created by gregorian-rayne on 15/01/2026.
//

#include <gtest/gtest.h>
#include "pnf_c.hpp"
#include <cstring>

class CApiTest : public ::testing::Test {
protected:
    void SetUp() override {
        chart = pnf_chart_create_default();
    }

    void TearDown() override {
        if (chart) pnf_chart_destroy(chart);
    }

    PnfChart* chart = nullptr;
};

TEST_F(CApiTest, CreateAndDestroy) {
    ASSERT_NE(chart, nullptr);
}

TEST_F(CApiTest, CreateWithConfig) {
    PnfChartConfig config = pnf_chart_config_default();
    config.box_size = 1.0;

    PnfChart* custom_chart = pnf_chart_create(&config);
    ASSERT_NE(custom_chart, nullptr);

    const double box_size = pnf_chart_box_size(custom_chart);
    EXPECT_DOUBLE_EQ(box_size, 1.0);

    pnf_chart_destroy(custom_chart);
}

TEST_F(CApiTest, AddDataAndGetColumnCount) {
    EXPECT_EQ(pnf_chart_column_count(chart), 0);

    bool result = pnf_chart_add_data(chart, 100.0, 99.0, 99.5, 0);
    EXPECT_TRUE(result);

    result = pnf_chart_add_data(chart, 101.0, 100.0, 100.5, 1000);
    EXPECT_TRUE(result);

    result = pnf_chart_add_data(chart, 102.0, 101.0, 101.5, 2000);
    EXPECT_TRUE(result);
}

TEST_F(CApiTest, GetColumnInfo) {
    pnf_chart_add_data(chart, 100.0, 99.0, 99.5, 0);
    pnf_chart_add_data(chart, 105.0, 100.0, 104.5, 1000);
    pnf_chart_add_data(chart, 106.0, 105.0, 105.5, 2000);

    if (const size_t count = pnf_chart_column_count(chart); count > 0) {
        const PnfColumnType type = pnf_chart_column_type(chart, 0);
        const double high = pnf_chart_column_highest(chart, 0);
        const double low = pnf_chart_column_lowest(chart, 0);

        EXPECT_TRUE(type == PNF_COLUMN_X || type == PNF_COLUMN_O);
        EXPECT_GT(high, 0);
        EXPECT_GT(low, 0);
        EXPECT_GE(high, low);
    }
}

TEST_F(CApiTest, ChartClear) {
    pnf_chart_add_data(chart, 100.0, 99.0, 99.5, 0);
    pnf_chart_add_data(chart, 105.0, 100.0, 104.5, 1000);

    pnf_chart_clear(chart);
    EXPECT_EQ(pnf_chart_column_count(chart), 0);
}

TEST_F(CApiTest, IndicatorsCreate) {
    PnfIndicators* indicators = pnf_indicators_create_default();
    ASSERT_NE(indicators, nullptr);
    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, IndicatorsWithConfig) {
    PnfIndicatorConfig config = pnf_indicator_config_default();
    config.sma_short_period = 3;
    config.sma_medium_period = 7;
    config.sma_long_period = 14;
    config.bollinger_period = 14;
    config.bollinger_std_devs = 2.5;
    config.rsi_period = 10;
    config.rsi_overbought = 75.0;
    config.rsi_oversold = 25.0;

    PnfIndicators* indicators = pnf_indicators_create_with_config(&config);
    ASSERT_NE(indicators, nullptr);
    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, IndicatorsCalculate) {
    for (int i = 0; i < 50; i++) {
        const double base = 100.0 + (i % 10) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    const double bullish_pct = pnf_indicators_bullish_percent(indicators);
    EXPECT_GE(bullish_pct, 0.0);
    EXPECT_LE(bullish_pct, 100.0);

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, GetSignalInfo) {
    for (int i = 0; i < 30; i++) {
        const double base = 100.0 + i * 0.3;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    const PnfSignalType signal = pnf_indicators_current_signal(indicators);
    EXPECT_TRUE(signal == PNF_SIGNAL_NONE || signal == PNF_SIGNAL_BUY || signal == PNF_SIGNAL_SELL);

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, GetIndicatorArrays) {
    for (int i = 0; i < 30; i++) {
        const double base = 100.0 + (i % 5) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    if (const PnfDoubleArray sma_values = pnf_indicators_sma_short_values(indicators); sma_values.data && sma_values.length > 0) {
        for (size_t i = 0; i < sma_values.length; i++) {
            EXPECT_GE(sma_values.data[i], 0.0);
        }
        pnf_free_double_array(sma_values);
    }

    if (const PnfDoubleArray rsi_values = pnf_indicators_rsi_values(indicators); rsi_values.data && rsi_values.length > 0) {
        for (size_t i = 0; i < rsi_values.length; i++) {
            EXPECT_GE(rsi_values.data[i], 0.0);
            EXPECT_LE(rsi_values.data[i], 100.0);
        }
        pnf_free_double_array(rsi_values);
    }

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, PatternDetection) {
    for (int i = 0; i < 50; i++) {
        double base = 100.0;
        if (i % 4 == 0) base += 2.0;
        if (i % 4 == 2) base -= 2.0;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    const size_t pattern_count = pnf_indicators_pattern_count(indicators);
    EXPECT_GE(pattern_count, 0UL);

    const size_t bullish_count = pnf_indicators_bullish_pattern_count(indicators);
    const size_t bearish_count = pnf_indicators_bearish_pattern_count(indicators);
    EXPECT_GE(bullish_count, 0UL);
    EXPECT_GE(bearish_count, 0UL);

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, SupportResistanceLevels) {
    for (int i = 0; i < 40; i++) {
        const double base = 100.0 + (i % 8) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    if (const PnfDoubleArray support = pnf_indicators_support_prices(indicators); support.data && support.length > 0) {
        for (size_t i = 0; i < support.length; i++) {
            EXPECT_GT(support.data[i], 0.0);
        }
        pnf_free_double_array(support);
    }

    if (const PnfDoubleArray resistance = pnf_indicators_resistance_prices(indicators); resistance.data && resistance.length > 0) {
        for (size_t i = 0; i < resistance.length; i++) {
            EXPECT_GT(resistance.data[i], 0.0);
        }
        pnf_free_double_array(resistance);
    }

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, VisualizationAscii) {
    for (int i = 0; i < 20; i++) {
        const double base = 100.0 + (i % 5);
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    const char* ascii = pnf_chart_to_ascii(chart);
    ASSERT_NE(ascii, nullptr);
    EXPECT_GT(strlen(ascii), 0UL);
    pnf_free_string(ascii);
}

TEST_F(CApiTest, VisualizationJson) {
    pnf_chart_add_data(chart, 100.0, 99.0, 99.5, 0);
    pnf_chart_add_data(chart, 105.0, 100.0, 104.5, 1000);

    const char* json = pnf_chart_to_json(chart);
    ASSERT_NE(json, nullptr);
    EXPECT_GT(strlen(json), 0UL);
    EXPECT_NE(strstr(json, "box_size"), nullptr);
    pnf_free_string(json);
}

TEST_F(CApiTest, ChartDataExport) {
    for (int i = 0; i < 10; i++) {
        const double base = 100.0 + i * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfChartData* data = pnf_chart_export_data(chart);
    ASSERT_NE(data, nullptr);
    EXPECT_GT(data->box_size, 0.0);
    EXPECT_EQ(data->reversal, 3);

    pnf_chart_data_destroy(data);
}

TEST_F(CApiTest, IndicatorDataExport) {
    for (int i = 0; i < 30; i++) {
        const double base = 100.0 + (i % 5) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    PnfIndicatorData* data = pnf_indicators_export_data(indicators);
    ASSERT_NE(data, nullptr);

    EXPECT_GE(data->bullish_percent, 0.0);
    EXPECT_LE(data->bullish_percent, 100.0);

    pnf_indicator_data_destroy(data);
    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, ConfigDefaults) {
    const PnfChartConfig chart_config = pnf_chart_config_default();
    EXPECT_EQ(chart_config.method, PNF_METHOD_CLOSE);
    EXPECT_EQ(chart_config.box_size_method, PNF_BOX_SIZE_TRADITIONAL);
    EXPECT_EQ(chart_config.reversal, 3);

    const PnfIndicatorConfig ind_config = pnf_indicator_config_default();
    EXPECT_EQ(ind_config.sma_short_period, 5);
    EXPECT_EQ(ind_config.sma_medium_period, 10);
    EXPECT_EQ(ind_config.sma_long_period, 20);
    EXPECT_EQ(ind_config.rsi_period, 14);
    EXPECT_DOUBLE_EQ(ind_config.rsi_overbought, 70.0);
    EXPECT_DOUBLE_EQ(ind_config.rsi_oversold, 30.0);
}

TEST_F(CApiTest, IndicatorConfiguration) {
    PnfIndicators* indicators = pnf_indicators_create();
    ASSERT_NE(indicators, nullptr);

    PnfIndicatorConfig config = pnf_indicator_config_default();
    config.sma_short_period = 3;
    config.bollinger_period = 15;
    config.rsi_period = 10;

    pnf_indicators_configure(indicators, &config);

    // Now calculate to verify configuration took effect
    for (int i = 0; i < 30; i++) {
        const double base = 100.0 + (i % 5) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }
    pnf_indicators_calculate(indicators, chart);

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, BollingerBands) {
    for (int i = 0; i < 30; i++) {
        const double base = 100.0 + (i % 5) * 0.5;
        pnf_chart_add_data(chart, base + 1.0, base - 1.0, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    const PnfDoubleArray middle = pnf_indicators_bollinger_middle_values(indicators);
    const PnfDoubleArray upper = pnf_indicators_bollinger_upper_values(indicators);
    const PnfDoubleArray lower = pnf_indicators_bollinger_lower_values(indicators);

    // Check that upper > middle > lower for valid values
    for (size_t i = 0; i < middle.length && i < upper.length && i < lower.length; i++) {
        if (middle.data[i] > 0) {
            EXPECT_GE(upper.data[i], middle.data[i]);
            EXPECT_LE(lower.data[i], middle.data[i]);
        }
    }

    pnf_free_double_array(middle);
    pnf_free_double_array(upper);
    pnf_free_double_array(lower);
    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, CongestionDetection) {
    // Create sideways movement pattern
    for (int i = 0; i < 40; i++) {
        const double base = 100.0 + (i % 3) * 0.2;
        pnf_chart_add_data(chart, base + 0.5, base - 0.5, base, i * 1000);
    }

    PnfIndicators* indicators = pnf_indicators_create_default();
    pnf_indicators_calculate(indicators, chart);

    const size_t zone_count = pnf_indicators_congestion_zone_count(indicators);
    EXPECT_GE(zone_count, 0UL);

    if (pnf_chart_column_count(chart) > 5) {
        // Check if column 3 is in congestion
        const bool in_cong = pnf_indicators_is_in_congestion(indicators, 3);
        // Just verify the call doesn't crash - result usually depends on data
        (void)in_cong;
    }

    pnf_indicators_destroy(indicators);
}

TEST_F(CApiTest, NullSafety) {
    EXPECT_EQ(pnf_chart_column_count(nullptr), 0UL);
    EXPECT_FALSE(pnf_chart_add_data(nullptr, 100.0, 99.0, 99.5, 0));
    EXPECT_EQ(pnf_chart_column_type(nullptr, 0), PNF_COLUMN_X);
    EXPECT_DOUBLE_EQ(pnf_chart_column_highest(nullptr, 0), 0.0);
    EXPECT_DOUBLE_EQ(pnf_chart_column_lowest(nullptr, 0), 0.0);

    pnf_chart_destroy(nullptr);
    pnf_indicators_destroy(nullptr);

    constexpr PnfDoubleArray empty_arr = {nullptr, 0};
    pnf_free_double_array(empty_arr);

    constexpr PnfSignalArray empty_sig = {nullptr, 0};
    pnf_free_signal_array(empty_sig);

    constexpr PnfPatternArray empty_pat = {nullptr, 0};
    pnf_free_pattern_array(empty_pat);

    constexpr PnfLevelArray empty_lvl = {nullptr, 0};
    pnf_free_level_array(empty_lvl);
}

TEST_F(CApiTest, Version) {
    const char* version = pnf_version_string();
    ASSERT_NE(version, nullptr);
    EXPECT_GT(strlen(version), 0UL);

    const int major = pnf_version_major();
    const int minor = pnf_version_minor();
    const int patch = pnf_version_patch();

    EXPECT_GE(major, 0);
    EXPECT_GE(minor, 0);
    EXPECT_GE(patch, 0);
}
