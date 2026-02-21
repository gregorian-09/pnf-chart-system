/// \file pnf_c.hpp
/// \brief Public API.

//
// Created by gregorian-rayne on 15/01/2026.
//

#ifndef PNF_C_HPP
#define PNF_C_HPP

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
    #endif

    #ifdef _WIN32
        #ifdef PNF_BUILD_DLL
            #define PNF_API __declspec(dllexport)
        #else
            #define PNF_API __declspec(dllimport)
        #endif
    #else
        #define PNF_API __attribute__((visibility("default")))
    #endif

/// \brief Opaque chart handle.
    typedef struct PnfChart PnfChart;
/// \brief Opaque indicators handle.
    typedef struct PnfIndicators PnfIndicators;
/// \brief Opaque viewer handle.
    typedef struct PnfViewer PnfViewer;

/// \brief Box type enumeration.
    typedef enum {
        PNF_BOX_X = 0,
        PNF_BOX_O = 1
    } PnfBoxType;

/// \brief Column type enumeration.
    typedef enum {
        PNF_COLUMN_X = 0,
        PNF_COLUMN_O = 1,
        PNF_COLUMN_MIXED = 2
    } PnfColumnType;

/// \brief Construction method enumeration.
    typedef enum {
        PNF_METHOD_CLOSE = 0,
        PNF_METHOD_HIGH_LOW = 1
    } PnfConstructionMethod;

/// \brief Box size method enumeration.
    typedef enum {
        PNF_BOX_SIZE_FIXED = 0,
        PNF_BOX_SIZE_TRADITIONAL = 1,
        PNF_BOX_SIZE_PERCENTAGE = 2,
        PNF_BOX_SIZE_POINTS = 3
    } PnfBoxSizeMethod;

/// \brief Signal type enumeration.
    typedef enum {
        PNF_SIGNAL_NONE = 0,
        PNF_SIGNAL_BUY = 1,
        PNF_SIGNAL_SELL = 2
    } PnfSignalType;

/// \brief Pattern type enumeration.
    typedef enum {
        PNF_PATTERN_NONE = 0,
        PNF_PATTERN_DOUBLE_TOP_BREAKOUT,
        PNF_PATTERN_DOUBLE_BOTTOM_BREAKDOWN,
        PNF_PATTERN_TRIPLE_TOP_BREAKOUT,
        PNF_PATTERN_TRIPLE_BOTTOM_BREAKDOWN,
        PNF_PATTERN_QUADRUPLE_TOP_BREAKOUT,
        PNF_PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN,
        PNF_PATTERN_ASCENDING_TRIPLE_TOP,
        PNF_PATTERN_DESCENDING_TRIPLE_BOTTOM,
        PNF_PATTERN_BULLISH_CATAPULT,
        PNF_PATTERN_BEARISH_CATAPULT,
        PNF_PATTERN_BULLISH_SIGNAL_REVERSED,
        PNF_PATTERN_BEARISH_SIGNAL_REVERSED,
        PNF_PATTERN_BULLISH_TRIANGLE,
        PNF_PATTERN_BEARISH_TRIANGLE,
        PNF_PATTERN_LONG_TAIL_DOWN,
        PNF_PATTERN_HIGH_POLE,
        PNF_PATTERN_LOW_POLE,
        PNF_PATTERN_BULL_TRAP,
        PNF_PATTERN_BEAR_TRAP,
        PNF_PATTERN_SPREAD_TRIPLE_TOP,
        PNF_PATTERN_SPREAD_TRIPLE_BOTTOM
    } PnfPatternType;

/// \brief Chart configuration parameters.
    typedef struct {
        PnfConstructionMethod method;
        PnfBoxSizeMethod box_size_method;
        double box_size;
        int reversal;
    } PnfChartConfig;

