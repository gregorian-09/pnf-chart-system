#include <pnf/pnf.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

using namespace pnf;

struct OHLCBar {
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
};

std::vector<OHLCBar> loadCSV(const std::string& filename) {
    std::vector<OHLCBar> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        OHLCBar bar;

        std::getline(ss, bar.timestamp, ',');
        std::getline(ss, field, ',');
        std::getline(ss, field, ',');
        bar.open = std::stod(field);
        std::getline(ss, field, ',');
        bar.high = std::stod(field);
        std::getline(ss, field, ',');
        bar.low = std::stod(field);
        std::getline(ss, field, ',');
        bar.close = std::stod(field);

        data.push_back(bar);
    }

    std::cout << "Loaded " << data.size() << " bars from " << filename << std::endl;
    return data;
}

void processDataset(const std::string& csvFile, const std::string& outputPrefix) {
    std::cout << "\n========================================\n";
    std::cout << "Processing: " << csvFile << std::endl;
    std::cout << "========================================\n";

    auto data = loadCSV(csvFile);
    if (data.empty()) {
        std::cerr << "No data loaded, skipping..." << std::endl;
        return;
    }

    ChartConfig config;
    config.method = ConstructionMethod::HighLow;

    if (csvFile.find("GBPUSD") != std::string::npos || csvFile.find("EURUSD") != std::string::npos) {
        config.box_size_method = BoxSizeMethod::Points;
        config.box_size = 0.0020;
    } else {
        config.box_size_method = BoxSizeMethod::Traditional;
        config.box_size = 0.0;
    }
    config.reversal = 3;

    Chart chart(config);

    auto timestamp = std::chrono::system_clock::now();
    for (const auto& bar : data) {
        chart.add_data(bar.high, bar.low, bar.close, timestamp);
        timestamp += std::chrono::seconds(1);
    }

    std::cout << "Chart Statistics:" << std::endl;
    std::cout << "  Total columns: " << chart.columns().size() << std::endl;
    std::cout << "  Box size: " << chart.config().box_size << std::endl;

    IndicatorConfig indConfig;
    indConfig.sma_short_period = 5;
    indConfig.sma_medium_period = 10;
    indConfig.sma_long_period = 20;
    indConfig.bollinger_period = 20;
    indConfig.bollinger_std_devs = 2.0;
    indConfig.rsi_period = 14;
    indConfig.rsi_overbought = 70.0;
    indConfig.rsi_oversold = 30.0;

    Indicators indicators(indConfig);
    indicators.calculate(chart);

    std::cout << "\nIndicator calculation complete!" << std::endl;

    std::cout << "\nGenerating visualizations..." << std::endl;

    std::ofstream asciiFile(outputPrefix + "_ascii.txt");
    asciiFile << Visualization::to_ascii(chart);
    asciiFile.close();
    std::cout << "  ASCII chart saved to: " << outputPrefix << "_ascii.txt" << std::endl;

    std::ofstream jsonFile(outputPrefix + "_chart.json");
    jsonFile << Visualization::to_json(chart);
    jsonFile.close();
    std::cout << "  JSON chart saved to: " << outputPrefix << "_chart.json" << std::endl;

    std::ofstream jsonIndFile(outputPrefix + "_indicators.json");
    jsonIndFile << Visualization::to_json_with_indicators(chart, indicators);
    jsonIndFile.close();
    std::cout << "  JSON with indicators saved to: " << outputPrefix << "_indicators.json" << std::endl;

    std::ofstream csvColumnsFile(outputPrefix + "_columns.csv");
    csvColumnsFile << Visualization::to_csv_columns(chart);
    csvColumnsFile.close();
    std::cout << "  CSV columns saved to: " << outputPrefix << "_columns.csv" << std::endl;

    std::ofstream csvSignalsFile(outputPrefix + "_signals.csv");
    csvSignalsFile << CsvExporter::export_signals(indicators);
    csvSignalsFile.close();
    std::cout << "  CSV signals saved to: " << outputPrefix << "_signals.csv" << std::endl;

    std::ofstream csvPatternsFile(outputPrefix + "_patterns.csv");
    csvPatternsFile << CsvExporter::export_patterns(indicators);
    csvPatternsFile.close();
    std::cout << "  CSV patterns saved to: " << outputPrefix << "_patterns.csv" << std::endl;

    std::cout << "\nProcessing complete!" << std::endl;
}

int main() {
    std::cout << "PnF Chart Library - Real World Data Example (C++)" << std::endl;
    std::cout << "Library Version: " << Version::string << std::endl;

    processDataset("../../fixtures/GBPUSD_PERIOD_M1.csv", "output/gbpusd_m1");
    processDataset("../../fixtures/Boom_500_Index_PERIOD_H1.csv", "output/boom500_h1");
    processDataset("../../fixtures/Volatility_75_Index_PERIOD_D1.csv", "output/volatility75_d1");

    std::cout << "\n========================================\n";
    std::cout << "All datasets processed successfully!" << std::endl;
    std::cout << "========================================\n";

    return 0;
}
