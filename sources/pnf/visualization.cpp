/// \file visualization.cpp
/// \brief Visualization implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include "pnf/visualization.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace pnf
{
    AsciiRenderer::AsciiRenderer(const RenderConfig& config) : config_(config) {}

    std::string AsciiRenderer::render_column_header(const size_t start, const size_t count) const {
        std::ostringstream oss;
        oss << std::string(config_.show_price_axis ? 10 : 0, ' ');

        for (size_t c = 0; c < count; c++) {
            if ((start + c) % 5 == 0) {
                oss << std::setw(config_.cell_width) << (start + c);
            } else {
                oss << std::string(config_.cell_width, ' ');
            }
        }
        oss << "\n";

        return oss.str();
    }

    std::string AsciiRenderer::render_price_axis(const double min_price, const double max_price, const double box_size) const {
        std::ostringstream oss;
        const int rows = static_cast<int>((max_price - min_price) / box_size) + 1;

        for (int r = 0; r < rows; r++) {
            const double price = max_price - r * box_size;
            oss << std::fixed << std::setprecision(config_.price_decimals)
                << std::setw(9) << price;
            if (r < rows - 1) oss << "\n";
        }

        return oss.str();
    }

    std::string AsciiRenderer::render(const Chart& chart) const {
        if (chart.column_count() == 0) return "Empty chart\n";

        std::ostringstream oss;
        const double box_size = chart.current_box_size();

        double min_price = std::numeric_limits<double>::max();
        double max_price = std::numeric_limits<double>::lowest();

        size_t start_col = 0;
        const size_t end_col = chart.column_count();
        if (config_.max_columns > 0 && chart.column_count() > static_cast<size_t>(config_.max_columns)) {
            start_col = chart.column_count() - config_.max_columns;
        }

        for (size_t i = start_col; i < end_col; i++) {
            const Column* col = chart.column(i);
            min_price = std::min(min_price, col->lowest_price());
            max_price = std::max(max_price, col->highest_price());
        }

        const int rows = static_cast<int>((max_price - min_price) / box_size) + 1;
        const size_t cols = end_col - start_col;

        std::vector<std::vector<char>> grid(rows, std::vector<char>(cols, config_.empty_char));

        for (size_t c = start_col; c < end_col; c++) {
            const Column* col = chart.column(c);
            const char ch = (col->type() == ColumnType::X) ? config_.x_char : config_.o_char;

            for (size_t b = 0; b < col->box_count(); b++) {
                const Box* box = col->get_box_at(b);
                if (const int row = static_cast<int>((max_price - box->price()) / box_size); row >= 0 && row < rows) {
                    grid[row][c - start_col] = ch;
                }
            }
        }

        if (config_.show_column_numbers) {
            oss << render_column_header(start_col, cols);
        }

        for (int r = 0; r < rows; r++) {
            if (config_.show_price_axis) {
                const double price = max_price - r * box_size;
                oss << std::fixed << std::setprecision(config_.price_decimals)
                    << std::setw(9) << price << " ";
            }

            for (size_t c = 0; c < cols; c++) {
                std::string cell(config_.cell_width, ' ');
                cell[config_.cell_width / 2] = grid[r][c];
                oss << cell;
            }
            oss << "\n";
        }

        if (config_.show_month_markers) {
            oss << std::string(config_.show_price_axis ? 10 : 0, ' ');
            for (size_t c = start_col; c < end_col; c++) {
                const Column* col = chart.column(c);
                std::string marker;
                if (col->box_count() > 0) {
                    if (const Box* first_box = col->get_box_at(0)) marker = first_box->marker();
                }
                if (!marker.empty()) {
                    oss << std::setw(config_.cell_width) << marker;
                } else {
                    oss << std::string(config_.cell_width, ' ');
                }
            }
            oss << "\n";
        }

        return oss.str();
    }

    std::string AsciiRenderer::render_with_indicators(const Chart& chart, const Indicators& indicators) const {
        std::ostringstream oss;
        oss << render(chart);
        oss << "\n" << indicators.summary();
        return oss.str();
    }

    SvgRenderer::SvgRenderer(const SvgConfig& config) : config_(config) {}

    std::string SvgRenderer::render_header(const int width, const int height)
    {
        std::ostringstream oss;
        oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        oss << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << width
            << "\" height=\"" << height << "\" viewBox=\"0 0 " << width << " " << height << "\">\n";
        oss << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
        return oss.str();
    }

    std::string SvgRenderer::render_grid(const int cols, const int rows) const {
        if (!config_.show_grid) return "";

        std::ostringstream oss;
        oss << "<g stroke=\"" << config_.grid_color << "\" stroke-width=\"0.5\">\n";

        for (int r = 0; r <= rows; r++) {
            const int y = config_.margin_top + r * config_.box_height;
            oss << "  <line x1=\"" << config_.margin_left << "\" y1=\"" << y
                << "\" x2=\"" << (config_.margin_left + cols * config_.box_width)
                << "\" y2=\"" << y << "\"/>\n";
        }

        for (int c = 0; c <= cols; c++) {
            const int x = config_.margin_left + c * config_.box_width;
            oss << "  <line x1=\"" << x << "\" y1=\"" << config_.margin_top
                << "\" x2=\"" << x << "\" y2=\"" << (config_.margin_top + rows * config_.box_height)
                << "\"/>\n";
        }

        oss << "</g>\n";
        return oss.str();
    }

    std::string SvgRenderer::render_price_axis(const int rows, const double min_price, const double box_size) const {
        std::ostringstream oss;
        oss << "<g fill=\"" << config_.axis_color << "\" font-family=\"" << config_.font_family
            << "\" font-size=\"" << config_.font_size << "\">\n";

        for (int r = 0; r <= rows; r += std::max(1, rows / 10)) {
            const double price = min_price + (rows - r) * box_size;
            const int y = config_.margin_top + r * config_.box_height + config_.font_size / 3;
            oss << "  <text x=\"" << (config_.margin_left - 5) << "\" y=\"" << y
                << R"(" text-anchor="end">)" << std::fixed << std::setprecision(2) << price << "</text>\n";
        }

        oss << "</g>\n";
        return oss.str();
    }

    std::string SvgRenderer::render_column(const Column* col, const int col_index, const double min_price, const double box_size) const {
        std::ostringstream oss;
        const std::string color = (col->type() == ColumnType::X) ? config_.x_color : config_.o_color;
        const char symbol = (col->type() == ColumnType::X) ? 'X' : 'O';

        const int x_center = config_.margin_left + col_index * config_.box_width + config_.box_width / 2;

        for (size_t b = 0; b < col->box_count(); b++) {
            const Box* box = col->get_box_at(b);
            const double price = box->price();
            const int row = static_cast<int>((price - min_price) / box_size);
            const int y_center = config_.margin_top + (static_cast<int>((col->highest_price() - min_price) / box_size) - row) * config_.box_height + config_.box_height / 2;

            oss << "  <text x=\"" << x_center << "\" y=\"" << (y_center + config_.font_size / 3)
                << R"(" text-anchor="middle" fill=")" << color << "\" font-family=\""
                << config_.font_family << "\" font-size=\"" << config_.font_size
                << R"(" font-weight="bold">)" << symbol << "</text>\n";
        }

        return oss.str();
    }

    std::string SvgRenderer::render_trend_lines(const Chart& chart, const double box_size) const {
        if (!config_.show_trend_lines) return "";

        std::ostringstream oss;
        const TrendLineManager* tm = chart.trend_line_manager();
        if (!tm) return "";

        oss << "<g stroke=\"" << config_.trend_line_color << "\" stroke-width=\"2\" stroke-dasharray=\"5,3\">\n";

        for (const auto& lines = tm->all_trend_lines(); const auto & i : lines) {
            const TrendLine* line = i.get();
            if (!line || !line->is_active()) continue;

            const TrendLinePoint& start = line->start_point();
            const TrendLinePoint& end = line->end_point();

            const int x1 = config_.margin_left + start.column_index * config_.box_width + config_.box_width / 2;
            const int y1 = config_.margin_top + static_cast<int>((chart.column(0)->highest_price() - start.price) / box_size) * config_.box_height + config_.box_height / 2;
            const int x2 = config_.margin_left + end.column_index * config_.box_width + config_.box_width / 2;
            const int y2 = config_.margin_top + static_cast<int>((chart.column(0)->highest_price() - end.price) / box_size) * config_.box_height + config_.box_height / 2;

            oss << "  <line x1=\"" << x1 << "\" y1=\"" << y1
                << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\"/>\n";
        }

        oss << "</g>\n";
        return oss.str();
    }

    std::string SvgRenderer::render_support_resistance(const Indicators& indicators, const int cols, const double box_size) const {
        if (!config_.show_support_resistance) return "";

        std::ostringstream oss;
        const SupportResistance* sr = indicators.support_resistance();
        if (!sr) return "";

        const int chart_width = cols * config_.box_width;

        oss << "<g stroke-width=\"1\" stroke-dasharray=\"3,3\">\n";

        for (const auto& level : sr->support_levels()) {
            const int y = config_.margin_top + static_cast<int>((sr->levels().front().price - level.price) / box_size) * config_.box_height;
            oss << "  <line x1=\"" << config_.margin_left << "\" y1=\"" << y
                << "\" x2=\"" << (config_.margin_left + chart_width) << "\" y2=\"" << y
                << "\" stroke=\"" << config_.support_color << "\"/>\n";
        }

        for (const auto& level : sr->resistance_levels()) {
            const int y = config_.margin_top + static_cast<int>((sr->levels().front().price - level.price) / box_size) * config_.box_height;
            oss << "  <line x1=\"" << config_.margin_left << "\" y1=\"" << y
                << "\" x2=\"" << (config_.margin_left + chart_width) << "\" y2=\"" << y
                << "\" stroke=\"" << config_.resistance_color << "\"/>\n";
        }

        oss << "</g>\n";
        return oss.str();
    }

    std::string SvgRenderer::render_footer()
    {
        return "</svg>\n";
    }

    std::string SvgRenderer::render(const Chart& chart) const {
        if (chart.column_count() == 0) return "";

        double min_price = std::numeric_limits<double>::max();
        double max_price = std::numeric_limits<double>::lowest();
        const double box_size = chart.current_box_size();

        for (size_t i = 0; i < chart.column_count(); i++) {
            const Column* col = chart.column(i);
            min_price = std::min(min_price, col->lowest_price());
            max_price = std::max(max_price, col->highest_price());
        }

        const int rows = static_cast<int>((max_price - min_price) / box_size) + 1;
        const int cols = static_cast<int>(chart.column_count());

        const int width = config_.margin_left + cols * config_.box_width + config_.margin_right;
        const int height = config_.margin_top + rows * config_.box_height + config_.margin_bottom;

        std::ostringstream oss;
        oss << render_header(width, height);
        oss << render_grid(cols, rows);
        oss << render_price_axis(rows, min_price, box_size);

        oss << "<g>\n";
        for (size_t c = 0; c < chart.column_count(); c++) {
            oss << render_column(chart.column(c), static_cast<int>(c), min_price, box_size);
        }
        oss << "</g>\n";

        oss << render_trend_lines(chart, box_size);
        oss << render_footer();

        return oss.str();
    }

    std::string SvgRenderer::render_with_indicators(const Chart& chart, const Indicators& indicators) const {
        if (chart.column_count() == 0) return "";

        double min_price = std::numeric_limits<double>::max();
        double max_price = std::numeric_limits<double>::lowest();
        const double box_size = chart.current_box_size();

        for (size_t i = 0; i < chart.column_count(); i++) {
            const Column* col = chart.column(i);
            min_price = std::min(min_price, col->lowest_price());
            max_price = std::max(max_price, col->highest_price());
        }

        const int rows = static_cast<int>((max_price - min_price) / box_size) + 1;
        const int cols = static_cast<int>(chart.column_count());

        const int width = config_.margin_left + cols * config_.box_width + config_.margin_right;
        const int height = config_.margin_top + rows * config_.box_height + config_.margin_bottom;

        std::ostringstream oss;
        oss << render_header(width, height);
        oss << render_grid(cols, rows);
        oss << render_price_axis(rows, min_price, box_size);

        oss << "<g>\n";
        for (size_t c = 0; c < chart.column_count(); c++) {
            oss << render_column(chart.column(c), static_cast<int>(c), min_price, box_size);
        }
        oss << "</g>\n";

        oss << render_trend_lines(chart, box_size);
        oss << render_support_resistance(indicators, cols, box_size);
        oss << render_footer();

        return oss.str();
    }

    JsonExporter::JsonExporter(const JsonConfig& config) : config_(config) {}

    std::string JsonExporter::indent(const int level) const {
        if (!config_.pretty_print) return "";
        return {static_cast<char>(level * config_.indent), ' '};
    }

    std::string JsonExporter::escape_string(const std::string& s) {
        std::ostringstream oss;
        for (const char c : s) {
            switch (c) {
            case '"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default: oss << c;
            }
        }
        return oss.str();
    }

    std::string JsonExporter::export_chart(const Chart& chart) const {
        std::ostringstream oss;
        const std::string nl = config_.pretty_print ? "\n" : "";
        const std::string sp = config_.pretty_print ? " " : "";

        oss << "{" << nl;
        oss << indent(1) << "\"box_size\":" << sp << chart.current_box_size() << "," << nl;
        oss << indent(1) << "\"reversal\":" << sp << chart.config().reversal << "," << nl;
        oss << indent(1) << "\"method\":" << sp << "\"" << (chart.config().method == ConstructionMethod::Close ? "close" : "high_low") << "\"," << nl;
        oss << indent(1) << "\"column_count\":" << sp << chart.column_count() << "," << nl;

        oss << indent(1) << "\"columns\":" << sp << "[" << nl;
        for (size_t i = 0; i < chart.column_count(); i++) {
            const Column* col = chart.column(i);
            oss << indent(2) << "{" << nl;
            oss << indent(3) << "\"index\":" << sp << i << "," << nl;
            oss << indent(3) << "\"type\":" << sp << "\"" << (col->type() == ColumnType::X ? "X" : "O") << "\"," << nl;
            oss << indent(3) << "\"high\":" << sp << col->highest_price() << "," << nl;
            oss << indent(3) << "\"low\":" << sp << col->lowest_price() << "," << nl;
            oss << indent(3) << "\"box_count\":" << sp << col->box_count();

            if (config_.include_boxes) {
                oss << "," << nl;
                oss << indent(3) << "\"boxes\":" << sp << "[";
                for (size_t b = 0; b < col->box_count(); b++) {
                    if (b > 0) oss << "," << sp;
                    oss << col->get_box_at(b)->price();
                }
                oss << "]" << nl;
            } else {
                oss << nl;
            }

            oss << indent(2) << "}" << (i < chart.column_count() - 1 ? "," : "") << nl;
        }
        oss << indent(1) << "]" << nl;
        oss << "}";

        return oss.str();
    }

    std::string JsonExporter::export_indicators(const Indicators& indicators) const {
        std::ostringstream oss;
        const std::string nl = config_.pretty_print ? "\n" : "";
        const std::string sp = config_.pretty_print ? " " : "";

        oss << "{" << nl;

        oss << indent(1) << "\"sma_short\":" << sp << "[";
        const auto& sma_s = indicators.sma_short()->values();
        for (size_t i = 0; i < sma_s.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << sma_s[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"sma_medium\":" << sp << "[";
        const auto& sma_m = indicators.sma_medium()->values();
        for (size_t i = 0; i < sma_m.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << sma_m[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"sma_long\":" << sp << "[";
        const auto& sma_l = indicators.sma_long()->values();
        for (size_t i = 0; i < sma_l.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << sma_l[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"rsi\":" << sp << "[";
        const auto& rsi = indicators.rsi()->values();
        for (size_t i = 0; i < rsi.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << rsi[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"bollinger_middle\":" << sp << "[";
        const auto& bb_m = indicators.bollinger()->middle_band();
        for (size_t i = 0; i < bb_m.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << bb_m[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"bollinger_upper\":" << sp << "[";
        const auto& bb_u = indicators.bollinger()->upper_band();
        for (size_t i = 0; i < bb_u.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << bb_u[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"bollinger_lower\":" << sp << "[";
        const auto& bb_l = indicators.bollinger()->lower_band();
        for (size_t i = 0; i < bb_l.size(); i++) {
            if (i > 0) oss << "," << sp;
            oss << bb_l[i];
        }
        oss << "]," << nl;

        oss << indent(1) << "\"bullish_percent\":" << sp << indicators.bullish_percent()->value() << "," << nl;

        oss << indent(1) << "\"current_signal\":" << sp << "\""
            << (indicators.signals()->has_buy_signal() ? "buy" :
                indicators.signals()->has_sell_signal() ? "sell" : "none") << "\"," << nl;

        oss << indent(1) << "\"pattern_count\":" << sp << indicators.patterns()->pattern_count() << "," << nl;
        oss << indent(1) << "\"bullish_patterns\":" << sp << indicators.patterns()->bullish_count() << "," << nl;
        oss << indent(1) << "\"bearish_patterns\":" << sp << indicators.patterns()->bearish_count() << nl;

        oss << "}";
        return oss.str();
    }

    std::string JsonExporter::export_chart_with_indicators(const Chart& chart, const Indicators& indicators) const {
        std::ostringstream oss;
        std::string nl = config_.pretty_print ? "\n" : "";
        std::string sp = config_.pretty_print ? " " : "";

        oss << "{" << nl;
        oss << indent(1) << "\"chart\":" << sp;

        std::string chart_json = export_chart(chart);
        if (config_.pretty_print) {
            std::istringstream iss(chart_json);
            std::string line;
            bool first = true;
            while (std::getline(iss, line)) {
                if (!first) oss << nl << indent(1);
                oss << line;
                first = false;
            }
        } else {
            oss << chart_json;
        }

        oss << "," << nl;
        oss << indent(1) << "\"indicators\":" << sp;

        std::string ind_json = export_indicators(indicators);
        if (config_.pretty_print) {
            std::istringstream iss(ind_json);
            std::string line;
            bool first = true;
            while (std::getline(iss, line)) {
                if (!first) oss << nl << indent(1);
                oss << line;
                first = false;
            }
        } else {
            oss << ind_json;
        }

        oss << nl << "}";
        return oss.str();
    }

    std::string CsvExporter::export_columns(const Chart& chart) {
        std::ostringstream oss;
        oss << "index,type,high,low,box_count,marker\n";

        for (size_t i = 0; i < chart.column_count(); i++) {
            const Column* col = chart.column(i);
            std::string marker;
            if (col->box_count() > 0) {
                if (const Box* first_box = col->get_box_at(0)) marker = first_box->marker();
            }
            oss << i << ","
                << (col->type() == ColumnType::X ? "X" : "O") << ","
                << col->highest_price() << ","
                << col->lowest_price() << ","
                << col->box_count() << ","
                << marker << "\n";
        }

        return oss.str();
    }

    std::string CsvExporter::export_boxes(const Chart& chart)
    {
        std::ostringstream oss;
        oss << "column_index,box_index,type,price\n";

        for (size_t c = 0; c < chart.column_count(); c++) {
            const Column* col = chart.column(c);
            const char type = (col->type() == ColumnType::X) ? 'X' : 'O';

            for (size_t b = 0; b < col->box_count(); b++) {
                oss << c << "," << b << "," << type << ","
                    << col->get_box_at(b)->price() << "\n";
            }
        }

        return oss.str();
    }

    std::string CsvExporter::export_signals(const Indicators& indicators)
    {
        std::ostringstream oss;
        oss << "type,column_index,price\n";

        for (const auto& signal : indicators.signals()->signals()) {
            oss << (signal.type == SignalType::Buy ? "BUY" : "SELL") << ","
                << signal.column_index << ","
                << signal.price << "\n";
        }

        return oss.str();
    }

    std::string CsvExporter::export_patterns(const Indicators& indicators)
    {
        std::ostringstream oss;
        oss << "type,start_column,end_column,price\n";

        for (const auto& pattern : indicators.patterns()->patterns()) {
            oss << pattern_type_to_string(pattern.type) << ","
                << pattern.start_column << ","
                << pattern.end_column << ","
                << pattern.price << "\n";
        }

        return oss.str();
    }

    std::string CsvExporter::export_indicator_values(const Indicators& indicators)
    {
        std::ostringstream oss;
        oss << "index,sma_short,sma_medium,sma_long,rsi,bb_middle,bb_upper,bb_lower\n";

        const auto& sma_s = indicators.sma_short()->values();
        const auto& sma_m = indicators.sma_medium()->values();
        const auto& sma_l = indicators.sma_long()->values();
        const auto& rsi = indicators.rsi()->values();
        const auto& bb_m = indicators.bollinger()->middle_band();
        const auto& bb_u = indicators.bollinger()->upper_band();
        const auto& bb_l = indicators.bollinger()->lower_band();

        const size_t max_size = std::max({sma_s.size(), sma_m.size(), sma_l.size(),
                                    rsi.size(), bb_m.size(), bb_u.size(), bb_l.size()});

        for (size_t i = 0; i < max_size; i++) {
            oss << i << ","
                << (i < sma_s.size() ? sma_s[i] : 0) << ","
                << (i < sma_m.size() ? sma_m[i] : 0) << ","
                << (i < sma_l.size() ? sma_l[i] : 0) << ","
                << (i < rsi.size() ? rsi[i] : 0) << ","
                << (i < bb_m.size() ? bb_m[i] : 0) << ","
                << (i < bb_u.size() ? bb_u[i] : 0) << ","
                << (i < bb_l.size() ? bb_l[i] : 0) << "\n";
        }

        return oss.str();
    }

    std::string Visualization::to_ascii(const Chart& chart, const RenderConfig& config) {
        const AsciiRenderer renderer(config);
        return renderer.render(chart);
    }

    std::string Visualization::to_svg(const Chart& chart, const SvgConfig& config) {
        const SvgRenderer renderer(config);
        return renderer.render(chart);
    }

    std::string Visualization::to_json(const Chart& chart, const JsonConfig& config) {
        const JsonExporter exporter(config);
        return exporter.export_chart(chart);
    }

    std::string Visualization::to_csv_columns(const Chart& chart) {
        return CsvExporter::export_columns(chart);
    }

    std::string Visualization::to_csv_boxes(const Chart& chart) {
        return CsvExporter::export_boxes(chart);
    }

    std::string Visualization::to_ascii_with_indicators(const Chart& chart, const Indicators& indicators, const RenderConfig& config) {
        const AsciiRenderer renderer(config);
        return renderer.render_with_indicators(chart, indicators);
    }

    std::string Visualization::to_svg_with_indicators(const Chart& chart, const Indicators& indicators, const SvgConfig& config) {
        const SvgRenderer renderer(config);
        return renderer.render_with_indicators(chart, indicators);
    }

    std::string Visualization::to_json_with_indicators(const Chart& chart, const Indicators& indicators, const JsonConfig& config) {
        const JsonExporter exporter(config);
        return exporter.export_chart_with_indicators(chart, indicators);
    }
}