/// \brief Indicator configuration parameters.
    typedef struct {
        int sma_short_period;
        int sma_medium_period;
        int sma_long_period;
        int bollinger_period;
        double bollinger_std_devs;
        int rsi_period;
        double rsi_overbought;
        double rsi_oversold;
        double bullish_alert_threshold;
        double bearish_alert_threshold;
        double support_resistance_threshold;
        int congestion_min_columns;
        double congestion_price_range;
    } PnfIndicatorConfig;

/// \brief PnfOHLC.
    typedef struct {
        int64_t timestamp;
        double open;
        double high;
        double low;
        double close;
        double volume;
    } PnfOHLC;

/// \brief PnfSignal.
    typedef struct {
        PnfSignalType type;
        int column_index;
        double price;
    } PnfSignal;

/// \brief PnfPattern.
    typedef struct {
        PnfPatternType type;
        int start_column;
        int end_column;
        double price;
        bool is_bullish;
    } PnfPattern;

/// \brief PnfSupportResistanceLevel.
    typedef struct {
        double price;
        int touch_count;
        bool is_support;
    } PnfSupportResistanceLevel;

/// \brief PnfColumnData.
    typedef struct {
        int index;
        PnfColumnType type;
        double high;
        double low;
        int box_count;
    } PnfColumnData;

/// \brief PnfDoubleArray.
    typedef struct {
        double* data;
        size_t length;
    } PnfDoubleArray;

/// \brief PnfSignalArray.
    typedef struct {
        PnfSignal* data;
        size_t length;
    } PnfSignalArray;

/// \brief PnfPatternArray.
    typedef struct {
        PnfPattern* data;
        size_t length;
    } PnfPatternArray;

/// \brief PnfLevelArray.
    typedef struct {
        PnfSupportResistanceLevel* data;
        size_t length;
    } PnfLevelArray;

/// \brief PnfChartData.
    typedef struct {
        PnfColumnData* columns;
        size_t column_count;
        double* prices;
        size_t price_count;
        double box_size;
        int reversal;
        PnfConstructionMethod method;
    } PnfChartData;

/// \brief Aggregated indicator data arrays and summary values.
    typedef struct {
        PnfDoubleArray sma_short;
        PnfDoubleArray sma_medium;
        PnfDoubleArray sma_long;
        PnfDoubleArray bollinger_middle;
        PnfDoubleArray bollinger_upper;
        PnfDoubleArray bollinger_lower;
        PnfDoubleArray rsi;
        PnfDoubleArray obv;
        double bullish_percent;
        PnfSignalArray signals;
        PnfPatternArray patterns;
        PnfLevelArray support_levels;
        PnfLevelArray resistance_levels;
    } PnfIndicatorData;

    // Version
/// \brief Version string.
    /// \return 
    PNF_API const char* pnf_version_string(void);
/// \brief Version major.
    /// \return 
    PNF_API int pnf_version_major(void);
/// \brief Version minor.
    /// \return 
    PNF_API int pnf_version_minor(void);
/// \brief Version patch.
    /// \return 
    PNF_API int pnf_version_patch(void);

    // Chart config helpers
/// \brief Chart config default.
    /// \return 
    PNF_API PnfChartConfig pnf_chart_config_default(void);
/// \brief Indicator config default.
    /// \return 
    PNF_API PnfIndicatorConfig pnf_indicator_config_default(void);

    // Chart creation/destruction
/// \brief Chart create.
    /// \param config 
    /// \return 
    PNF_API PnfChart* pnf_chart_create(const PnfChartConfig* config);
/// \brief Chart create default.
    /// \return 
    PNF_API PnfChart* pnf_chart_create_default(void);
/// \brief Chart destroy.
    /// \param chart 
    PNF_API void pnf_chart_destroy(PnfChart* chart);

    // Data input
/// \brief Chart add data.
    /// \param chart 
    /// \param high 
    /// \param low 
    /// \param close 
    /// \param timestamp 
    /// \return 
    PNF_API bool pnf_chart_add_data(PnfChart* chart, double high, double low, double close, int64_t timestamp);
