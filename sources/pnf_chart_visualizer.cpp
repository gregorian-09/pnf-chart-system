//
// Created by gregorian on 04/10/2025.
//

#include "pnf_chart.h"
#include "pnf_chart_indicators.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <GLFW/glfw3.h>

ChartSystem::PnFChartVisualizer::PnFChartVisualizer(PointAndFigureChart* chart, PnFIndicators* indicators)
    : chart(chart), indicators(indicators), initialized(false),
      showMovingAverages(false), showBollingerBands(false), showSupportResistance(false)
{
}

ChartSystem::PnFChartVisualizer::~PnFChartVisualizer()
{
    Cleanup();
}

void ChartSystem::PnFChartVisualizer::Initialize()
{
    if (initialized) return;

    if (!glfwInit())
        return;

    window = glfwCreateWindow(1400, 800, "Point & Figure Chart", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    initialized = true;
}

void ChartSystem::PnFChartVisualizer::Cleanup()
{
    if (!initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    initialized = false;
}

void ChartSystem::PnFChartVisualizer::DrawTrendlines() const
{
    const PnFTrendLineManager* manager = chart->GetTrendLineManager();
    if (manager == nullptr) return;

    const int columnCount = chart->GetColumnCount();
    if (columnCount == 0) return;

    const std::vector<std::unique_ptr<PnFTrendLine>>* allTrendLines =
        const_cast<PnFTrendLineManager*>(manager)->GetAllTrendLines();

    if (allTrendLines == nullptr || allTrendLines->empty()) return;

    for (size_t idx = 0; idx < allTrendLines->size(); ++idx)
    {
        const auto& trendLine = (*allTrendLines)[idx];
        if (trendLine == nullptr) continue;

        const TrendLinePoint startPoint = trendLine->GetStartPoint();
        const int startColumn = startPoint.columnIndex;

        int endColumn;
        if (trendLine->IsActive())
        {
            endColumn = columnCount - 1;
        }
        else
        {
            endColumn = startColumn + 1;
            for (int col = startColumn + 1; col < columnCount; ++col)
            {
                const double priceAtCol = trendLine->GetPriceAtColumn(col);
                const PnFColumn* column = chart->GetColumn(col);
                if (column == nullptr) continue;

                if (trendLine->GetTrendLineType() == BULLISH_SUPPORT)
                {
                    if (column->GetLowestPrice() < priceAtCol - chart->GetBoxSize())
                    {
                        endColumn = col;
                        break;
                    }
                }
                else
                {
                    if (column->GetHighestPrice() > priceAtCol + chart->GetBoxSize())
                    {
                        endColumn = col;
                        break;
                    }
                }
                endColumn = col;
            }
        }

        if (endColumn < startColumn) continue;

        const int numPoints = endColumn - startColumn + 1;
        if (numPoints < 2) continue;

        std::vector<double> x_points;
        std::vector<double> y_points;
        x_points.reserve(numPoints);
        y_points.reserve(numPoints);

        for (int col = startColumn; col <= endColumn; ++col)
        {
            x_points.push_back(col);
            y_points.push_back(trendLine->GetPriceAtColumn(col));
        }

        std::string label;
        if (trendLine->GetTrendLineType() == BULLISH_SUPPORT)
        {
            if (trendLine->IsActive())
            {
                ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), 2.5f);
                label = "Active Bullish Support";
            }
            else
            {
                ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.5f, 0.0f, 0.4f), 1.5f);
                label = "Past Bullish Support " + std::to_string(idx);
            }
        }
        else
        {
            if (trendLine->IsActive())
            {
                ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), 2.5f);
                label = "Active Bearish Resistance";
            }
            else
            {
                ImPlot::SetNextLineStyle(ImVec4(0.5f, 0.0f, 0.0f, 0.4f), 1.5f);
                label = "Past Bearish Resistance " + std::to_string(idx);
            }
        }

        ImPlot::PlotLine(label.c_str(), x_points.data(), y_points.data(), static_cast<int>(x_points.size()));
    }
}

