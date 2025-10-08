# Point and Figure Chart System

A comprehensive C++ library for creating, analyzing, and visualizing Point and Figure (P&F) charts with automatic trend line detection, technical indicators, and pattern recognition.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Quick Start](#quick-start)
- [Documentation](#documentation)
- [Examples](#examples)
- [API Reference](#api-reference)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Features

### Core Functionality
- **Multiple Construction Types**
  - Closing price method
  - High-Low method for intraday data

- **Flexible Box Sizing**
  - Auto-calculated box sizes (Default)
  - Fixed box sizes
  - Percentage box calculation
  - Points box calculation (for example currency pairs like EURUSD which has 4 digits points)

- **Reversal Methods**
  - 1-box reversal
  - 3-box reversal (traditional)
  - Custom reversal counts

### Advanced Analysis
- **Automatic Trend Line Detection**
  - Bullish support lines
  - Bearish resistance lines
  - Touch count tracking
  - Break detection

- **Technical Indicators**
  - Point and Figure Moving Averages (SMA5, SMA10)
  - Bollinger Bands
  - Signal Detection (Buy/Sell)
  - Custom indicator support

- **Pattern Recognition** (21 Patterns)
  - Double/Triple/Quadruple Top Breakouts
  - Double/Triple/Quadruple Bottom Breakdowns
  - Bullish/Bearish Catapults
  - High/Low Poles
  - Triangle Patterns
  - Signal Reversals
  - And more...

- **Support & Resistance**
  - Automatic level identification
  - Touch count tracking
  - Proximity alerts

- **Price Objectives**
  - Vertical count method
  - Target price calculation

### Visualization & Export
- **Interactive Chart Visualization**
  - Real-time rendering with ImGui/ImPlot
  - Trend line overlay
  - Indicator visualization
  - Customizable display options

- **Data Export**
  - Excel format (.xlsx) via OpenXLSX
  - Full chart data preservation

### External Data Integration
- **Flexible Data Input**
  - Compatible with any market data API (brokers, data providers)
  - Works with standard OHLC (Open, High, Low, Close) format
  - Simply ensure your data source provides OHLC data compatible with the `AddData(double high, double low, double close, std::chrono::system_clock::time_point time)`, `AddData(double price, std::chrono::system_clock::time_point time)` methods

## Requirements

### Compiler
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)

### Dependencies (Included as Git Submodules)

All dependencies are managed as Git submodules in this project:

- **ImGui** - Immediate mode GUI library
- **ImPlot** - Plotting library for ImGui
- **GLFW** - Window and input handling
- **OpenXLSX** - Excel file reading/writing
- **GoogleTest** - Unit testing framework (in `test/` directory)

## Installation

### Clone with Submodules

```bash
# Clone the repository with all submodules
git clone --recursive https://github.com/gregorian-09/pnf-chart-system.git
cd pnf-chart-system

# If you already cloned without --recursive, initialize submodules:
git submodule update --init --recursive
```

### Build Instructions

#### Linux (Ubuntu/Debian)

```bash
# Install system dependencies (if needed)
sudo apt-get update
sudo apt-get install build-essential cmake xorg-dev

# Build the project
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Install (optional)
sudo make install
```

#### macOS

```bash
# Install CMake if not already installed
brew install cmake

# Build the project
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run tests
ctest --output-on-failure
```

#### Windows (Visual Studio)

```bash
# Using Visual Studio 2019 or later
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Or open the generated .sln file in Visual Studio
```

#### Windows (MinGW)

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make -j4
```

### CMakeLists.txt Structure

The project uses a well-organized CMake configuration that handles all submodules automatically:

```cmake
cmake_minimum_required(VERSION 3.31)
project(ChartSystem)

set(CMAKE_CXX_STANDARD 20)

# Disable unnecessary submodule features
set(OPENXLSX_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Find system packages
find_package(OpenGL REQUIRED)

# Add submodules
add_subdirectory(OpenXLSX)
add_subdirectory(glfw)
add_subdirectory(test)
add_subdirectory(examples)

# Core library (no visualization dependencies)
add_library(ChartSystemCore STATIC
    sources/pnf_column.cpp
    sources/pnf_chart.cpp
    sources/pnf_chart_indicators.cpp
    sources/csv_loader.cpp
)

target_include_directories(ChartSystemCore PUBLIC
    ${PROJECT_SOURCE_DIR}/headers
    ${PROJECT_SOURCE_DIR}/glfw/include
)

target_link_libraries(ChartSystemCore
    PUBLIC OpenXLSX::OpenXLSX
    PUBLIC glfw
)

# Main executable with visualization
add_executable(ChartSystem
    sources/main.cpp
    sources/pnf_chart_visualizer.cpp
    
    # ImGui sources
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    
    # ImPlot sources
    implot/implot.cpp
    implot/implot_items.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/headers
    ${PROJECT_SOURCE_DIR}/imgui
    ${PROJECT_SOURCE_DIR}/imgui/backends
    ${PROJECT_SOURCE_DIR}/implot
    ${PROJECT_SOURCE_DIR}/glfw/include
)

target_link_libraries(ChartSystem
    PRIVATE ChartSystemExamples
    PRIVATE ChartSystemCore
    PRIVATE OpenGL::GL
)
```

**Note:** ImGui and ImPlot are included directly in the main executable since they don't provide CMakeLists.txt files. This keeps the core library (ChartSystemCore) lightweight and independent of visualization dependencies.

## Project Structure

```
pnf-chart-system/
├── headers/
│   ├── pnf_chart.h
│   ├── pnf_column.h
│   ├── csv_loader.h
│   └── pnf_chart_indicators.h
├── sources/
│   ├── pnf_chart.cpp
│   ├── pnf_column.cpp
│   ├── pnf_chart_indicators.cpp
│   ├── csv_loader.cpp
│   ├── main.cpp
│   └── pnf_chart_visualizer.cpp
├── examples/
│   ├── CMakeList.txt
│   ├── example_usage.cpp
│   └── example_usage.h
├── test/
│   ├── googletest/          (submodule)
│   ├── test_main.cpp
│   ├── test_pnf_bollinger_bands.cpp
│   ├── test_pnf_box.cpp
│   ├── test_pnf_chart.cpp
│   ├── test_pnf_column.cpp
│   ├── test_pnf_moving_average.cpp
│   ├── test_pnf_pattern_recognizer.cpp
│   ├── test_pnf_price_objective.cpp
│   ├── test_pnf_signal_detector.cpp
│   ├── test_pnf_support_resistance.cpp
│   ├── test_pnf_trendline.cpp
│   └── CMakeLists.txt
├── imgui/                   (submodule)
├── implot/                  (submodule)
├── glfw/                    (submodule)
├── OpenXLSX/                (submodule)
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## Quick Start

### Basic Example

```cpp
#include "pnf_chart.h"
#include <chrono>

using namespace ChartSystem;

int main() {
    // Create a Point and Figure chart
    PointAndFigureChart chart(
        CLOSING_PRICE,  // Construction type
        FIXED,          // Box size type
        1.0,            // Box size
        3               // Reversal count
    );
    
    // Add price data
    auto now = std::chrono::system_clock::now();
    
    chart.AddData(100.0, now);
    chart.AddData(102.0, now + std::chrono::hours(1));
    chart.AddData(105.0, now + std::chrono::hours(2));
    chart.AddData(103.0, now + std::chrono::hours(3));
    
    // Analyze the chart
    std::cout << "Total columns: " << chart.GetColumnCount() << std::endl;
    std::cout << "X columns: " << chart.GetXColumnCount() << std::endl;
    std::cout << "O columns: " << chart.GetOColumnCount() << std::endl;
    
    // Check market bias
    if (chart.HasBullishBias()) {
        std::cout << "Market shows bullish bias" << std::endl;
    }
    
    return 0;
}
```

### Using External Market Data

The library is compatible with any market data source that provides OHLC data:

```cpp
#include "pnf_chart.h"

// Example: Using data from a broker API
void ProcessMarketData() {
    PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);
    
    // Fetch data from your broker API
    auto marketData = YourBrokerAPI::GetOHLCData("EURUSD", "1H");
    
    for (const auto& candle : marketData) {
        // AddData is compatible with standard OHLC format
        chart.AddData(
            candle.high,
            candle.low,
            candle.close,
            candle.timestamp
        );
    }
    
    // Analyze the chart
    // ...
}
```

### Compile and Run

```bash
# Build and run the main application
cd build
./ChartSystem

# The main.cpp calls the Execute() method from example_usage.cpp
```

## Documentation

### Construction Types

#### CLOSING_PRICE
Uses only closing prices to determine box additions. Best for end-of-day data.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);
chart.AddData(closePrice, timestamp);
```

#### HIGH_LOW
Uses both high and low prices to capture intraday volatility. Recommended for intraday analysis.

```cpp
PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);
chart.AddData(high, low, close, timestamp);
```

### Box Size Types

#### DEFAULT (Auto-calculated)
Automatically calculates appropriate box size based on price levels.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, DEFAULT, 0.0, 3);
```

#### FIXED
Uses a fixed box size regardless of price level.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, FIXED, 2.0, 3);
```

#### PERCENTAGE
Computes the chart using the percentage provided.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, PERCENTAGE, 2.0, 3);
```

#### POINTS
Construct the Point and Figure Chart using points specified

```cpp
PointAndFigureChart chart(CLOSING_PRICE, POINTS, 0.0020, 3);
```

### Using Indicators

```cpp
#include "pnf_chart_indicators.h"

// Create indicators
PnFIndicators indicators;

// Calculate all indicators
indicators.Calculate(&chart);

// Access individual indicators
PnFMovingAverage* sma5 = indicators.GetSMA5();
PnFBollingerBands* bb = indicators.GetBollingerBands();
PnFSignalDetector* signals = indicators.GetSignalDetector();
PnFPatternRecognizer* patterns = indicators.GetPatternRecognizer();
PnFSupportResistance* sr = indicators.GetSupportResistance();
PnFPriceObjective* objectives = indicators.GetPriceObjective();

// Check signals
if (signals->HasBuySignal()) {
    std::cout << "Buy signal detected!" << std::endl;
}

// Get detected patterns
auto allPatterns = patterns->GetAllPatterns();
for (const auto& pattern : allPatterns) {
    std::cout << PnFPatternRecognizer::PatternTypeToString(pattern.type) << std::endl;
}
```

### Visualization

```cpp
#include "pnf_chart.h"

// Compute all indicators
PnFIndicators indicators;
indicators.Calculate(&chart);

// Create visualizer with indicators
PnFChartVisualizer visualizer(&chart, &indicators);

// Enable indicators
visualizer.SetShowMovingAverages(true);
visualizer.SetShowBollingerBands(true);

// Display chart (opens interactive window)
visualizer.Show();
```

### Data Export

```cpp
// Export to Excel
if (chart.ExportToExcel("my_chart.xlsx")) {
    std::cout << "Chart exported successfully" << std::endl;
}

// Get all price data
std::vector<double> prices = chart.GetAllPrices();
```

## Examples

The `examples/example_usage.cpp` contains the `Execute()` method which is called from `main.cpp`, providing a complete demonstration of the library's capabilities.

### Running Examples

```bash
# Build the project
cd build
cmake ..
make

# Run the main application (executes example_usage.cpp)
./ChartSystem
```

## API Reference

### PointAndFigureChart Class

#### Constructor
```cpp
PointAndFigureChart(
    ConstructionType constructionType = CLOSING_PRICE,
    BoxSizeType boxSizeType = DEFAULT,
    double boxSize = 0.0,
    int reversalCount = 3
)
```

#### Key Methods

**Data Input**
- `bool AddData(double price, time_point time)` - Add single price
- `bool AddData(double high, double low, double close, time_point time)` - Add OHLC data

**Chart Analysis**
- `int GetColumnCount()` - Get total number of columns
- `PnFColumn* GetColumn(int index)` - Get specific column
- `PnFColumn* GetLastColumn()` - Get most recent column
- `int GetXColumnCount()` - Count of X (bullish) columns
- `int GetOColumnCount()` - Count of O (bearish) columns

**Trend Analysis**
- `bool HasBullishBias()` - Check if chart shows bullish bias
- `bool HasBearishBias()` - Check if chart shows bearish bias
- `bool ShouldTakeBullishSignals()` - Check if conditions favor long trades
- `bool ShouldTakeBearishSignals()` - Check if conditions favor short trades
- `PnFTrendLineManager* GetTrendLineManager()` - Access trend line manager

**Export**
- `bool ExportToExcel(const std::string& filename)` - Export chart to Excel
- `std::vector<double> GetAllPrices()` - Get all price data

### PnFIndicators Class

#### Key Methods
- `void Calculate(const PointAndFigureChart* chart)` - Calculate all indicators
- `PnFMovingAverage* GetSMA5()` - Get 5-period moving average
- `PnFMovingAverage* GetSMA10()` - Get 10-period moving average
- `PnFBollingerBands* GetBollingerBands()` - Get Bollinger Bands
- `PnFSignalDetector* GetSignalDetector()` - Get signal detector
- `PnFPatternRecognizer* GetPatternRecognizer()` - Get pattern recognizer
- `PnFSupportResistance* GetSupportResistance()` - Get S/R analyzer
- `PnFPriceObjective* GetPriceObjective()` - Get price objectives
- `std::string GetSummary()` - Get complete indicator summary

### Supported Patterns

- Double/Triple/Quadruple Top Breakout
- Double/Triple/Quadruple Bottom Breakdown
- Ascending Triple Top
- Descending Triple Bottom
- Bullish/Bearish Catapult
- Bullish/Bearish Signal Reversed
- Bullish/Bearish Triangle
- Long Tail Down
- High/Low Pole
- Bull/Bear Trap
- Spread Triple Top/Bottom

## Testing

The project uses Google Test for unit testing. All tests are defined in the `test/` directory and executed through `test_main.cpp`.
```bash
# Run all tests using CTest
cd build
ctest --output-on-failure

# Or run the test executable directly
./test/ChartSystemTests

# Run specific test suite
./test/ChartSystemTests --gtest_filter=ChartTest.*
./test/ChartSystemTests --gtest_filter=IndicatorTest.*
```

### Test Structure

```
test/
├── googletest/          (submodule)
├── test_main.cpp        (main test runner)
├── test_pnf_bollinger_bands.cpp
├── test_pnf_box.cpp
├── test_pnf_chart.cpp
├── test_pnf_column.cpp
├── test_pnf_moving_average.cpp
├── test_pnf_pattern_recognizer.cpp
├── test_pnf_price_objective.cpp
├── test_pnf_signal_detector.cpp
├── test_pnf_support_resistance.cpp
├── test_pnf_trendline.cpp
└── CMakeLists.txt
```

## Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Setup

```bash
# Clone with submodules
git clone --recursive https://github.com/gregorian-09/pnf-chart-system.git
cd pnf-chart-system

# Create a development build
mkdir build-dev && cd build-dev
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Run tests before committing
ctest --output-on-failure
```

### Code Style
- Follow C++20 standards
- Use meaningful variable names
- Add comments for complex logic
- Include unit tests for new features
- Run clang-format before committing

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Point and Figure charting methodology by Charles Dow
- Technical analysis concepts from Tom Dorsey
- **ImGui** by Omar Cornut
- **ImPlot** by Evan Pezent
- **GLFW** team
- **OpenXLSX** by Kenneth Troldal
- Community contributors and testers

## Contact

- **Author**: Gregorian Rayne
- **Email**: gregorianrayne09@gmail.com
- **GitHub**: [@gregorian-09](https://github.com/gregorian-09)
- **Issues**: [GitHub Issues](https://github.com/gregorian-09/pnf-chart-system/issues)

## Performance

- Handles 100,000+ data points efficiently
- Real-time processing capability
- Low memory footprint (~50MB typical usage)
- Optimized rendering at 60+ FPS

## Star History

If you find this project useful, please consider giving it a star!

[![Star History Chart](https://api.star-history.com/svg?repos=gregorian-09/pnf-chart-system&type=Date)](https://star-history.com/#gregorian-09/pnf-chart-system&Date)

---

**Made with ❤️ by the Point and Figure community**