/// \brief Chart add price.
    /// \param chart 
    /// \param price 
    /// \param timestamp 
    /// \return 
    PNF_API bool pnf_chart_add_price(PnfChart* chart, double price, int64_t timestamp);
/// \brief Chart add ohlc.
    /// \param chart 
    /// \param ohlc 
    /// \return 
    PNF_API bool pnf_chart_add_ohlc(PnfChart* chart, const PnfOHLC* ohlc);

    // Chart info
/// \brief Chart column count.
    /// \param chart 
    /// \return 
    PNF_API size_t pnf_chart_column_count(const PnfChart* chart);
/// \brief Chart x column count.
    /// \param chart 
    /// \return 
    PNF_API size_t pnf_chart_x_column_count(const PnfChart* chart);
/// \brief Chart o column count.
    /// \param chart 
    /// \return 
    PNF_API size_t pnf_chart_o_column_count(const PnfChart* chart);
/// \brief Chart box size.
    /// \param chart 
    /// \return 
    PNF_API double pnf_chart_box_size(const PnfChart* chart);

    // Column info
/// \brief Chart column type.
    /// \param chart 
    /// \param index 
    /// \return 
    PNF_API PnfColumnType pnf_chart_column_type(const PnfChart* chart, size_t index);
/// \brief Chart column box count.
    /// \param chart 
    /// \param index 
    /// \return 
    PNF_API size_t pnf_chart_column_box_count(const PnfChart* chart, size_t index);
/// \brief Chart column highest.
    /// \param chart 
    /// \param index 
    /// \return 
    PNF_API double pnf_chart_column_highest(const PnfChart* chart, size_t index);
/// \brief Chart column lowest.
    /// \param chart 
    /// \param index 
    /// \return 
    PNF_API double pnf_chart_column_lowest(const PnfChart* chart, size_t index);

    // Box info
/// \brief Chart box price.
    /// \param chart 
    /// \param col_index 
    /// \param box_index 
    /// \return 
    PNF_API double pnf_chart_box_price(const PnfChart* chart, size_t col_index, size_t box_index);
/// \brief Chart box type.
    /// \param chart 
    /// \param col_index 
    /// \param box_index 
    /// \return 
    PNF_API PnfBoxType pnf_chart_box_type(const PnfChart* chart, size_t col_index, size_t box_index);
/// \brief Chart box marker.
    /// \param chart 
    /// \param col_index 
    /// \param box_index 
    /// \return 
    PNF_API const char* pnf_chart_box_marker(const PnfChart* chart, size_t col_index, size_t box_index);

    // Trend analysis
/// \brief Chart has bullish bias.
    /// \param chart 
    /// \return 
    PNF_API bool pnf_chart_has_bullish_bias(const PnfChart* chart);
/// \brief Chart has bearish bias.
    /// \param chart 
    /// \return 
    PNF_API bool pnf_chart_has_bearish_bias(const PnfChart* chart);
/// \brief Chart is above bullish support.
    /// \param chart 
    /// \param price 
    /// \return 
    PNF_API bool pnf_chart_is_above_bullish_support(const PnfChart* chart, double price);
/// \brief Chart is below bearish resistance.
    /// \param chart 
    /// \param price 
    /// \return 
    PNF_API bool pnf_chart_is_below_bearish_resistance(const PnfChart* chart, double price);

    // Clear and utility
/// \brief Chart clear.
    /// \param chart 
    PNF_API void pnf_chart_clear(PnfChart* chart);
/// \brief Chart to string.
    /// \param chart 
    /// \return 
    PNF_API const char* pnf_chart_to_string(const PnfChart* chart);
/// \brief Chart to ascii.
    /// \param chart 
    /// \return 
    PNF_API const char* pnf_chart_to_ascii(const PnfChart* chart);
/// \brief Chart to json.
    /// \param chart 
    /// \return 
    PNF_API const char* pnf_chart_to_json(const PnfChart* chart);

    // Chart data export
/// \brief Chart export data.
    /// \param chart 
    /// \return 
    PNF_API PnfChartData* pnf_chart_export_data(const PnfChart* chart);
