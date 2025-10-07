//
// Created by gregorian on 04/10/2025.
//

#include "csv_loader.h"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace ChartSystem
{
    std::chrono::system_clock::time_point CSVLoader::ParseDateTime(const std::string& dateStr)
    {
        std::tm tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, "%Y.%m.%d %H:%M:%S");

        const std::time_t tt = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(tt);
    }

    std::vector<OHLCData> CSVLoader::LoadFromCSV(const std::string& filename)
    {
        std::vector<OHLCData> data;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::string line;
        std::getline(file, line);

        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            std::istringstream ss(line);
            std::string timestamp, date, openStr, highStr, lowStr, closeStr;

            std::getline(ss, timestamp, ',');
            std::getline(ss, date, ',');
            std::getline(ss, openStr, ',');
            std::getline(ss, highStr, ',');
            std::getline(ss, lowStr, ',');
            std::getline(ss, closeStr, ',');

            OHLCData ohlc;
            ohlc.timestamp = ParseDateTime(timestamp);
            ohlc.open = std::stod(openStr);
            ohlc.high = std::stod(highStr);
            ohlc.low = std::stod(lowStr);
            ohlc.close = std::stod(closeStr);

            data.push_back(ohlc);
        }

        file.close();
        return data;
    }
}

