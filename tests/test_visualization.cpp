/// \file test_visualization.cpp
/// \brief Test visualization implementation.

//
// Created by gregorian-rayne on 15/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

class VisualizationTest : public ::testing::Test {
protected:
    void SetUp() override {
        chart = std::make_unique<Chart>();
        for (int i = 0; i < 30; i++) {
            const double base = 100.0 + (i % 8) * 0.5;
            chart->add_data(base + 1.0, base - 1.0, base,
                std::chrono::system_clock::from_time_t(i * 86400));
        }
        indicators = std::make_unique<Indicators>();
        indicators->calculate(*chart);
    }

    std::unique_ptr<Chart> chart;
    std::unique_ptr<Indicators> indicators;
};

TEST_F(VisualizationTest, AsciiRenderDefault) {
    std::string ascii = Visualization::to_ascii(*chart);
    EXPECT_FALSE(ascii.empty());
    EXPECT_NE(ascii.find('X'), std::string::npos);
}

TEST_F(VisualizationTest, AsciiRenderCustomConfig) {
    RenderConfig config;
    config.cell_width = 5;
    config.x_char = '*';
    config.o_char = 'o';
    config.show_price_axis = true;
    config.show_column_numbers = true;

    std::string ascii = Visualization::to_ascii(*chart, config);
    EXPECT_FALSE(ascii.empty());
}

TEST_F(VisualizationTest, AsciiRenderWithIndicators) {
    std::string ascii = Visualization::to_ascii_with_indicators(*chart, *indicators);
    EXPECT_FALSE(ascii.empty());
    EXPECT_NE(ascii.find("SIGNAL"), std::string::npos);
}

TEST_F(VisualizationTest, SvgRenderDefault) {
    std::string svg = Visualization::to_svg(*chart);
    EXPECT_FALSE(svg.empty());
    EXPECT_NE(svg.find("<svg"), std::string::npos);
    EXPECT_NE(svg.find("</svg>"), std::string::npos);
}

TEST_F(VisualizationTest, SvgRenderCustomConfig) {
    SvgConfig config;
    config.box_width = 30;
    config.box_height = 30;
    config.x_color = "#00ff00";
    config.o_color = "#ff0000";
    config.show_grid = true;

    std::string svg = Visualization::to_svg(*chart, config);
    EXPECT_FALSE(svg.empty());
    EXPECT_NE(svg.find("<svg"), std::string::npos);
}

TEST_F(VisualizationTest, SvgRenderWithIndicators) {
    SvgConfig config;
    config.show_support_resistance = true;

    std::string svg = Visualization::to_svg_with_indicators(*chart, *indicators, config);
    EXPECT_FALSE(svg.empty());
    EXPECT_NE(svg.find("<svg"), std::string::npos);
}

TEST_F(VisualizationTest, JsonExportDefault) {
    std::string json = Visualization::to_json(*chart);
    EXPECT_FALSE(json.empty());
    EXPECT_NE(json.find("box_size"), std::string::npos);
    EXPECT_NE(json.find("columns"), std::string::npos);
}

TEST_F(VisualizationTest, JsonExportCustomConfig) {
    JsonConfig config;
    config.pretty_print = false;
    config.include_boxes = true;

    std::string json = Visualization::to_json(*chart, config);
    EXPECT_FALSE(json.empty());
    EXPECT_NE(json.find("box_size"), std::string::npos);
}

TEST_F(VisualizationTest, JsonExportWithIndicators) {
    std::string json = Visualization::to_json_with_indicators(*chart, *indicators);
    EXPECT_FALSE(json.empty());
    EXPECT_NE(json.find("chart"), std::string::npos);
    EXPECT_NE(json.find("indicators"), std::string::npos);
}

TEST_F(VisualizationTest, CsvExportColumns) {
    std::string csv = Visualization::to_csv_columns(*chart);
    EXPECT_FALSE(csv.empty());
    EXPECT_NE(csv.find("index"), std::string::npos);
    EXPECT_NE(csv.find("type"), std::string::npos);
    EXPECT_NE(csv.find("high"), std::string::npos);
    EXPECT_NE(csv.find("low"), std::string::npos);
}

TEST_F(VisualizationTest, CsvExportBoxes) {
    std::string csv = Visualization::to_csv_boxes(*chart);
    EXPECT_FALSE(csv.empty());
    EXPECT_NE(csv.find("column_index"), std::string::npos);
    EXPECT_NE(csv.find("box_index"), std::string::npos);
    EXPECT_NE(csv.find("price"), std::string::npos);
}

TEST_F(VisualizationTest, AsciiRendererClass) {
    const AsciiRenderer renderer;
    const std::string output = renderer.render(*chart);
    EXPECT_FALSE(output.empty());
}

TEST_F(VisualizationTest, SvgRendererClass) {
    const SvgRenderer renderer;
    std::string output = renderer.render(*chart);
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("<svg"), std::string::npos);
}

TEST_F(VisualizationTest, JsonExporterClass) {
    const JsonExporter exporter;
    std::string output = exporter.export_chart(*chart);
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("box_size"), std::string::npos);
}

TEST_F(VisualizationTest, JsonExporterIndicators) {
    const JsonExporter exporter;
    std::string output = exporter.export_indicators(*indicators);
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("sma_short"), std::string::npos);
    EXPECT_NE(output.find("rsi"), std::string::npos);
}

TEST_F(VisualizationTest, CsvExporterSignals) {
    std::string output = CsvExporter::export_signals(*indicators);
    EXPECT_NE(output.find("type"), std::string::npos);
    EXPECT_NE(output.find("column_index"), std::string::npos);
}

TEST_F(VisualizationTest, CsvExporterPatterns) {
    std::string output = CsvExporter::export_signals(*indicators);
    EXPECT_NE(output.find("type"), std::string::npos);
}

TEST_F(VisualizationTest, EmptyChartHandling) {
    const Chart empty_chart;
    std::string ascii = Visualization::to_ascii(empty_chart);
    EXPECT_NE(ascii.find("Empty"), std::string::npos);

    const std::string svg = Visualization::to_svg(empty_chart);
    EXPECT_TRUE(svg.empty());
}

TEST_F(VisualizationTest, MaxColumnsConfig) {
    RenderConfig config;
    config.max_columns = 10;

    const std::string ascii = Visualization::to_ascii(*chart, config);
    EXPECT_FALSE(ascii.empty());
}