/// \brief Chart data destroy.
    /// \param data 
    PNF_API void pnf_chart_data_destroy(const PnfChartData* data);

    // Indicators creation/destruction
/// \brief Indicators create.
    /// \return 
    PNF_API PnfIndicators* pnf_indicators_create(void);
/// \brief Indicators create default.
    /// \return 
    PNF_API PnfIndicators* pnf_indicators_create_default(void);
/// \brief Indicators create with config.
    /// \param config 
    /// \return 
    PNF_API PnfIndicators* pnf_indicators_create_with_config(const PnfIndicatorConfig* config);
/// \brief Indicators destroy.
    /// \param indicators 
    PNF_API void pnf_indicators_destroy(PnfIndicators* indicators);

    // Indicator configuration
/// \brief Indicators configure.
    /// \param ind 
    /// \param config 
    PNF_API void pnf_indicators_configure(PnfIndicators* ind, const PnfIndicatorConfig* config);
/// \brief Indicators set sma periods.
    /// \param ind 
    /// \param short_period 
    /// \param medium_period 
    /// \param long_period 
    PNF_API void pnf_indicators_set_sma_periods(PnfIndicators* ind, int short_period, int medium_period, int long_period);
/// \brief Indicators set bollinger params.
    /// \param ind 
    /// \param period 
    /// \param std_devs 
    PNF_API void pnf_indicators_set_bollinger_params(PnfIndicators* ind, int period, double std_devs);
/// \brief Indicators set rsi params.
    /// \param ind 
    /// \param period 
    /// \param overbought 
    /// \param oversold 
    PNF_API void pnf_indicators_set_rsi_params(PnfIndicators* ind, int period, double overbought, double oversold);
/// \brief Indicators set bullish percent thresholds.
    /// \param ind 
    /// \param bullish 
    /// \param bearish 
    PNF_API void pnf_indicators_set_bullish_percent_thresholds(PnfIndicators* ind, double bullish, double bearish);
/// \brief Indicators set support resistance threshold.
    /// \param ind 
    /// \param threshold 
    PNF_API void pnf_indicators_set_support_resistance_threshold(PnfIndicators* ind, double threshold);
/// \brief Indicators set congestion params.
    /// \param ind 
    /// \param min_columns 
    /// \param price_range 
    PNF_API void pnf_indicators_set_congestion_params(PnfIndicators* ind, int min_columns, double price_range);

    // Indicator calculation
/// \brief Indicators calculate.
    /// \param indicators 
    /// \param chart 
    PNF_API void pnf_indicators_calculate(PnfIndicators* indicators, const PnfChart* chart);

    // SMA values
/// \brief Indicators sma short.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_sma_short(const PnfIndicators* ind, int column);
/// \brief Indicators sma medium.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_sma_medium(const PnfIndicators* ind, int column);
/// \brief Indicators sma long.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_sma_long(const PnfIndicators* ind, int column);
/// \brief Indicators sma short values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_sma_short_values(const PnfIndicators* ind);
/// \brief Indicators sma medium values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_sma_medium_values(const PnfIndicators* ind);
/// \brief Indicators sma long values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_sma_long_values(const PnfIndicators* ind);

    // Bollinger values
/// \brief Indicators bollinger middle.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_bollinger_middle(const PnfIndicators* ind, int column);
/// \brief Indicators bollinger upper.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_bollinger_upper(const PnfIndicators* ind, int column);
/// \brief Indicators bollinger lower.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_bollinger_lower(const PnfIndicators* ind, int column);
/// \brief Indicators bollinger middle values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_bollinger_middle_values(const PnfIndicators* ind);
/// \brief Indicators bollinger upper values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_bollinger_upper_values(const PnfIndicators* ind);
/// \brief Indicators bollinger lower values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_bollinger_lower_values(const PnfIndicators* ind);

    // RSI values