void ChartSystem::PnFChartVisualizer::RenderFrame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1400, 800), ImGuiCond_FirstUseEver);

    ImGui::Begin("Point & Figure Chart", nullptr);

    if (ImPlot::BeginPlot("P&F Chart", ImVec2(-1, -50)))
    {
        ImPlot::SetupAxes("Column", "Price");

        const int columnCount = chart->GetColumnCount();
        ImPlot::SetupAxisLimits(ImAxis_X1, -1, columnCount, ImGuiCond_Always);

        if (const std::vector<double> allPrices = chart->GetAllPrices(); !allPrices.empty())
        {
            const double minPrice = allPrices.back();
            const double maxPrice = allPrices.front();
            const double boxSize = chart->GetBoxSize();
            const double padding = boxSize * 5;
            ImPlot::SetupAxisLimits(ImAxis_Y1, minPrice - padding, maxPrice + padding, ImGuiCond_Always);
        }

        ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Linear);
        ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Linear);

        for (int col = 0; col < chart->GetColumnCount(); col++)
        {
            const PnFColumn* column = chart->GetColumn(col);
            if (!column) continue;

            for (int b = 0; b < column->GetBoxCount(); b++)
            {
                const PnFBox* box = column->GetBoxAt(b);
                if (!box) continue;

                auto x = static_cast<double>(col);
                double price = box->GetPrice();
                const BoxType type = box->GetBoxType();
                std::string marker = box->GetMarker();

                if (!marker.empty())
                {
                    if (chart->IsMonthMarker(marker))
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 15, ImVec4(0, 0.7, 1, 1));
                        ImPlot::PlotScatter("##monthmarker", &x, &price, 1);
                        ImPlot::PlotText(marker.c_str(), x, price);
                    }
                    else
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 15, ImVec4(1, 1, 0, 1));
                        ImPlot::PlotScatter("##marker", &x, &price, 1);
                        ImPlot::PlotText(marker.c_str(), x, price);
                    }
                }
                else if (type == X)
                {
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 12, ImVec4(0, 1, 0, 1), 2);
                    ImPlot::PlotScatter("##X", &x, &price, 1);
                }
                else if (type == O)
                {
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 12, ImVec4(1, 0, 0, 1), 2);
                    ImPlot::PlotScatter("##O", &x, &price, 1);
                }
            }
        }

        DrawTrendlines();
        DrawMovingAverages();
        DrawBollingerBands();
        DrawSupportResistance();

        ImPlot::EndPlot();
    }

    ImGui::Text("Total Columns: %d | X Columns: %d | O Columns: %d | Mixed: %d",
                chart->GetColumnCount(),
                chart->GetXColumnCount(),
                chart->GetOColumnCount(),
                chart->GetMixedColumnCount());

    ImGui::Text("Trend Bias: %s",
                chart->HasBullishBias() ? "BULLISH" :
                chart->HasBearishBias() ? "BEARISH" : "NONE");

    ImGui::Separator();
    ImGui::Text("Indicators:");
    ImGui::Checkbox("Show Moving Averages", const_cast<bool*>(&showMovingAverages));
    ImGui::SameLine();
    ImGui::Checkbox("Show Bollinger Bands", const_cast<bool*>(&showBollingerBands));
    ImGui::SameLine();
    ImGui::Checkbox("Show Support/Resistance", const_cast<bool*>(&showSupportResistance));

    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ChartSystem::PnFChartVisualizer::DrawMovingAverages() const
{
    if (!showMovingAverages || indicators == nullptr) return;

    const PnFMovingAverage* sma5 = indicators->GetSMA5();
    const PnFMovingAverage* sma10 = indicators->GetSMA10();

    const int columnCount = chart->GetColumnCount();
    if (columnCount == 0) return;

    if (sma5 != nullptr)
    {
        std::vector<double> x_points;
        std::vector<double> y_points;

        for (int col = 0; col < columnCount; ++col)
        {
            if (sma5->HasValue(col))
            {
                x_points.push_back(col);
                y_points.push_back(sma5->GetValue(col));
            }
        }

        if (!x_points.empty())
        {
            ImPlot::SetNextLineStyle(ImVec4(1.0f, 0.5f, 0.0f, 0.8f), 2.0f);
            ImPlot::PlotLine("SMA-5", x_points.data(), y_points.data(),
                           static_cast<int>(x_points.size()));
        }
    }

    if (sma10 != nullptr)
    {
        std::vector<double> x_points;
        std::vector<double> y_points;

        for (int col = 0; col < columnCount; ++col)
        {
            if (sma10->HasValue(col))
            {
                x_points.push_back(col);
                y_points.push_back(sma10->GetValue(col));
            }
        }

        if (!x_points.empty())
        {
            ImPlot::SetNextLineStyle(ImVec4(0.5f, 0.5f, 1.0f, 0.8f), 2.0f);
            ImPlot::PlotLine("SMA-10", x_points.data(), y_points.data(),
                           static_cast<int>(x_points.size()));
        }
    }
}

