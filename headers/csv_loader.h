//
// Created by gregorian on 04/10/2025.
//

#pragma once
#include <string>
#include <vector>
#include <chrono>

namespace ChartSystem
{
    struct OHLCData
    {
        std::chrono::system_clock::time_point timestamp;
        double open{};
        double high{};
        double low{};
        double close{};
    };

    class CSVLoader
    {
    public:
        static std::vector<OHLCData> LoadFromCSV(const std::string& filename);
    private:
        static std::chrono::system_clock::time_point ParseDateTime(const std::string& dateStr);
    };
}
