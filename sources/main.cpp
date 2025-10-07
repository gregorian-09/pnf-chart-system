#include "csv_loader.h"
#include "pnf_chart_indicators.h"
#include "../examples/example_usage.h"
#include <iostream>

int main()
{
    try
    {
        const std::string csvPath = "data/Boom_500_Index_PERIOD_H1.csv";
        const std::vector<ChartSystem::OHLCData> data = ChartSystem::CSVLoader::LoadFromCSV(csvPath);

        std::cout << "Loaded " << data.size() << " records from CSV\n";

        ChartSystem::PointAndFigureChart chart(ChartSystem::CLOSING_PRICE, ChartSystem::PERCENTAGE, 2, 3);

        for (const auto& candle : data)
        {
            chart.AddData(candle.high, candle.low, candle.close, candle.timestamp);
        }

        std::cout << "Chart created with " << chart.GetColumnCount() << " columns\n";
        std::cout << "X Columns: " << chart.GetXColumnCount() << "\n";
        std::cout << "O Columns: " << chart.GetOColumnCount() << "\n";
        std::cout << "Trend Bias: " << (chart.HasBullishBias() ? "BULLISH" :
                                        chart.HasBearishBias() ? "BEARISH" : "NONE") << "\n";

        if (chart.ExportToExcel("boom_500_pnf_chart.xlsx"))
            std::cout << "Chart exported to excels/boom_500_pnf_chart.xlsx\n";

        ChartSystem::PnFIndicators indicators;
        indicators.Calculate(&chart);

        ChartSystem::PnFChartVisualizer visualizer(&chart, &indicators);

        visualizer.SetShowMovingAverages(false);
        visualizer.SetShowBollingerBands(true);
        visualizer.Show();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    execute();

    return 0;
}