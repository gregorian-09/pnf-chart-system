using System;
using System.Runtime.InteropServices;

namespace PnF
{
    internal static class NativeMethods
    {
        private const string DllName = "pnf_native";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_version_string.
        /// </summary>
        /// <returns></returns>
        public static extern IntPtr pnf_version_string();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_version_major.
        /// </summary>
        /// <returns></returns>
        public static extern int pnf_version_major();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_version_minor.
        /// </summary>
        /// <returns></returns>
        public static extern int pnf_version_minor();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_version_patch.
        /// </summary>
        /// <returns></returns>
        public static extern int pnf_version_patch();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_config_default.
        /// </summary>
        /// <returns></returns>
        public static extern ChartConfig pnf_chart_config_default();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicator_config_default.
        /// </summary>
        /// <returns></returns>
        public static extern IndicatorConfig pnf_indicator_config_default();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_create.
        /// </summary>
        /// <param name="config"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_create(ref ChartConfig config);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_create_default.
        /// </summary>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_create_default();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_destroy.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern void pnf_chart_destroy(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_add_data.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="high"></param>
        /// <param name="low"></param>
        /// <param name="close"></param>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_add_data(IntPtr chart, double high, double low, double close, long timestamp);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_add_price.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="price"></param>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_add_price(IntPtr chart, double price, long timestamp);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_add_ohlc.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="ohlc"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_add_ohlc(IntPtr chart, ref OHLC ohlc);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_column_count.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_chart_column_count(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_x_column_count.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_chart_x_column_count(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_o_column_count.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_chart_o_column_count(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_box_size.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern double pnf_chart_box_size(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_column_type.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern ColumnType pnf_chart_column_type(IntPtr chart, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_column_box_count.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_chart_column_box_count(IntPtr chart, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_column_highest.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern double pnf_chart_column_highest(IntPtr chart, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_column_lowest.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern double pnf_chart_column_lowest(IntPtr chart, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_box_price.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="colIndex"></param>
        /// <param name="boxIndex"></param>
        /// <returns></returns>
        public static extern double pnf_chart_box_price(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_box_type.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="colIndex"></param>
        /// <param name="boxIndex"></param>
        /// <returns></returns>
        public static extern BoxType pnf_chart_box_type(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_box_marker.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="colIndex"></param>
        /// <param name="boxIndex"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_box_marker(IntPtr chart, UIntPtr colIndex, UIntPtr boxIndex);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_has_bullish_bias.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_has_bullish_bias(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_has_bearish_bias.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_has_bearish_bias(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_is_above_bullish_support.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="price"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_is_above_bullish_support(IntPtr chart, double price);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_chart_is_below_bearish_resistance.
        /// </summary>
        /// <param name="chart"></param>
        /// <param name="price"></param>
        /// <returns></returns>
        public static extern bool pnf_chart_is_below_bearish_resistance(IntPtr chart, double price);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_clear.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern void pnf_chart_clear(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_to_string.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_to_string(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_to_ascii.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_to_ascii(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_chart_to_json.
        /// </summary>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_chart_to_json(IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_create.
        /// </summary>
        /// <returns></returns>
        public static extern IntPtr pnf_indicators_create();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_create_default.
        /// </summary>
        /// <returns></returns>
        public static extern IntPtr pnf_indicators_create_default();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_create_with_config.
        /// </summary>
        /// <param name="config"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_indicators_create_with_config(ref IndicatorConfig config);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_destroy.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_destroy(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_configure.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="config"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_configure(IntPtr indicators, ref IndicatorConfig config);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_sma_periods.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="shortPeriod"></param>
        /// <param name="mediumPeriod"></param>
        /// <param name="longPeriod"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_sma_periods(IntPtr indicators, int shortPeriod, int mediumPeriod, int longPeriod);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_bollinger_params.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="period"></param>
        /// <param name="stdDevs"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_bollinger_params(IntPtr indicators, int period, double stdDevs);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_rsi_params.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="period"></param>
        /// <param name="overbought"></param>
        /// <param name="oversold"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_rsi_params(IntPtr indicators, int period, double overbought, double oversold);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_bullish_percent_thresholds.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="bullish"></param>
        /// <param name="bearish"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_bullish_percent_thresholds(IntPtr indicators, double bullish, double bearish);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_support_resistance_threshold.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="threshold"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_support_resistance_threshold(IntPtr indicators, double threshold);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_set_congestion_params.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="minColumns"></param>
        /// <param name="priceRange"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_set_congestion_params(IntPtr indicators, int minColumns, double priceRange);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_calculate.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="chart"></param>
        /// <returns></returns>
        public static extern void pnf_indicators_calculate(IntPtr indicators, IntPtr chart);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_short.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_sma_short(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_medium.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_sma_medium(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_long.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_sma_long(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_short_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_sma_short_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_medium_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_sma_medium_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sma_long_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_sma_long_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_middle.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_bollinger_middle(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_upper.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_bollinger_upper(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_lower.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_bollinger_lower(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_middle_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_bollinger_middle_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_upper_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_bollinger_upper_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bollinger_lower_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_bollinger_lower_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_rsi.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_rsi(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_rsi_is_overbought.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_rsi_is_overbought(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_rsi_is_oversold.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_rsi_is_oversold(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_rsi_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_rsi_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_obv.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_obv(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_obv_values.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_obv_values(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bullish_percent.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern double pnf_indicators_bullish_percent(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_is_bullish_alert.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_is_bullish_alert(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_is_bearish_alert.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_is_bearish_alert(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_current_signal.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern SignalType pnf_indicators_current_signal(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_signal_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_signal_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_signal_at.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern PnfSignal pnf_indicators_signal_at(IntPtr indicators, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_signals.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfSignalArray pnf_indicators_signals(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_buy_signal_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern int pnf_indicators_buy_signal_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_sell_signal_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern int pnf_indicators_sell_signal_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_pattern_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_pattern_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bullish_pattern_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_bullish_pattern_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bearish_pattern_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_bearish_pattern_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_pattern_at.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public static extern PnfPattern pnf_indicators_pattern_at(IntPtr indicators, UIntPtr index);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_patterns.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfPatternArray pnf_indicators_patterns(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bullish_patterns.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfPatternArray pnf_indicators_bullish_patterns(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_bearish_patterns.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfPatternArray pnf_indicators_bearish_patterns(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_support_level_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_support_level_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_resistance_level_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_resistance_level_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_is_near_support.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="price"></param>
        /// <param name="tolerance"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_is_near_support(IntPtr indicators, double price, double tolerance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_is_near_resistance.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="price"></param>
        /// <param name="tolerance"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_is_near_resistance(IntPtr indicators, double price, double tolerance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_support_levels.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfLevelArray pnf_indicators_support_levels(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_resistance_levels.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfLevelArray pnf_indicators_resistance_levels(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_support_prices.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_support_prices(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_resistance_prices.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern PnfDoubleArray pnf_indicators_resistance_prices(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        /// <summary>
        /// Native entry point: pnf_indicators_is_in_congestion.
        /// </summary>
        /// <param name="indicators"></param>
        /// <param name="column"></param>
        /// <returns></returns>
        public static extern bool pnf_indicators_is_in_congestion(IntPtr indicators, int column);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_congestion_zone_count.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern UIntPtr pnf_indicators_congestion_zone_count(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_summary.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_indicators_summary(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_indicators_to_string.
        /// </summary>
        /// <param name="indicators"></param>
        /// <returns></returns>
        public static extern IntPtr pnf_indicators_to_string(IntPtr indicators);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_free_string.
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static extern void pnf_free_string(IntPtr str);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_free_double_array.
        /// </summary>
        /// <param name="arr"></param>
        /// <returns></returns>
        public static extern void pnf_free_double_array(PnfDoubleArray arr);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_free_signal_array.
        /// </summary>
        /// <param name="arr"></param>
        /// <returns></returns>
        public static extern void pnf_free_signal_array(PnfSignalArray arr);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_free_pattern_array.
        /// </summary>
        /// <param name="arr"></param>
        /// <returns></returns>
        public static extern void pnf_free_pattern_array(PnfPatternArray arr);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Native entry point: pnf_free_level_array.
        /// </summary>
        /// <param name="arr"></param>
        /// <returns></returns>
        public static extern void pnf_free_level_array(PnfLevelArray arr);
    }
}
