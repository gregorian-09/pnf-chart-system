/// \file csv_loader.cpp
/// \brief Csv loader implementation.

//
// Created by gregorian-rayne on 12/01/2026.
//

#include "pnf/csv_loader.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace pnf {

    Timestamp CSVLoader::parse_datetime(const std::string& date_str, const std::string& format) {
        std::tm tm = {};
        std::istringstream ss(date_str);
        ss >> std::get_time(&tm, format.c_str());
        const std::time_t tt = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(tt);
    }

    std::vector<OHLC> CSVLoader::load(const std::string& filename) {
        namespace fs = std::filesystem;
        fs::path filePath{filename};

        if (filePath.is_relative()) {
            filePath = fs::absolute(filePath);
        }

        if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
            throw std::runtime_error("File does not exist or is not a file: " + filePath.string());
        }

        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath.string());
        }

        std::string line;
        std::getline(file, line);

        std::vector<OHLC> data;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::istringstream ss(line);
            std::string timestamp, date, open_str, high_str, low_str, close_str, vol_str;

            std::getline(ss, timestamp, ',');
            std::getline(ss, date, ',');
            std::getline(ss, open_str, ',');
            std::getline(ss, high_str, ',');
            std::getline(ss, low_str, ',');
            std::getline(ss, close_str, ',');
            std::getline(ss, vol_str, ',');

            OHLC ohlc;
            ohlc.time = parse_datetime(timestamp);
            ohlc.open = std::stod(open_str);
            ohlc.high = std::stod(high_str);
            ohlc.low = std::stod(low_str);
            ohlc.close = std::stod(close_str);
            ohlc.volume = vol_str.empty() ? 0.0 : std::stod(vol_str);

            data.push_back(ohlc);
        }

        return data;
    }

}