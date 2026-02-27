/// \file visualization.hpp
/// \brief Visualization/export APIs.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include "chart.hpp"
#include "indicators.hpp"
#include <string>

namespace pnf
{
    /**
     * @brief Configuration for ASCII chart rendering.
     */
    struct RenderConfig {
        int cell_width = 3;                 /**< Width of a single box */
        int cell_height = 1;                /**< Height of a single box */
        char x_char = 'X';                  /**< Character to render X boxes */
        char o_char = 'O';                  /**< Character to render O boxes */
        char empty_char = ' ';              /**< Character for empty cells */
        char grid_char = '.';               /**< Character for grid lines */
        bool show_price_axis = true;        /**< Show price axis */
        bool show_column_numbers = true;    /**< Show column numbers */
        bool show_trend_lines = true;       /**< Show trend lines */
        bool show_month_markers = true;     /**< Show month markers */
        int price_decimals = 2;             /**< Decimal precision for prices */
        int max_columns = 0;                /**< Maximum columns to render (0 = all) */
    };

    /**
     * @brief Configuration for SVG chart rendering.
     */
    struct SvgConfig {
        int box_width = 20;                 /**< Width of a single box */
        int box_height = 20;                /**< Height of a single box */
        int margin_left = 80;               /**< Left margin */
        int margin_right = 20;              /**< Right margin */
        int margin_top = 40;                /**< Top margin */
        int margin_bottom = 60;             /**< Bottom margin */
        std::string x_color = "#22c55e";   /**< Color for X boxes */
        std::string o_color = "#ef4444";   /**< Color for O boxes */
        std::string grid_color = "#e5e7eb";/**< Grid line color */
        std::string axis_color = "#374151";/**< Axis color */
        std::string trend_line_color = "#3b82f6"; /**< Trend line color */
        std::string support_color = "#22c55e";     /**< Support line color */
        std::string resistance_color = "#ef4444";  /**< Resistance line color */
        std::string font_family = "monospace";     /**< Font family */
        int font_size = 12;                         /**< Font size */
        bool show_grid = true;                      /**< Show grid */
        bool show_trend_lines = true;               /**< Show trend lines */
        bool show_support_resistance = false;       /**< Show support/resistance lines */
    };

    /**
     * @brief Configuration for JSON export.
     */
    struct JsonConfig {
        bool pretty_print = true;       /**< Enable pretty printing */
        int indent = 2;                 /**< Indentation spaces */
        bool include_boxes = true;      /**< Include box data */
        bool include_indicators = true; /**< Include indicator data */
        bool include_trend_lines = true;/**< Include trend lines */
    };

    /**
     * @brief Renders a chart as ASCII text.
     */
    class AsciiRenderer {
    public:
        explicit AsciiRenderer(const RenderConfig& config = {});

        std::string render(const Chart& chart) const;
        std::string render_with_indicators(const Chart& chart, const Indicators& indicators) const;

        void set_config(const RenderConfig& config) { config_ = config; }
        RenderConfig config() const { return config_; }

    private:
        std::string render_price_axis(double min_price, double max_price, double box_size) const;
        std::string render_column_header(size_t start, size_t count) const;

        RenderConfig config_; /**< ASCII render configuration */
    };

    /**
     * @brief Renders a chart as an SVG image.
     */
    class SvgRenderer {
    public:
        explicit SvgRenderer(const SvgConfig& config = {});

        std::string render(const Chart& chart) const;
        std::string render_with_indicators(const Chart& chart, const Indicators& indicators) const;

        void set_config(const SvgConfig& config) { config_ = config; }
        SvgConfig config() const { return config_; }

    private:
        static std::string render_header(int width, int height);
        std::string render_grid(int cols, int rows) const;
        std::string render_price_axis(int rows, double min_price, double box_size) const;
        std::string render_column(const Column* col, int col_index, double min_price, double box_size) const;
        std::string render_trend_lines(const Chart& chart, double box_size) const;
        std::string render_support_resistance(const Indicators& indicators, int cols, double box_size) const;
        static std::string render_footer();

        SvgConfig config_; /**< SVG render configuration */
    };

    /**
     * @brief Exports chart data to JSON.
     */
    class JsonExporter {
    public:
        explicit JsonExporter(const JsonConfig& config = {});

        std::string export_chart(const Chart& chart) const;
        std::string export_chart_with_indicators(const Chart& chart, const Indicators& indicators) const;
        std::string export_indicators(const Indicators& indicators) const;

        void set_config(const JsonConfig& config) { config_ = config; }
        JsonConfig config() const { return config_; }

    private:
        std::string indent(int level) const;
        static std::string escape_string(const std::string& s) ;

        JsonConfig config_; /**< JSON export configuration */
    };

    /**
     * @brief Exports chart and indicator data to CSV.
     */
    class CsvExporter {
    public:
        static std::string export_columns(const Chart& chart) ;
        static std::string export_boxes(const Chart& chart);
        static std::string export_signals(const Indicators& indicators);
        static std::string export_patterns(const Indicators& indicators);
        static std::string export_indicator_values(const Indicators& indicators);
    };

    /**
     * @brief High-level visualization utilities.
     *
     * Provides static methods to render charts in ASCII, SVG, JSON, or CSV.
     */
    class Visualization {
    public:
        static std::string to_ascii(const Chart& chart, const RenderConfig& config = {});
        static std::string to_svg(const Chart& chart, const SvgConfig& config = {});
        static std::string to_json(const Chart& chart, const JsonConfig& config = {});
        static std::string to_csv_columns(const Chart& chart);
        static std::string to_csv_boxes(const Chart& chart);

        static std::string to_ascii_with_indicators(const Chart& chart, const Indicators& indicators, const RenderConfig& config = {});
        static std::string to_svg_with_indicators(const Chart& chart, const Indicators& indicators, const SvgConfig& config = {});
        static std::string to_json_with_indicators(const Chart& chart, const Indicators& indicators, const JsonConfig& config = {});
    };
} // namespace pnf

#endif //VISUALIZATION_HPP
