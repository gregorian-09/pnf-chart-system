/// \file pnf_c.cpp
/// \brief Public API.

//
// Created by gregorian-rayne on 15/01/2026.
//

#include "pnf_c.hpp"
#include "pnf/pnf.hpp"
#include <cstring>
#include <string>
#include <vector>

static PnfDoubleArray make_double_array(const std::vector<double>& vec) {
    PnfDoubleArray arr;
    arr.length = vec.size();
    if (arr.length > 0) {
        arr.data = new double[arr.length];
        std::memcpy(arr.data, vec.data(), arr.length * sizeof(double));
    } else {
        arr.data = nullptr;
    }
    return arr;
}

static char* duplicate_string(const std::string& value) {
    const size_t len = value.size();
    char* buffer = new char[len + 1];
    std::memcpy(buffer, value.c_str(), len);
    buffer[len] = '\0';
    return buffer;
}

static pnf::IndicatorConfig to_cpp_config(const PnfIndicatorConfig* config) {
    pnf::IndicatorConfig cfg;
    if (config) {
        cfg.sma_short_period = config->sma_short_period;
        cfg.sma_medium_period = config->sma_medium_period;
        cfg.sma_long_period = config->sma_long_period;
        cfg.bollinger_period = config->bollinger_period;
        cfg.bollinger_std_devs = config->bollinger_std_devs;
        cfg.rsi_period = config->rsi_period;
        cfg.rsi_overbought = config->rsi_overbought;
        cfg.rsi_oversold = config->rsi_oversold;
        cfg.bullish_alert_threshold = config->bullish_alert_threshold;
        cfg.bearish_alert_threshold = config->bearish_alert_threshold;
        cfg.support_resistance_threshold = config->support_resistance_threshold;
        cfg.congestion_min_columns = config->congestion_min_columns;
        cfg.congestion_price_range = config->congestion_price_range;
    }
    return cfg;
}