void ChartSystem::PnFChartVisualizer::DrawBollingerBands() const
{
    if (!showBollingerBands || indicators == nullptr) return;

    const PnFBollingerBands* bb = indicators->GetBollingerBands();
    if (bb == nullptr) return;

    const int columnCount = chart->GetColumnCount();
    if (columnCount == 0) return;

    std::vector<double> x_points;
    std::vector<double> upper_points;
    std::vector<double> middle_points;
    std::vector<double> lower_points;

    for (int col = 0; col < columnCount; ++col)
    {
        if (bb->HasValue(col))
        {
            x_points.push_back(col);
            upper_points.push_back(bb->GetUpperBand(col));
            middle_points.push_back(bb->GetMiddleBand(col));
            lower_points.push_back(bb->GetLowerBand(col));
        }
    }

    if (!x_points.empty())
    {
        ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.4f, 0.8f, 0.6f), 1.5f);
        ImPlot::PlotLine("BB Upper", x_points.data(), upper_points.data(),
                       static_cast<int>(x_points.size()));

        ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.4f, 0.8f, 0.8f), 2.0f);
        ImPlot::PlotLine("BB Middle", x_points.data(), middle_points.data(),
                       static_cast<int>(x_points.size()));

        ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.4f, 0.8f, 0.6f), 1.5f);
        ImPlot::PlotLine("BB Lower", x_points.data(), lower_points.data(),
                       static_cast<int>(x_points.size()));

        ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.4f, 0.8f, 0.15f));
        ImPlot::PlotShaded("BB Fill", x_points.data(), upper_points.data(),
                          lower_points.data(), static_cast<int>(x_points.size()));
    }
}

void ChartSystem::PnFChartVisualizer::DrawSupportResistance() const
{
    if (!showSupportResistance || indicators == nullptr) return;

    const PnFSupportResistance* sr = indicators->GetSupportResistance();
    if (sr == nullptr) return;

    if (const int columnCount = chart->GetColumnCount(); columnCount == 0) return;

    const std::vector<SupportResistanceLevel> supportLevels = sr->GetSupportLevels();
    const std::vector<SupportResistanceLevel> resistanceLevels = sr->GetResistanceLevels();

    for (size_t i = 0; i < supportLevels.size(); ++i)
    {
        const auto& level = supportLevels[i];

        std::vector x_points = {
            static_cast<double>(level.firstColumnIndex),
            static_cast<double>(level.lastColumnIndex)
        };
        std::vector y_points = {level.price, level.price};

        std::string label = "Support " + std::to_string(i);

        if (level.touchCount >= 3)
        {
            ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.8f, 0.0f, 0.9f), 2.5f);
        }
        else
        {
            ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.6f, 0.0f, 0.5f), 1.5f);
        }

        ImPlot::PlotLine(label.c_str(), x_points.data(), y_points.data(), 2);
    }

    for (size_t i = 0; i < resistanceLevels.size(); ++i)
    {
        const auto& level = resistanceLevels[i];

        std::vector x_points = {
            static_cast<double>(level.firstColumnIndex),
            static_cast<double>(level.lastColumnIndex)
        };
        std::vector y_points = {level.price, level.price};

        std::string label = "Resistance " + std::to_string(i);

        if (level.touchCount >= 3)
        {
            ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.0f, 0.0f, 0.9f), 2.5f);
        }
        else
        {
            ImPlot::SetNextLineStyle(ImVec4(0.6f, 0.0f, 0.0f, 0.5f), 1.5f);
        }

        ImPlot::PlotLine(label.c_str(), x_points.data(), y_points.data(), 2);
    }
}

void ChartSystem::PnFChartVisualizer::Show()
{
    Initialize();

    if (!initialized || !window)
        return;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        RenderFrame();
        glfwSwapBuffers(window);
    }

    Cleanup();
}