/// \brief Indicators rsi.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_rsi(const PnfIndicators* ind, int column);
/// \brief Indicators rsi is overbought.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API bool pnf_indicators_rsi_is_overbought(const PnfIndicators* ind, int column);
/// \brief Indicators rsi is oversold.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API bool pnf_indicators_rsi_is_oversold(const PnfIndicators* ind, int column);
/// \brief Indicators rsi values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_rsi_values(const PnfIndicators* ind);

    // OBV values
/// \brief Indicators obv.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API double pnf_indicators_obv(const PnfIndicators* ind, int column);
/// \brief Indicators obv values.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_obv_values(const PnfIndicators* ind);

    // Bullish percent
/// \brief Indicators bullish percent.
    /// \param ind 
    /// \return 
    PNF_API double pnf_indicators_bullish_percent(const PnfIndicators* ind);
/// \brief Indicators is bullish alert.
    /// \param ind 
    /// \return 
    PNF_API bool pnf_indicators_is_bullish_alert(const PnfIndicators* ind);
/// \brief Indicators is bearish alert.
    /// \param ind 
    /// \return 
    PNF_API bool pnf_indicators_is_bearish_alert(const PnfIndicators* ind);

    // Signal detection
/// \brief Indicators current signal.
    /// \param ind 
    /// \return 
    PNF_API PnfSignalType pnf_indicators_current_signal(const PnfIndicators* ind);
/// \brief Indicators signal count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_signal_count(const PnfIndicators* ind);
/// \brief Indicators signal at.
    /// \param ind 
    /// \param index 
    /// \return 
    PNF_API PnfSignal pnf_indicators_signal_at(const PnfIndicators* ind, size_t index);
/// \brief Indicators signals.
    /// \param ind 
    /// \return 
    PNF_API PnfSignalArray pnf_indicators_signals(const PnfIndicators* ind);
/// \brief Indicators buy signal count.
    /// \param ind 
    /// \return 
    PNF_API int pnf_indicators_buy_signal_count(const PnfIndicators* ind);
/// \brief Indicators sell signal count.
    /// \param ind 
    /// \return 
    PNF_API int pnf_indicators_sell_signal_count(const PnfIndicators* ind);

    // Pattern detection
/// \brief Indicators pattern count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_pattern_count(const PnfIndicators* ind);
/// \brief Indicators bullish pattern count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_bullish_pattern_count(const PnfIndicators* ind);
/// \brief Indicators bearish pattern count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_bearish_pattern_count(const PnfIndicators* ind);
/// \brief Indicators pattern at.
    /// \param ind 
    /// \param index 
    /// \return 
    PNF_API PnfPattern pnf_indicators_pattern_at(const PnfIndicators* ind, size_t index);
/// \brief Indicators patterns.
    /// \param ind 
    /// \return 
    PNF_API PnfPatternArray pnf_indicators_patterns(const PnfIndicators* ind);
/// \brief Indicators bullish patterns.
    /// \param ind 
    /// \return 
    PNF_API PnfPatternArray pnf_indicators_bullish_patterns(const PnfIndicators* ind);
/// \brief Indicators bearish patterns.
    /// \param ind 
    /// \return 
    PNF_API PnfPatternArray pnf_indicators_bearish_patterns(const PnfIndicators* ind);

    // Support/Resistance
/// \brief Indicators support level count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_support_level_count(const PnfIndicators* ind);
/// \brief Indicators resistance level count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_resistance_level_count(const PnfIndicators* ind);
/// \brief Indicators is near support.
    /// \param ind 
    /// \param price 
    /// \param tolerance 
    /// \return 
    PNF_API bool pnf_indicators_is_near_support(const PnfIndicators* ind, double price, double tolerance);
/// \brief Indicators is near resistance.
    /// \param ind 
    /// \param price 
    /// \param tolerance 
    /// \return 
    PNF_API bool pnf_indicators_is_near_resistance(const PnfIndicators* ind, double price, double tolerance);
/// \brief Indicators support levels.
    /// \param ind 
    /// \return 
    PNF_API PnfLevelArray pnf_indicators_support_levels(const PnfIndicators* ind);
