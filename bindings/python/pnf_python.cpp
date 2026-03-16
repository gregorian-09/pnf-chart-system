//
// Created by gregorian-rayne on 15/01/2026.
//

/// \file pnf_python.cpp
/// \brief Pybind11 module definitions for the Python bindings.

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>
#include "pnf/pnf.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pypnf, m) {
    m.doc() = "Point and Figure Chart Library - Technical Analysis for P&F Charts";

    m.def("version", []() { return pnf::Version::string; });
    m.def("version_major", []() { return pnf::Version::major; });
    m.def("version_minor", []() { return pnf::Version::minor; });
    m.def("version_patch", []() { return pnf::Version::patch; });

    py::enum_<pnf::BoxType>(m, "BoxType")
        .value("X", pnf::BoxType::X)
        .value("O", pnf::BoxType::O);

    py::enum_<pnf::ColumnType>(m, "ColumnType")
        .value("X", pnf::ColumnType::X)
        .value("O", pnf::ColumnType::O)
        .value("Mixed", pnf::ColumnType::Mixed);

    py::enum_<pnf::ConstructionMethod>(m, "ConstructionMethod")
        .value("Close", pnf::ConstructionMethod::Close)
        .value("HighLow", pnf::ConstructionMethod::HighLow);

    py::enum_<pnf::BoxSizeMethod>(m, "BoxSizeMethod")
        .value("Fixed", pnf::BoxSizeMethod::Fixed)
        .value("Traditional", pnf::BoxSizeMethod::Traditional)
        .value("Percentage", pnf::BoxSizeMethod::Percentage)
        .value("Points", pnf::BoxSizeMethod::Points);

    py::enum_<pnf::SignalType>(m, "SignalType")
        .value("NONE", pnf::SignalType::None)
        .value("Buy", pnf::SignalType::Buy)
        .value("Sell", pnf::SignalType::Sell);

    py::enum_<pnf::PatternType>(m, "PatternType")
        .value("NONE", pnf::PatternType::None)
        .value("DoubleTopBreakout", pnf::PatternType::DoubleTopBreakout)
        .value("DoubleBottomBreakdown", pnf::PatternType::DoubleBottomBreakdown)
        .value("TripleTopBreakout", pnf::PatternType::TripleTopBreakout)
        .value("TripleBottomBreakdown", pnf::PatternType::TripleBottomBreakdown)
        .value("QuadrupleTopBreakout", pnf::PatternType::QuadrupleTopBreakout)
        .value("QuadrupleBottomBreakdown", pnf::PatternType::QuadrupleBottomBreakdown)
        .value("AscendingTripleTop", pnf::PatternType::AscendingTripleTop)
        .value("DescendingTripleBottom", pnf::PatternType::DescendingTripleBottom)
        .value("BullishCatapult", pnf::PatternType::BullishCatapult)
        .value("BearishCatapult", pnf::PatternType::BearishCatapult)
        .value("BullishSignalReversed", pnf::PatternType::BullishSignalReversed)
        .value("BearishSignalReversed", pnf::PatternType::BearishSignalReversed)
        .value("BullishTriangle", pnf::PatternType::BullishTriangle)
        .value("BearishTriangle", pnf::PatternType::BearishTriangle)
        .value("LongTailDown", pnf::PatternType::LongTailDown)
        .value("HighPole", pnf::PatternType::HighPole)
        .value("LowPole", pnf::PatternType::LowPole)
        .value("BullTrap", pnf::PatternType::BullTrap)
        .value("BearTrap", pnf::PatternType::BearTrap)
        .value("SpreadTripleTop", pnf::PatternType::SpreadTripleTop)
        .value("SpreadTripleBottom", pnf::PatternType::SpreadTripleBottom);

    py::class_<pnf::ChartConfig>(m, "ChartConfig")
        .def(py::init<>())
        .def_readwrite("method", &pnf::ChartConfig::method)
        .def_readwrite("box_size_method", &pnf::ChartConfig::box_size_method)
        .def_readwrite("box_size", &pnf::ChartConfig::box_size)
        .def_readwrite("reversal", &pnf::ChartConfig::reversal);

    py::class_<pnf::IndicatorConfig>(m, "IndicatorConfig")
        .def(py::init<>())
        .def_readwrite("sma_short_period", &pnf::IndicatorConfig::sma_short_period)
        .def_readwrite("sma_medium_period", &pnf::IndicatorConfig::sma_medium_period)
        .def_readwrite("sma_long_period", &pnf::IndicatorConfig::sma_long_period)
        .def_readwrite("bollinger_period", &pnf::IndicatorConfig::bollinger_period)
        .def_readwrite("bollinger_std_devs", &pnf::IndicatorConfig::bollinger_std_devs)
        .def_readwrite("rsi_period", &pnf::IndicatorConfig::rsi_period)
        .def_readwrite("rsi_overbought", &pnf::IndicatorConfig::rsi_overbought)
        .def_readwrite("rsi_oversold", &pnf::IndicatorConfig::rsi_oversold)
        .def_readwrite("bullish_alert_threshold", &pnf::IndicatorConfig::bullish_alert_threshold)
        .def_readwrite("bearish_alert_threshold", &pnf::IndicatorConfig::bearish_alert_threshold)
        .def_readwrite("support_resistance_threshold", &pnf::IndicatorConfig::support_resistance_threshold)
        .def_readwrite("congestion_min_columns", &pnf::IndicatorConfig::congestion_min_columns)
        .def_readwrite("congestion_price_range", &pnf::IndicatorConfig::congestion_price_range);

    py::class_<pnf::ColumnData>(m, "ColumnData")
        .def_readonly("index", &pnf::ColumnData::index)
        .def_readonly("type", &pnf::ColumnData::type)
        .def_readonly("high", &pnf::ColumnData::high)
        .def_readonly("low", &pnf::ColumnData::low)
        .def_readonly("box_count", &pnf::ColumnData::box_count)
        .def_readonly("marker", &pnf::ColumnData::marker);

    py::class_<pnf::ChartData>(m, "ChartData")
        .def_readonly("columns", &pnf::ChartData::columns)
        .def_readonly("prices", &pnf::ChartData::prices)
        .def_readonly("box_size", &pnf::ChartData::box_size)
        .def_readonly("reversal", &pnf::ChartData::reversal)
        .def_readonly("method", &pnf::ChartData::method);

    py::class_<pnf::IndicatorData>(m, "IndicatorData")
        .def_readonly("sma_short", &pnf::IndicatorData::sma_short)
        .def_readonly("sma_medium", &pnf::IndicatorData::sma_medium)
        .def_readonly("sma_long", &pnf::IndicatorData::sma_long)
        .def_readonly("bollinger_middle", &pnf::IndicatorData::bollinger_middle)
        .def_readonly("bollinger_upper", &pnf::IndicatorData::bollinger_upper)
        .def_readonly("bollinger_lower", &pnf::IndicatorData::bollinger_lower)
        .def_readonly("rsi", &pnf::IndicatorData::rsi)
        .def_readonly("obv", &pnf::IndicatorData::obv)
        .def_readonly("bullish_percent", &pnf::IndicatorData::bullish_percent)
        .def_readonly("signals", &pnf::IndicatorData::signals)
        .def_readonly("patterns", &pnf::IndicatorData::patterns)
        .def_readonly("support_levels", &pnf::IndicatorData::support_levels)
        .def_readonly("resistance_levels", &pnf::IndicatorData::resistance_levels)
        .def_readonly("price_objectives", &pnf::IndicatorData::price_objectives);

    py::class_<pnf::OHLC>(m, "OHLC")
        .def(py::init<>())
        .def_readwrite("time", &pnf::OHLC::time)
        .def_readwrite("open", &pnf::OHLC::open)
        .def_readwrite("high", &pnf::OHLC::high)
        .def_readwrite("low", &pnf::OHLC::low)
        .def_readwrite("close", &pnf::OHLC::close)
        .def_readwrite("volume", &pnf::OHLC::volume);

    py::class_<pnf::Signal>(m, "Signal")
        .def_readonly("type", &pnf::Signal::type)
        .def_readonly("column_index", &pnf::Signal::column_index)
        .def_readonly("price", &pnf::Signal::price);

    py::class_<pnf::Pattern>(m, "Pattern")
        .def_readonly("type", &pnf::Pattern::type)
        .def_readonly("start_column", &pnf::Pattern::start_column)
        .def_readonly("end_column", &pnf::Pattern::end_column)
        .def_readonly("price", &pnf::Pattern::price)
        .def("is_bullish", [](const pnf::Pattern& p) { return pnf::is_bullish_pattern(p.type); });

    py::class_<pnf::SupportResistanceLevel>(m, "SupportResistanceLevel")
        .def_readonly("price", &pnf::SupportResistanceLevel::price)
        .def_readonly("touch_count", &pnf::SupportResistanceLevel::touch_count);

    py::class_<pnf::PriceObjective>(m, "PriceObjective")
        .def_readonly("target_price", &pnf::PriceObjective::target_price)
        .def_readonly("base_column", &pnf::PriceObjective::base_column)
        .def_readonly("box_count", &pnf::PriceObjective::box_count)
        .def_readonly("is_bullish", &pnf::PriceObjective::is_bullish);

    py::class_<pnf::Box>(m, "Box")
        .def("price", &pnf::Box::price)
        .def("type", &pnf::Box::type)
        .def("marker", &pnf::Box::marker)
        .def("__str__", &pnf::Box::to_string);

    py::class_<pnf::Column, std::unique_ptr<pnf::Column, py::nodelete>>(m, "Column")
        .def("box_count", &pnf::Column::box_count)
        .def("type", &pnf::Column::type)
        .def("highest_price", &pnf::Column::highest_price)
        .def("lowest_price", &pnf::Column::lowest_price)
        .def("get_box_at",
             static_cast<const pnf::Box* (pnf::Column::*)(size_t) const>(&pnf::Column::get_box_at),
             py::return_value_policy::reference_internal)
        .def("has_box", &pnf::Column::has_box)
        .def("__str__", &pnf::Column::to_string);

    py::class_<pnf::Chart>(m, "Chart")
        .def(py::init<>())
        .def(py::init<const pnf::ChartConfig&>())
        .def("add_data", py::overload_cast<double, double, double, pnf::Timestamp>(&pnf::Chart::add_data))
        .def("add_price", py::overload_cast<double, pnf::Timestamp>(&pnf::Chart::add_data))
        .def("add_ohlc", &pnf::Chart::add_ohlc)
        .def("column_count", &pnf::Chart::column_count)
        .def("column_type", [](const pnf::Chart& c, size_t i) -> pnf::ColumnType {
            const auto* col = c.column(i);
            return col ? col->type() : pnf::ColumnType::X;
        })
        .def("column_box_count", [](const pnf::Chart& c, size_t i) -> size_t {
            const auto* col = c.column(i);
            return col ? col->box_count() : 0;
        })
        .def("column_high", [](const pnf::Chart& c, size_t i) -> double {
            const auto* col = c.column(i);
            return col ? col->highest_price() : 0.0;
        })
        .def("column_low", [](const pnf::Chart& c, size_t i) -> double {
            const auto* col = c.column(i);
            return col ? col->lowest_price() : 0.0;
        })
        .def("box_price", [](const pnf::Chart& c, size_t col_index, size_t box_index) -> double {
            const auto* col = c.column(col_index);
            const auto* box = col ? col->get_box_at(box_index) : nullptr;
            return box ? box->price() : 0.0;
        })
        .def("box_type", [](const pnf::Chart& c, size_t col_index, size_t box_index) -> pnf::BoxType {
            const auto* col = c.column(col_index);
            const auto* box = col ? col->get_box_at(box_index) : nullptr;
            return box ? box->type() : pnf::BoxType::X;
        })
        .def("box_marker", [](const pnf::Chart& c, size_t col_index, size_t box_index) -> std::string {
            const auto* col = c.column(col_index);
            const auto* box = col ? col->get_box_at(box_index) : nullptr;
            return box ? box->marker() : std::string{};
        })
        .def("x_column_count", &pnf::Chart::x_column_count)
        .def("o_column_count", &pnf::Chart::o_column_count)
        .def("all_prices", &pnf::Chart::all_prices)
        .def("current_box_size", &pnf::Chart::current_box_size)
        .def("has_bullish_bias", &pnf::Chart::has_bullish_bias)
        .def("has_bearish_bias", &pnf::Chart::has_bearish_bias)
        .def("is_above_bullish_support", &pnf::Chart::is_above_bullish_support)
        .def("is_below_bearish_resistance", &pnf::Chart::is_below_bearish_resistance)
        .def("clear", &pnf::Chart::clear)
        .def("to_ascii", [](const pnf::Chart& c) { return pnf::Visualization::to_ascii(c); })
        .def("to_json", [](const pnf::Chart& c) { return pnf::Visualization::to_json(c); })
        .def("__str__", &pnf::Chart::to_string)
        .def("__len__", &pnf::Chart::column_count);

    py::class_<pnf::MovingAverage>(m, "MovingAverage")
        .def("value", &pnf::MovingAverage::value)
        .def("has_value", &pnf::MovingAverage::has_value)
        .def("period", &pnf::MovingAverage::period)
        .def("set_period", &pnf::MovingAverage::set_period)
        .def("values", &pnf::MovingAverage::values)
        .def("values_copy", &pnf::MovingAverage::values_copy)
        .def("__str__", &pnf::MovingAverage::to_string);

    py::class_<pnf::BollingerBands>(m, "BollingerBands")
        .def("middle", &pnf::BollingerBands::middle)
        .def("upper", &pnf::BollingerBands::upper)
        .def("lower", &pnf::BollingerBands::lower)
        .def("has_value", &pnf::BollingerBands::has_value)
        .def("is_above_upper", &pnf::BollingerBands::is_above_upper)
        .def("is_below_lower", &pnf::BollingerBands::is_below_lower)
        .def("period", &pnf::BollingerBands::period)
        .def("std_devs", &pnf::BollingerBands::std_devs)
        .def("set_period", &pnf::BollingerBands::set_period)
        .def("set_std_devs", &pnf::BollingerBands::set_std_devs)
        .def("middle_band", &pnf::BollingerBands::middle_band)
        .def("upper_band", &pnf::BollingerBands::upper_band)
        .def("lower_band", &pnf::BollingerBands::lower_band)
        .def("middle_copy", &pnf::BollingerBands::middle_copy)
        .def("upper_copy", &pnf::BollingerBands::upper_copy)
        .def("lower_copy", &pnf::BollingerBands::lower_copy)
        .def("__str__", &pnf::BollingerBands::to_string);

    py::class_<pnf::RSI>(m, "RSI")
        .def("value", &pnf::RSI::value)
        .def("has_value", &pnf::RSI::has_value)
        .def("is_overbought", py::overload_cast<int>(&pnf::RSI::is_overbought, py::const_))
        .def("is_oversold", py::overload_cast<int>(&pnf::RSI::is_oversold, py::const_))
        .def("is_overbought_custom", &pnf::RSI::is_overbought_custom)
        .def("is_oversold_custom", &pnf::RSI::is_oversold_custom)
        .def("period", &pnf::RSI::period)
        .def("overbought_threshold", &pnf::RSI::overbought_threshold)
        .def("oversold_threshold", &pnf::RSI::oversold_threshold)
        .def("set_period", &pnf::RSI::set_period)
        .def("set_thresholds", &pnf::RSI::set_thresholds)
        .def("values", &pnf::RSI::values)
        .def("values_copy", &pnf::RSI::values_copy)
        .def("__str__", &pnf::RSI::to_string);

    py::class_<pnf::OnBalanceVolume>(m, "OnBalanceVolume")
        .def("value", &pnf::OnBalanceVolume::value)
        .def("has_value", &pnf::OnBalanceVolume::has_value)
        .def("values", &pnf::OnBalanceVolume::values)
        .def("values_copy", &pnf::OnBalanceVolume::values_copy)
        .def("__str__", &pnf::OnBalanceVolume::to_string);

    py::class_<pnf::BullishPercent>(m, "BullishPercent")
        .def("value", &pnf::BullishPercent::value)
        .def("is_bullish_alert", &pnf::BullishPercent::is_bullish_alert)
        .def("is_bearish_alert", &pnf::BullishPercent::is_bearish_alert)
        .def("bullish_threshold", &pnf::BullishPercent::bullish_threshold)
        .def("bearish_threshold", &pnf::BullishPercent::bearish_threshold)
        .def("set_thresholds", &pnf::BullishPercent::set_thresholds)
        .def("__str__", &pnf::BullishPercent::to_string);

    py::class_<pnf::SignalDetector>(m, "SignalDetector")
        .def("current_signal", &pnf::SignalDetector::current_signal)
        .def("signals", &pnf::SignalDetector::signals)
        .def("signals_copy", &pnf::SignalDetector::signals_copy)
        .def("last_signal", &pnf::SignalDetector::last_signal)
        .def("has_buy_signal", &pnf::SignalDetector::has_buy_signal)
        .def("has_sell_signal", &pnf::SignalDetector::has_sell_signal)
        .def("buy_signals", &pnf::SignalDetector::buy_signals)
        .def("sell_signals", &pnf::SignalDetector::sell_signals)
        .def("buy_count", &pnf::SignalDetector::buy_count)
        .def("sell_count", &pnf::SignalDetector::sell_count)
        .def("__str__", &pnf::SignalDetector::to_string);

    py::class_<pnf::PatternRecognizer>(m, "PatternRecognizer")
        .def("patterns", &pnf::PatternRecognizer::patterns)
        .def("patterns_copy", &pnf::PatternRecognizer::patterns_copy)
        .def("bullish_patterns", &pnf::PatternRecognizer::bullish_patterns)
        .def("bearish_patterns", &pnf::PatternRecognizer::bearish_patterns)
        .def("latest_pattern", &pnf::PatternRecognizer::latest_pattern)
        .def("has_pattern", &pnf::PatternRecognizer::has_pattern)
        .def("patterns_of_type", &pnf::PatternRecognizer::patterns_of_type)
        .def("pattern_count", &pnf::PatternRecognizer::pattern_count)
        .def("bullish_count", &pnf::PatternRecognizer::bullish_count)
        .def("bearish_count", &pnf::PatternRecognizer::bearish_count)
        .def("__str__", &pnf::PatternRecognizer::to_string);

    py::class_<pnf::SupportResistance>(m, "SupportResistance")
        .def("support_levels", &pnf::SupportResistance::support_levels)
        .def("resistance_levels", &pnf::SupportResistance::resistance_levels)
        .def("levels_copy", &pnf::SupportResistance::levels_copy)
        .def("significant_levels", &pnf::SupportResistance::significant_levels, py::arg("min_touches") = 3)
        .def("is_near_support", &pnf::SupportResistance::is_near_support)
        .def("is_near_resistance", &pnf::SupportResistance::is_near_resistance)
        .def("support_prices", &pnf::SupportResistance::support_prices)
        .def("resistance_prices", &pnf::SupportResistance::resistance_prices)
        .def("threshold", &pnf::SupportResistance::threshold)
        .def("set_threshold", &pnf::SupportResistance::set_threshold)
        .def("__str__", &pnf::SupportResistance::to_string);

    py::class_<pnf::PriceObjectiveCalculator>(m, "PriceObjectiveCalculator")
        .def("objectives", &pnf::PriceObjectiveCalculator::objectives)
        .def("objectives_copy", &pnf::PriceObjectiveCalculator::objectives_copy)
        .def("latest", &pnf::PriceObjectiveCalculator::latest)
        .def("bullish_objectives", &pnf::PriceObjectiveCalculator::bullish_objectives)
        .def("bearish_objectives", &pnf::PriceObjectiveCalculator::bearish_objectives)
        .def("bullish_targets", &pnf::PriceObjectiveCalculator::bullish_targets)
        .def("bearish_targets", &pnf::PriceObjectiveCalculator::bearish_targets)
        .def("__str__", &pnf::PriceObjectiveCalculator::to_string);

    py::class_<pnf::CongestionDetector::CongestionZone>(m, "CongestionZone")
        .def_readonly("start_column", &pnf::CongestionDetector::CongestionZone::start_column)
        .def_readonly("end_column", &pnf::CongestionDetector::CongestionZone::end_column)
        .def_readonly("high_price", &pnf::CongestionDetector::CongestionZone::high_price)
        .def_readonly("low_price", &pnf::CongestionDetector::CongestionZone::low_price)
        .def_readonly("column_count", &pnf::CongestionDetector::CongestionZone::column_count);

    py::class_<pnf::CongestionDetector>(m, "CongestionDetector")
        .def("zones", &pnf::CongestionDetector::zones)
        .def("zones_copy", &pnf::CongestionDetector::zones_copy)
        .def("is_in_congestion", &pnf::CongestionDetector::is_in_congestion)
        .def("largest_zone", &pnf::CongestionDetector::largest_zone)
        .def("min_columns", &pnf::CongestionDetector::min_columns)
        .def("threshold", &pnf::CongestionDetector::threshold)
        .def("set_min_columns", &pnf::CongestionDetector::set_min_columns)
        .def("set_threshold", &pnf::CongestionDetector::set_threshold)
        .def("__str__", &pnf::CongestionDetector::to_string);

    py::class_<pnf::Indicators>(m, "Indicators")
        .def(py::init<>())
        .def(py::init<const pnf::IndicatorConfig&>())
        .def("configure", &pnf::Indicators::configure)
        .def("config", &pnf::Indicators::config)
        .def("calculate", &pnf::Indicators::calculate)
        .def("calculate_with_volume", &pnf::Indicators::calculate_with_volume)
        .def("sma_short",
             static_cast<const pnf::MovingAverage* (pnf::Indicators::*)() const>(&pnf::Indicators::sma_short),
             py::return_value_policy::reference)
        .def("sma_medium",
             static_cast<const pnf::MovingAverage* (pnf::Indicators::*)() const>(&pnf::Indicators::sma_medium),
             py::return_value_policy::reference)
        .def("sma_long",
             static_cast<const pnf::MovingAverage* (pnf::Indicators::*)() const>(&pnf::Indicators::sma_long),
             py::return_value_policy::reference)
        .def("bollinger",
             static_cast<const pnf::BollingerBands* (pnf::Indicators::*)() const>(&pnf::Indicators::bollinger),
             py::return_value_policy::reference)
        .def("rsi",
             static_cast<const pnf::RSI* (pnf::Indicators::*)() const>(&pnf::Indicators::rsi),
             py::return_value_policy::reference)
        .def("obv",
             static_cast<const pnf::OnBalanceVolume* (pnf::Indicators::*)() const>(&pnf::Indicators::obv),
             py::return_value_policy::reference)
        .def("bullish_percent",
             static_cast<const pnf::BullishPercent* (pnf::Indicators::*)() const>(&pnf::Indicators::bullish_percent),
             py::return_value_policy::reference)
        .def("signals",
             static_cast<const pnf::SignalDetector* (pnf::Indicators::*)() const>(&pnf::Indicators::signals),
             py::return_value_policy::reference)
        .def("patterns",
             static_cast<const pnf::PatternRecognizer* (pnf::Indicators::*)() const>(&pnf::Indicators::patterns),
             py::return_value_policy::reference)
        .def("support_resistance",
             static_cast<const pnf::SupportResistance* (pnf::Indicators::*)() const>(&pnf::Indicators::support_resistance),
             py::return_value_policy::reference)
        .def("objectives",
             static_cast<const pnf::PriceObjectiveCalculator* (pnf::Indicators::*)() const>(&pnf::Indicators::objectives),
             py::return_value_policy::reference)
        .def("congestion",
             static_cast<const pnf::CongestionDetector* (pnf::Indicators::*)() const>(&pnf::Indicators::congestion),
             py::return_value_policy::reference)
        .def("export_data", &pnf::Indicators::export_data)
        .def_static("export_chart_data", &pnf::Indicators::export_chart_data)
        .def("summary", &pnf::Indicators::summary)
        .def("__str__", &pnf::Indicators::to_string);

    py::class_<pnf::Visualization>(m, "Visualization")
        .def_static("to_ascii", [](const pnf::Chart& c) { return pnf::Visualization::to_ascii(c); })
        .def_static("to_json", [](const pnf::Chart& c) { return pnf::Visualization::to_json(c); })
        .def_static("to_csv_columns", &pnf::Visualization::to_csv_columns)
        .def_static("to_csv_boxes", &pnf::Visualization::to_csv_boxes);

    py::class_<pnf::CSVLoader>(m, "CSVLoader")
        .def_static("load", &pnf::CSVLoader::load);
}