extern "C" {

    const char* pnf_version_string(void) {
        return duplicate_string(pnf::Version::string);
    }

    int pnf_version_major(void) {
        return pnf::Version::major;
    }

    int pnf_version_minor(void) {
        return pnf::Version::minor;
    }

    int pnf_version_patch(void) {
        return pnf::Version::patch;
    }

    PnfChartConfig pnf_chart_config_default(void) {
        PnfChartConfig cfg;
        cfg.method = PNF_METHOD_CLOSE;
        cfg.box_size_method = PNF_BOX_SIZE_TRADITIONAL;
        cfg.box_size = 0.0;
        cfg.reversal = 3;
        return cfg;
    }

    PnfIndicatorConfig pnf_indicator_config_default(void) {
        PnfIndicatorConfig cfg;
        cfg.sma_short_period = 5;
        cfg.sma_medium_period = 10;
        cfg.sma_long_period = 20;
        cfg.bollinger_period = 20;
        cfg.bollinger_std_devs = 2.0;
        cfg.rsi_period = 14;
        cfg.rsi_overbought = 70.0;
        cfg.rsi_oversold = 30.0;
        cfg.bullish_alert_threshold = 70.0;
        cfg.bearish_alert_threshold = 30.0;
        cfg.support_resistance_threshold = 0.01;
        cfg.congestion_min_columns = 4;
        cfg.congestion_price_range = 0.05;
        return cfg;
    }

    PnfChart* pnf_chart_create(const PnfChartConfig* config) {
        pnf::ChartConfig cfg;
        if (config) {
            cfg.method = static_cast<pnf::ConstructionMethod>(config->method);
            cfg.box_size_method = static_cast<pnf::BoxSizeMethod>(config->box_size_method);
            cfg.box_size = config->box_size;
            cfg.reversal = config->reversal;
        }
        return reinterpret_cast<PnfChart*>(new pnf::Chart(cfg));
    }

    PnfChart* pnf_chart_create_default(void) {
        return reinterpret_cast<PnfChart*>(new pnf::Chart());
    }

    void pnf_chart_destroy(PnfChart* chart) {
        delete reinterpret_cast<pnf::Chart*>(chart);
    }

    bool pnf_chart_add_data(PnfChart* chart, const double high, const double low, const double close, const int64_t timestamp) {
        if (!chart) return false;
        auto* c = reinterpret_cast<pnf::Chart*>(chart);
        const auto time = std::chrono::system_clock::from_time_t(timestamp);
        return c->add_data(high, low, close, time);
    }

    bool pnf_chart_add_price(PnfChart* chart, const double price, const int64_t timestamp) {
        if (!chart) return false;
        auto* c = reinterpret_cast<pnf::Chart*>(chart);
        const auto time = std::chrono::system_clock::from_time_t(timestamp);
        return c->add_data(price, time);
    }

    bool pnf_chart_add_ohlc(PnfChart* chart, const PnfOHLC* ohlc) {
        if (!ohlc) return false;
        return pnf_chart_add_data(chart, ohlc->high, ohlc->low, ohlc->close, ohlc->timestamp);
    }

    size_t pnf_chart_column_count(const PnfChart* chart) {
        if (!chart) return 0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->column_count();
    }

    size_t pnf_chart_x_column_count(const PnfChart* chart) {
        if (!chart) return 0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->x_column_count();
    }

    size_t pnf_chart_o_column_count(const PnfChart* chart) {
        if (!chart) return 0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->o_column_count();
    }

    double pnf_chart_box_size(const PnfChart* chart) {
        if (!chart) return 0.0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->current_box_size();
    }

    PnfColumnType pnf_chart_column_type(const PnfChart* chart, const size_t index) {
        if (!chart) return PNF_COLUMN_X;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(index);
        if (!col) return PNF_COLUMN_X;
        return static_cast<PnfColumnType>(col->type());
    }

    size_t pnf_chart_column_box_count(const PnfChart* chart, const size_t index) {
        if (!chart) return 0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(index);
        return col ? col->box_count() : 0;
    }

    double pnf_chart_column_highest(const PnfChart* chart, const size_t index) {
        if (!chart) return 0.0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(index);
        return col ? col->highest_price() : 0.0;
    }

    double pnf_chart_column_lowest(const PnfChart* chart, const size_t index) {
        if (!chart) return 0.0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(index);
        return col ? col->lowest_price() : 0.0;
    }

    double pnf_chart_box_price(const PnfChart* chart, const size_t col_index, const size_t box_index) {
        if (!chart) return 0.0;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(col_index);
        if (!col) return 0.0;
        const auto* box = col->get_box_at(box_index);
        return box ? box->price() : 0.0;
    }

    PnfBoxType pnf_chart_box_type(const PnfChart* chart, const size_t col_index, const size_t box_index) {
        if (!chart) return PNF_BOX_X;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(col_index);
        if (!col) return PNF_BOX_X;
        const auto* box = col->get_box_at(box_index);
        return box ? static_cast<PnfBoxType>(box->type()) : PNF_BOX_X;
    }

    const char* pnf_chart_box_marker(const PnfChart* chart, const size_t col_index, const size_t box_index) {
        if (!chart) return duplicate_string("");
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        const auto* col = c->column(col_index);
        if (!col) return duplicate_string("");
        const auto* box = col->get_box_at(box_index);
        if (!box) return duplicate_string("");
        return duplicate_string(box->marker());
    }

    bool pnf_chart_has_bullish_bias(const PnfChart* chart) {
        if (!chart) return false;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->has_bullish_bias();
    }

    bool pnf_chart_has_bearish_bias(const PnfChart* chart) {
        if (!chart) return false;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->has_bearish_bias();
    }

    bool pnf_chart_is_above_bullish_support(const PnfChart* chart, const double price) {
        if (!chart) return false;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->is_above_bullish_support(price);
    }

    bool pnf_chart_is_below_bearish_resistance(const PnfChart* chart, const double price) {
        if (!chart) return false;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return c->is_below_bearish_resistance(price);
    }

    void pnf_chart_clear(PnfChart* chart) {
        if (!chart) return;
        auto* c = reinterpret_cast<pnf::Chart*>(chart);
        c->clear();
    }

    const char* pnf_chart_to_string(const PnfChart* chart) {
        if (!chart) return duplicate_string("");
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return duplicate_string(c->to_string());
    }

    const char* pnf_chart_to_ascii(const PnfChart* chart) {
        if (!chart) return duplicate_string("");
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return duplicate_string(pnf::Visualization::to_ascii(*c));
    }

    const char* pnf_chart_to_json(const PnfChart* chart) {
        if (!chart) return duplicate_string("");
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        return duplicate_string(pnf::Visualization::to_json(*c));
    }

    PnfChartData* pnf_chart_export_data(const PnfChart* chart) {
        if (!chart) return nullptr;
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        auto [columns, prices, box_size, reversal, method] = pnf::Indicators::export_chart_data(*c);

        auto* result = new PnfChartData;
        result->column_count = columns.size();
        result->columns = new PnfColumnData[result->column_count];
        for (size_t i = 0; i < result->column_count; ++i) {
            result->columns[i].index = columns[i].index;
            result->columns[i].type = static_cast<PnfColumnType>(columns[i].type);
            result->columns[i].high = columns[i].high;
            result->columns[i].low = columns[i].low;
            result->columns[i].box_count = columns[i].box_count;
        }

        result->price_count = prices.size();
        result->prices = new double[result->price_count];
        std::memcpy(result->prices, prices.data(), result->price_count * sizeof(double));

        result->box_size = box_size;
        result->reversal = reversal;
        result->method = static_cast<PnfConstructionMethod>(method);

        return result;
    }

    void pnf_chart_data_destroy(const PnfChartData* data) {
        if (data) {
            delete[] data->columns;
            delete[] data->prices;
            delete data;
        }
    }

    PnfIndicators* pnf_indicators_create(void) {
        return reinterpret_cast<PnfIndicators*>(new pnf::Indicators());
    }

    PnfIndicators* pnf_indicators_create_default(void) {
        return pnf_indicators_create();
    }

    PnfIndicators* pnf_indicators_create_with_config(const PnfIndicatorConfig* config) {
        const auto cfg = to_cpp_config(config);
        return reinterpret_cast<PnfIndicators*>(new pnf::Indicators(cfg));
    }

    void pnf_indicators_destroy(PnfIndicators* indicators) {
        delete reinterpret_cast<pnf::Indicators*>(indicators);
    }

    void pnf_indicators_configure(PnfIndicators* ind, const PnfIndicatorConfig* config) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->configure(to_cpp_config(config));
    }

    void pnf_indicators_set_sma_periods(PnfIndicators* ind, const int short_period, const int medium_period, const int long_period) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->sma_short()->set_period(short_period);
        i->sma_medium()->set_period(medium_period);
        i->sma_long()->set_period(long_period);
    }

    void pnf_indicators_set_bollinger_params(PnfIndicators* ind, const int period, const double std_devs) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->bollinger()->set_period(period);
        i->bollinger()->set_std_devs(std_devs);
    }

    void pnf_indicators_set_rsi_params(PnfIndicators* ind, const int period, const double overbought, const double oversold) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->rsi()->set_period(period);
        i->rsi()->set_thresholds(overbought, oversold);
    }

    void pnf_indicators_set_bullish_percent_thresholds(PnfIndicators* ind, const double bullish, const double bearish) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->bullish_percent()->set_thresholds(bullish, bearish);
    }

    void pnf_indicators_set_support_resistance_threshold(PnfIndicators* ind, const double threshold) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->support_resistance()->set_threshold(threshold);
    }

    void pnf_indicators_set_congestion_params(PnfIndicators* ind, const int min_columns, const double price_range) {
        if (!ind) return;
        auto* i = reinterpret_cast<pnf::Indicators*>(ind);
        i->congestion()->set_min_columns(min_columns);
        i->congestion()->set_threshold(price_range);
    }

    void pnf_indicators_calculate(PnfIndicators* indicators, const PnfChart* chart) {
        if (!indicators || !chart) return;
        const auto* ind = reinterpret_cast<pnf::Indicators*>(indicators);
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        ind->calculate(*c);
    }

    double pnf_indicators_sma_short(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->sma_short()->value(column);
    }

    double pnf_indicators_sma_medium(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->sma_medium()->value(column);
    }

    double pnf_indicators_sma_long(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->sma_long()->value(column);
    }

    PnfDoubleArray pnf_indicators_sma_short_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->sma_short()->values());
    }

    PnfDoubleArray pnf_indicators_sma_medium_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->sma_medium()->values());
    }

    PnfDoubleArray pnf_indicators_sma_long_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->sma_long()->values());
    }

    double pnf_indicators_bollinger_middle(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bollinger()->middle(column);
    }

    double pnf_indicators_bollinger_upper(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bollinger()->upper(column);
    }

    double pnf_indicators_bollinger_lower(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bollinger()->lower(column);
    }

    PnfDoubleArray pnf_indicators_bollinger_middle_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->bollinger()->middle_band());
    }

    PnfDoubleArray pnf_indicators_bollinger_upper_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->bollinger()->upper_band());
    }

    PnfDoubleArray pnf_indicators_bollinger_lower_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->bollinger()->lower_band());
    }

    double pnf_indicators_rsi(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->rsi()->value(column);
    }

    bool pnf_indicators_rsi_is_overbought(const PnfIndicators* ind, const int column) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->rsi()->is_overbought(column);
    }

    bool pnf_indicators_rsi_is_oversold(const PnfIndicators* ind, const int column) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->rsi()->is_oversold(column);
    }

    PnfDoubleArray pnf_indicators_rsi_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->rsi()->values());
    }

    double pnf_indicators_obv(const PnfIndicators* ind, const int column) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->obv()->value(column);
    }

    PnfDoubleArray pnf_indicators_obv_values(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->obv()->values());
    }

    double pnf_indicators_bullish_percent(const PnfIndicators* ind) {
        if (!ind) return 0.0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bullish_percent()->value();
    }

    bool pnf_indicators_is_bullish_alert(const PnfIndicators* ind) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bullish_percent()->is_bullish_alert();
    }

    bool pnf_indicators_is_bearish_alert(const PnfIndicators* ind) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->bullish_percent()->is_bearish_alert();
    }

    PnfSignalType pnf_indicators_current_signal(const PnfIndicators* ind) {
        if (!ind) return PNF_SIGNAL_NONE;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return static_cast<PnfSignalType>(i->signals()->current_signal());
    }

    size_t pnf_indicators_signal_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->signals()->signals().size();
    }

    PnfSignal pnf_indicators_signal_at(const PnfIndicators* ind, const size_t index) {
        PnfSignal result = {PNF_SIGNAL_NONE, -1, 0.0};
        if (!ind) return result;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        if (const auto& signals = i->signals()->signals(); index < signals.size()) {
            result.type = static_cast<PnfSignalType>(signals[index].type);
            result.column_index = signals[index].column_index;
            result.price = signals[index].price;
        }
        return result;
    }

    PnfSignalArray pnf_indicators_signals(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        const auto& signals = i->signals()->signals();
        PnfSignalArray arr;
        arr.length = signals.size();
        if (arr.length > 0) {
            arr.data = new PnfSignal[arr.length];
            for (size_t j = 0; j < arr.length; ++j) {
                arr.data[j].type = static_cast<PnfSignalType>(signals[j].type);
                arr.data[j].column_index = signals[j].column_index;
                arr.data[j].price = signals[j].price;
            }
        } else {
            arr.data = nullptr;
        }
        return arr;
    }

    int pnf_indicators_buy_signal_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->signals()->buy_count();
    }

    int pnf_indicators_sell_signal_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->signals()->sell_count();
    }

    size_t pnf_indicators_pattern_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->patterns()->patterns().size();
    }

    size_t pnf_indicators_bullish_pattern_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->patterns()->bullish_patterns().size();
    }

    size_t pnf_indicators_bearish_pattern_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->patterns()->bearish_patterns().size();
    }

    static PnfPattern to_c_pattern(const pnf::Pattern& p) {
        PnfPattern result;
        result.type = static_cast<PnfPatternType>(p.type);
        result.start_column = p.start_column;
        result.end_column = p.end_column;
        result.price = p.price;
        result.is_bullish = pnf::is_bullish_pattern(p.type);
        return result;
    }

    PnfPattern pnf_indicators_pattern_at(const PnfIndicators* ind, const size_t index) {
        if (!ind) return {PNF_PATTERN_NONE, -1, -1, 0.0, false};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        if (const auto& patterns = i->patterns()->patterns(); index < patterns.size()) {
            return to_c_pattern(patterns[index]);
        }
        return {PNF_PATTERN_NONE, -1, -1, 0.0, false};
    }

    static PnfPatternArray make_pattern_array(const std::vector<pnf::Pattern>& patterns) {
        PnfPatternArray arr;
        arr.length = patterns.size();
        if (arr.length > 0) {
            arr.data = new PnfPattern[arr.length];
            for (size_t j = 0; j < arr.length; ++j) {
                arr.data[j] = to_c_pattern(patterns[j]);
            }
        } else {
            arr.data = nullptr;
        }
        return arr;
    }

    PnfPatternArray pnf_indicators_patterns(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_pattern_array(i->patterns()->patterns());
    }

    PnfPatternArray pnf_indicators_bullish_patterns(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_pattern_array(i->patterns()->bullish_patterns());
    }

    PnfPatternArray pnf_indicators_bearish_patterns(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_pattern_array(i->patterns()->bearish_patterns());
    }

    size_t pnf_indicators_support_level_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->support_resistance()->support_levels().size();
    }

    size_t pnf_indicators_resistance_level_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->support_resistance()->resistance_levels().size();
    }

    bool pnf_indicators_is_near_support(const PnfIndicators* ind, const double price, const double tolerance) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->support_resistance()->is_near_support(price, tolerance);
    }

    bool pnf_indicators_is_near_resistance(const PnfIndicators* ind, const double price, const double tolerance) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->support_resistance()->is_near_resistance(price, tolerance);
    }

    static PnfLevelArray make_level_array(const std::vector<pnf::SupportResistanceLevel>& levels, const bool is_support) {
        PnfLevelArray arr;
        arr.length = levels.size();
        if (arr.length > 0) {
            arr.data = new PnfSupportResistanceLevel[arr.length];
            for (size_t j = 0; j < arr.length; ++j) {
                arr.data[j].price = levels[j].price;
                arr.data[j].touch_count = levels[j].touch_count;
                arr.data[j].is_support = is_support;
            }
        } else {
            arr.data = nullptr;
        }
        return arr;
    }

    PnfLevelArray pnf_indicators_support_levels(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_level_array(i->support_resistance()->support_levels(), true);
    }

    PnfLevelArray pnf_indicators_resistance_levels(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_level_array(i->support_resistance()->resistance_levels(), false);
    }

    PnfDoubleArray pnf_indicators_support_prices(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->support_resistance()->support_prices());
    }

    PnfDoubleArray pnf_indicators_resistance_prices(const PnfIndicators* ind) {
        if (!ind) return {nullptr, 0};
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return make_double_array(i->support_resistance()->resistance_prices());
    }

    bool pnf_indicators_is_in_congestion(const PnfIndicators* ind, int column) {
        if (!ind) return false;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->congestion()->is_in_congestion(column);
    }

    size_t pnf_indicators_congestion_zone_count(const PnfIndicators* ind) {
        if (!ind) return 0;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return i->congestion()->zones().size();
    }

    PnfIndicatorData* pnf_indicators_export_data(const PnfIndicators* ind) {
        if (!ind) return nullptr;
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        auto data = i->export_data();

        auto* result = new PnfIndicatorData;
        result->sma_short = make_double_array(data.sma_short);
        result->sma_medium = make_double_array(data.sma_medium);
        result->sma_long = make_double_array(data.sma_long);
        result->bollinger_middle = make_double_array(data.bollinger_middle);
        result->bollinger_upper = make_double_array(data.bollinger_upper);
        result->bollinger_lower = make_double_array(data.bollinger_lower);
        result->rsi = make_double_array(data.rsi);
        result->obv = make_double_array(data.obv);
        result->bullish_percent = data.bullish_percent;

        result->signals.length = data.signals.size();
        if (result->signals.length > 0) {
            result->signals.data = new PnfSignal[result->signals.length];
            for (size_t j = 0; j < result->signals.length; ++j) {
                result->signals.data[j].type = static_cast<PnfSignalType>(data.signals[j].type);
                result->signals.data[j].column_index = data.signals[j].column_index;
                result->signals.data[j].price = data.signals[j].price;
            }
        } else {
            result->signals.data = nullptr;
        }

        result->patterns = make_pattern_array(data.patterns);
        result->support_levels = make_level_array(data.support_levels, true);
        result->resistance_levels = make_level_array(data.resistance_levels, false);

        return result;
    }

    void pnf_indicator_data_destroy(const PnfIndicatorData* data) {
        if (data) {
            delete[] data->sma_short.data;
            delete[] data->sma_medium.data;
            delete[] data->sma_long.data;
            delete[] data->bollinger_middle.data;
            delete[] data->bollinger_upper.data;
            delete[] data->bollinger_lower.data;
            delete[] data->rsi.data;
            delete[] data->obv.data;
            delete[] data->signals.data;
            delete[] data->patterns.data;
            delete[] data->support_levels.data;
            delete[] data->resistance_levels.data;
            delete data;
        }
    }

    const char* pnf_indicators_summary(const PnfIndicators* ind) {
        if (!ind) return duplicate_string("");
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return duplicate_string(i->summary());
    }

    const char* pnf_indicators_to_string(const PnfIndicators* ind) {
        if (!ind) return duplicate_string("");
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        return duplicate_string(i->to_string());
    }

    void pnf_free_string(const char* str) {
        delete[] str;
    }

    void pnf_free_double_array(const PnfDoubleArray arr) {
        delete[] arr.data;
    }

    void pnf_free_signal_array(const PnfSignalArray arr) {
        delete[] arr.data;
    }

    void pnf_free_pattern_array(const PnfPatternArray arr) {
        delete[] arr.data;
    }

    void pnf_free_level_array(const PnfLevelArray arr) {
        delete[] arr.data;
    }

    // Interactive Viewer
    PnfViewer* pnf_viewer_create(const char* title, const int width, const int height) {
        pnf::ViewerConfig config;
        if (title) config.window_title = title;
        if (width > 0) config.window_width = width;
        if (height > 0) config.window_height = height;
        return reinterpret_cast<PnfViewer*>(new pnf::Viewer(config));
    }

    void pnf_viewer_destroy(PnfViewer* viewer) {
        delete reinterpret_cast<pnf::Viewer*>(viewer);
    }

    void pnf_viewer_set_chart(PnfViewer* viewer, const PnfChart* chart) {
        if (!viewer) return;
        auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        auto* c = reinterpret_cast<const pnf::Chart*>(chart);
        v->set_chart(c);
    }

    void pnf_viewer_set_indicators(PnfViewer* viewer, const PnfIndicators* ind) {
        if (!viewer) return;
        auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        auto* i = reinterpret_cast<const pnf::Indicators*>(ind);
        v->set_indicators(i);
    }

    void pnf_viewer_update(PnfViewer* viewer) {
        if (!viewer) return;
        const auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        v->update();
    }

    bool pnf_viewer_is_open(const PnfViewer* viewer) {
        if (!viewer) return false;
        auto* v = reinterpret_cast<const pnf::Viewer*>(viewer);
        return v->is_open();
    }

    void pnf_viewer_show(PnfViewer* viewer) {
        if (!viewer) return;
        auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        v->show();
    }

    void pnf_viewer_close(PnfViewer* viewer) {
        if (!viewer) return;
        auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        v->close();
    }

    void pnf_viewer_poll_events(PnfViewer* viewer) {
        if (!viewer) return;
        const auto* v = reinterpret_cast<pnf::Viewer*>(viewer);
        v->update();
    }

}