/// \brief Indicators resistance levels.
    /// \param ind 
    /// \return 
    PNF_API PnfLevelArray pnf_indicators_resistance_levels(const PnfIndicators* ind);
/// \brief Indicators support prices.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_support_prices(const PnfIndicators* ind);
/// \brief Indicators resistance prices.
    /// \param ind 
    /// \return 
    PNF_API PnfDoubleArray pnf_indicators_resistance_prices(const PnfIndicators* ind);

    // Congestion detection
/// \brief Indicators is in congestion.
    /// \param ind 
    /// \param column 
    /// \return 
    PNF_API bool pnf_indicators_is_in_congestion(const PnfIndicators* ind, int column);
/// \brief Indicators congestion zone count.
    /// \param ind 
    /// \return 
    PNF_API size_t pnf_indicators_congestion_zone_count(const PnfIndicators* ind);

    // Indicator data export
/// \brief Indicators export data.
    /// \param ind 
    /// \return 
    PNF_API PnfIndicatorData* pnf_indicators_export_data(const PnfIndicators* ind);
/// \brief Indicator data destroy.
    /// \param data 
    PNF_API void pnf_indicator_data_destroy(const PnfIndicatorData* data);

    // String output
/// \brief Indicators summary.
    /// \param ind 
    /// \return 
    PNF_API const char* pnf_indicators_summary(const PnfIndicators* ind);
/// \brief Indicators to string.
    /// \param ind 
    /// \return 
    PNF_API const char* pnf_indicators_to_string(const PnfIndicators* ind);

    // Memory management
/// \brief Free string.
    /// \param str 
    PNF_API void pnf_free_string(const char* str);
/// \brief Free double array.
    /// \param arr 
    PNF_API void pnf_free_double_array(PnfDoubleArray arr);
/// \brief Free signal array.
    /// \param arr 
    PNF_API void pnf_free_signal_array(PnfSignalArray arr);
/// \brief Free pattern array.
    /// \param arr 
    PNF_API void pnf_free_pattern_array(PnfPatternArray arr);
/// \brief Free level array.
    /// \param arr 
    PNF_API void pnf_free_level_array(PnfLevelArray arr);

    // Interactive viewer
/// \brief Viewer create.
    /// \param title 
    /// \param width 
    /// \param height 
    /// \return 
    PNF_API PnfViewer* pnf_viewer_create(const char* title, int width, int height);
/// \brief Viewer destroy.
    /// \param viewer 
    PNF_API void pnf_viewer_destroy(PnfViewer* viewer);
/// \brief Viewer set chart.
    /// \param viewer 
    /// \param chart 
    PNF_API void pnf_viewer_set_chart(PnfViewer* viewer, const PnfChart* chart);
/// \brief Viewer set indicators.
    /// \param viewer 
    /// \param ind 
    PNF_API void pnf_viewer_set_indicators(PnfViewer* viewer, const PnfIndicators* ind);
/// \brief Viewer update.
    /// \param viewer 
    PNF_API void pnf_viewer_update(PnfViewer* viewer);
/// \brief Viewer is open.
    /// \param viewer 
    /// \return 
    PNF_API bool pnf_viewer_is_open(const PnfViewer* viewer);
/// \brief Viewer show.
    /// \param viewer 
    PNF_API void pnf_viewer_show(PnfViewer* viewer);
/// \brief Viewer close.
    /// \param viewer 
    PNF_API void pnf_viewer_close(PnfViewer* viewer);
/// \brief Viewer poll events.
    /// \param viewer 
    PNF_API void pnf_viewer_poll_events(PnfViewer* viewer);

    // Deprecated aliases for backwards compatibility
    #define pnf_indicators_sma5 pnf_indicators_sma_short
    #define pnf_indicators_sma10 pnf_indicators_sma_medium
    #define pnf_indicators_sma20 pnf_indicators_sma_long

    #ifdef __cplusplus
}
#endif

#endif //PNF_C_H
