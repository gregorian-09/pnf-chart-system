/// \file csv_loader.hpp
/// \brief CSV loader for chart data.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef CSV_LOADER_HPP
#define CSV_LOADER_HPP

#include "types.hpp"
#include <string>
#include <vector>

namespace pnf {

    /**
     * @brief Utility class for loading OHLC data from CSV files.
     *
     * Provides static methods to read OHLC data and parse timestamps.
     */
    class CSVLoader {
    public:
        /**
         * @brief Loads OHLC data from a CSV file.
         *
         * The CSV file should contain columns for date/time, open, high, low, close, and optionally volume.
         *
         * @param filename Path to the CSV file
         * @return std::vector<OHLC> Vector of OHLC structures loaded from the file
         */
        static std::vector<OHLC> load(const std::string& filename);

        /**
         * @brief Parses a date/time string into a Timestamp.
         *
         * @param date_str Date/time string to parse
         * @param format Optional format string (default: "%Y.%m.%d %H:%M:%S")
         * @return Timestamp corresponding to the parsed date/time
         */
        static Timestamp parse_datetime(const std::string& date_str, const std::string& format = "%Y.%m.%d %H:%M:%S");
    };

} // namespace pnf


#endif //CSV_LOADER_HPP
