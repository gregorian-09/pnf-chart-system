# Point and Figure Chart System

A comprehensive C++ library for creating, analyzing, and visualizing Point and Figure (P&F) charts with automatic trend line detection, technical indicators, and pattern recognition capabilities.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Core Concepts](#core-concepts)
- [API Reference](#api-reference)
- [Technical Indicators](#technical-indicators)
- [Pattern Recognition](#pattern-recognition)
- [Visualization](#visualization)
- [Data Integration](#data-integration)
- [Testing](#testing)
- [Examples](#examples)
- [License](#license)

## Features

### Core Chart Functionality

**Construction Methods**
- Closing Price Method: Uses closing prices for chart updates
- High-Low Method: Captures intraday volatility using high and low prices

**Box Size Configuration**
- Default (Auto-calculated): Automatically determines optimal box size based on price levels
- Fixed: User-defined fixed box size
- Percentage: Box size calculated as percentage of price
- Points: Box size specified in points (ideal for forex pairs with decimal precision)

**Reversal Configuration**
- Supports 1-box reversal for sensitive price action
- Traditional 3-box reversal
- Custom reversal counts for specialized strategies

### Advanced Analysis Features

**Automatic Trend Line Detection**
- Bullish support line identification and tracking
- Bearish resistance line identification and tracking
- Touch count monitoring for trend line validation
- Break detection and notification
- Real-time trend bias calculation

**Technical Indicators Suite**
- Simple Moving Averages (SMA-5, SMA-10)
- Bollinger Bands with configurable periods and standard deviations
- Buy and Sell signal detection
- Price objective calculation using vertical count method
- Support and resistance level identification

**Pattern Recognition System**

The library identifies 21 distinct Point and Figure patterns:

*Bullish Patterns*
- Double Top Breakout
- Triple Top Breakout
- Quadruple Top Breakout
- Ascending Triple Top
- Bullish Catapult
- Bullish Signal Reversed
- Bullish Triangle
- Low Pole
- Bear Trap
- Spread Triple Top

*Bearish Patterns*
- Double Bottom Breakdown
- Triple Bottom Breakdown
- Quadruple Bottom Breakdown
- Descending Triple Bottom
- Bearish Catapult
- Bearish Signal Reversed
- Bearish Triangle
- Long Tail Down
- High Pole
- Bull Trap
- Spread Triple Bottom

**Support and Resistance Analysis**
- Automatic level identification based on price touch points
- Touch count tracking for level significance
- Proximity alerts for current price relative to key levels
- Merging of similar levels to reduce noise

**Price Objectives**
- Vertical count method implementation
- Target price calculation for both bullish and bearish scenarios
- Distance to target tracking

### Visualization and Export

**Interactive Visualization**
- Real-time chart rendering using ImGui and ImPlot
- Trend line overlay with customizable display
- Technical indicator visualization
- Configurable display options for indicators

**Data Export**
- Excel format (.xlsx) export via OpenXLSX
- Complete chart data preservation
- Price history extraction

## Requirements

### Compiler Requirements
- C++20 compatible compiler
  - GCC 10 or later
  - Clang 10 or later
  - MSVC 2019 or later

### Dependencies

All dependencies are included as Git submodules:

- **ImGui**: Immediate mode graphical user interface library
- **ImPlot**: Advanced plotting extension for ImGui
- **GLFW**: Multi-platform library for window creation and input handling
- **OpenXLSX**: Modern C++ library for Excel file operations
- **GoogleTest**: Unit testing framework

## Installation

### Clone with Submodules

```bash
# Clone repository with all dependencies
git clone --recursive https://github.com/gregorian-09/ChartSystem.git
cd ChartSystem

# If already cloned without --recursive flag
git submodule update --init --recursive
```

### Build Instructions

#### Linux (Ubuntu/Debian)

```bash
# Install system dependencies
sudo apt-get update
sudo apt-get install build-essential cmake xorg-dev

# Build project
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Optional: Install system-wide
sudo make install
```

#### macOS

```bash
# Install CMake if needed
brew install cmake

# Build project
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run tests
ctest --output-on-failure
```

#### Windows (Visual Studio)

```bash
# Generate Visual Studio solution
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Or open generated .sln file in Visual Studio
```

#### Windows (MinGW)

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make -j4
```

## Quick Start

### Basic Chart Creation

```cpp
#include "pnf_chart.h"
#include <chrono>

using namespace ChartSystem;

int main() {
    // Create Point and Figure chart
    PointAndFigureChart chart(
        CLOSING_PRICE,  // Construction type
        FIXED,          // Box size type
        1.0,            // Box size value
        3               // Reversal count
    );

    // Add price data
    auto now = std::chrono::system_clock::now();

    chart.AddData(100.0, now);
    chart.AddData(102.0, now + std::chrono::hours(1));
    chart.AddData(105.0, now + std::chrono::hours(2));
    chart.AddData(103.0, now + std::chrono::hours(3));

    // Analyze chart
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

### High-Low Construction

```cpp
PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);

// Add OHLC data
chart.AddData(
    high,       // High price
    low,        // Low price
    close,      // Close price
    timestamp   // Time point
);
```

## Core Concepts

### Box Size Types

#### DEFAULT (Auto-calculated)
Automatically determines appropriate box size based on price level. The algorithm adjusts box size as price moves to different ranges, ensuring optimal chart resolution.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, DEFAULT, 0.0, 3);
```

#### FIXED
Uses a constant box size regardless of price level. Best for assets with stable price ranges or when specific box sizes are required for strategy testing.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, FIXED, 2.0, 3);
```

#### PERCENTAGE
Calculates box size as a percentage of current price. Useful for assets with wide price ranges or for percentage-based analysis.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, PERCENTAGE, 2.0, 3);  // 2% box size
```

#### POINTS
Defines box size in decimal points. Essential for forex pairs and other instruments with decimal precision requirements.

```cpp
PointAndFigureChart chart(CLOSING_PRICE, POINTS, 0.0020, 3);  // 20 pips for forex
```

### Construction Types

#### CLOSING_PRICE
Updates chart based solely on closing prices. Recommended for end-of-day data and longer timeframes where intraday volatility is less critical.

#### HIGH_LOW
Processes both high and low prices within each bar. Captures intraday volatility and provides more responsive chart updates. Recommended for intraday analysis and real-time data feeds.

### Column Types

- **COLUMN_X**: Represents rising prices (bullish movement)
- **COLUMN_O**: Represents falling prices (bearish movement)
- **COLUMN_MIXED**: Contains both X and O boxes (used internally)

### Reversal Mechanics

A reversal occurs when price moves against the current column direction by the specified reversal count multiplied by box size. For example, with a 3-box reversal and 1.0 box size, price must move 3.0 points in the opposite direction to create a new column.

## API Reference

### PointAndFigureChart Class

#### Constructor

```cpp
PointAndFigureChart(
    ConstructionType constructionType = CLOSING_PRICE,
    BoxSizeType boxSizeType = DEFAULT,
    double boxSize = 0.0,
    int reversalCount = 3
);
```

**Parameters:**
- `constructionType`: Method for chart construction (CLOSING_PRICE or HIGH_LOW)
- `boxSizeType`: Box size calculation method (DEFAULT, FIXED, PERCENTAGE, or POINTS)
- `boxSize`: Box size value (0.0 for auto-calculation with DEFAULT)
- `reversalCount`: Number of boxes required for reversal (typically 1 or 3)

#### Data Input Methods

```cpp
bool AddData(double price, std::chrono::system_clock::time_point time);
```
Adds single price point to chart. Returns true if chart was updated.

```cpp
bool AddData(double high, double low, double close,
             std::chrono::system_clock::time_point time);
```
Adds OHLC data to chart. Processes high and low prices for comprehensive price action capture.

#### Chart Analysis Methods

```cpp
int GetColumnCount() const;
```
Returns total number of columns in chart.

```cpp
PnFColumn* GetColumn(int index) const;
```
Retrieves column at specified index. Returns nullptr if index is invalid.

```cpp
PnFColumn* GetLastColumn() const;
```
Returns pointer to most recent column.

```cpp
int GetXColumnCount() const;
int GetOColumnCount() const;
int GetMixedColumnCount() const;
```
Returns count of X columns, O columns, and mixed columns respectively.

```cpp
std::vector<int> GetXColumnIndices() const;
std::vector<int> GetOColumnIndices() const;
std::vector<int> GetMixedColumnIndices() const;
```
Returns indices of all columns of specified type.

#### Trend Analysis Methods

```cpp
PnFTrendLineManager* GetTrendLineManager() const;
```
Returns pointer to trend line manager for accessing trend line analysis.

```cpp
bool HasBullishBias() const;
bool HasBearishBias() const;
```
Indicates whether chart shows bullish or bearish bias based on trend line analysis.

```cpp
bool ShouldTakeBullishSignals() const;
bool ShouldTakeBearishSignals() const;
```
Determines if market conditions favor bullish or bearish signals.

```cpp
bool IsAboveBullishSupport(double price) const;
bool IsBelowBearishResistance(double price) const;
```
Checks if specified price is above bullish support or below bearish resistance.

#### Configuration Methods

```cpp
void SetConstructionType(ConstructionType constructionType);
void SetBoxSize(BoxSizeType boxSizeType);
void SetBoxSizeType(BoxSizeType boxSizeType);
void SetReversalCount(int reversalCount);
```
Modifies chart parameters. Note that changing parameters after data has been added may produce unexpected results.

```cpp
ConstructionType GetConstructionType() const;
BoxSizeType GetBoxSizeType() const;
double GetBoxSize() const;
int GetReversalCount() const;
```
Retrieves current chart configuration.

#### Export Methods

```cpp
bool ExportToExcel(const std::string& filename) const;
```
Exports complete chart data to Excel file. Returns true on success.

```cpp
std::vector<double> GetAllPrices() const;
```
Retrieves all price points from chart history.

```cpp
void Clear();
```
Removes all data from chart, resetting to initial state.

```cpp
std::string ToString() const;
```
Returns string representation of chart for debugging.

### PnFColumn Class

```cpp
int GetBoxCount() const;
```
Returns number of boxes in column.

```cpp
double GetHighestPrice() const;
double GetLowestPrice() const;
```
Returns highest and lowest prices in column.

```cpp
ColumnType GetColumnType() const;
```
Returns column type (COLUMN_X, COLUMN_O, or COLUMN_MIXED).

```cpp
PnFBox* GetBox(double price) const;
PnFBox* GetBoxAt(int index) const;
```
Retrieves box by price or index.

```cpp
bool HasBox(double price) const;
```
Checks if column contains box at specified price.

### PnFBox Class

```cpp
double GetPrice() const;
BoxType GetBoxType() const;
std::string GetMarker() const;
```
Retrieves box properties including price, type (X or O), and optional month marker.

### PnFTrendLine Class

```cpp
TrendLineType GetTrendLineType() const;
```
Returns BULLISH_SUPPORT or BEARISH_RESISTANCE.

```cpp
bool IsActive() const;
bool WasTouched() const;
int GetTouchCount() const;
```
Returns trend line status information.

```cpp
double GetPriceAtColumn(int columnIndex) const;
```
Calculates trend line price at specified column index.

```cpp
bool IsBroken(int columnIndex, double price) const;
```
Determines if trend line has been broken at specified column and price.

```cpp
TrendLinePoint GetStartPoint() const;
TrendLinePoint GetEndPoint() const;
```
Returns start and end points of trend line.

### PnFTrendLineManager Class

```cpp
PnFTrendLine* GetActiveTrendLine() const;
```
Returns pointer to currently active trend line.

```cpp
std::vector<std::unique_ptr<PnFTrendLine>>* GetAllTrendLines();
```
Returns pointer to vector containing all trend lines.

```cpp
bool IsAboveBullishSupport(int columnIndex, double price) const;
bool IsBelowBearishResistance(int columnIndex, double price) const;
```
Checks price position relative to trend lines at specified column.

```cpp
bool HasBullishBias() const;
bool HasBearishBias() const;
```
Indicates overall market bias based on trend line analysis.

```cpp
void Clear();
```
Removes all trend lines.

## Technical Indicators

### PnFIndicators Class

The `PnFIndicators` class serves as a container and manager for all technical indicators. It calculates and maintains all indicators simultaneously.

#### Usage

```cpp
#include "pnf_chart_indicators.h"

// Create indicators
PnFIndicators indicators;

// Calculate all indicators
indicators.Calculate(&chart);

// Access individual indicators
PnFMovingAverage* sma5 = indicators.GetSMA5();
PnFMovingAverage* sma10 = indicators.GetSMA10();
PnFBollingerBands* bb = indicators.GetBollingerBands();
PnFSignalDetector* signals = indicators.GetSignalDetector();
PnFPatternRecognizer* patterns = indicators.GetPatternRecognizer();
PnFSupportResistance* sr = indicators.GetSupportResistance();
PnFPriceObjective* objectives = indicators.GetPriceObjective();
```

#### Methods

```cpp
void Calculate(const PointAndFigureChart* chart) const;
```
Calculates all indicators based on current chart state. Must be called before accessing indicator values.

```cpp
PnFMovingAverage* GetSMA5() const;
PnFMovingAverage* GetSMA10() const;
```
Returns 5-period and 10-period simple moving averages.

```cpp
PnFBollingerBands* GetBollingerBands() const;
```
Returns Bollinger Bands indicator (20-period, 2 standard deviations by default).

```cpp
PnFSignalDetector* GetSignalDetector() const;
```
Returns signal detector for buy/sell signal identification.

```cpp
PnFPatternRecognizer* GetPatternRecognizer() const;
```
Returns pattern recognizer for chart pattern detection.

```cpp
PnFSupportResistance* GetSupportResistance() const;
```
Returns support and resistance analyzer.

```cpp
PnFPriceObjective* GetPriceObjective() const;
```
Returns price objective calculator.

```cpp
std::string GetSummary() const;
std::string ToString() const;
```
Returns formatted summary or detailed string representation of all indicators.

### PnFMovingAverage Class

Calculates simple moving average based on column average prices.

```cpp
PnFMovingAverage(int period);
void Calculate(const PointAndFigureChart* chart);
```

```cpp
double GetValue(int columnIndex) const;
bool HasValue(int columnIndex) const;
int GetPeriod() const;
std::vector<double> GetValues() const;
```

### PnFBollingerBands Class

Implements Bollinger Bands for Point and Figure charts.

```cpp
PnFBollingerBands(int period, double stdDev);
void Calculate(const PointAndFigureChart* chart);
```

```cpp
double GetMiddleBand(int columnIndex) const;
double GetUpperBand(int columnIndex) const;
double GetLowerBand(int columnIndex) const;
```

```cpp
bool IsAboveUpperBand(int columnIndex, double price) const;
bool IsBelowLowerBand(int columnIndex, double price) const;
bool HasValue(int columnIndex) const;
```

```cpp
std::vector<double> GetMiddleBandValues() const;
std::vector<double> GetUpperBandValues() const;
std::vector<double> GetLowerBandValues() const;
```

### PnFSignalDetector Class

Identifies buy and sell signals based on Point and Figure methodology.

```cpp
PnFSignalDetector();
void DetectSignals(const PointAndFigureChart* chart);
```

```cpp
SignalType GetCurrentSignal() const;
bool HasBuySignal() const;
bool HasSellSignal() const;
```

```cpp
const std::vector<Signal>& GetAllSignals() const;
Signal GetLastSignal() const;
```

**Signal Structure:**
```cpp
struct Signal {
    SignalType type;           // SIGNAL_BUY, SIGNAL_SELL, or SIGNAL_NONE
    int columnIndex;           // Column where signal occurred
    double price;              // Price level of signal
    std::chrono::system_clock::time_point timestamp;
};
```

### PnFPatternRecognizer Class

Detects and tracks Point and Figure chart patterns.

```cpp
PnFPatternRecognizer();
void DetectPatterns(const PointAndFigureChart* chart);
```

```cpp
const std::vector<Pattern>& GetAllPatterns() const;
std::vector<Pattern> GetBullishPatterns() const;
std::vector<Pattern> GetBearishPatterns() const;
Pattern GetLatestPattern() const;
```

```cpp
bool HasPattern(PatternType type) const;
static std::string PatternTypeToString(PatternType type);
```

**Pattern Structure:**
```cpp
struct Pattern {
    PatternType type;          // Pattern type identifier
    int startColumnIndex;      // First column of pattern
    int endColumnIndex;        // Last column of pattern
    double price;              // Significant price level for pattern
};
```

### PnFSupportResistance Class

Identifies and tracks support and resistance levels.

```cpp
PnFSupportResistance(double priceThreshold = 0.01);
void IdentifyLevels(const PointAndFigureChart* chart);
```

```cpp
std::vector<SupportResistanceLevel> GetSupportLevels() const;
std::vector<SupportResistanceLevel> GetResistanceLevels() const;
std::vector<SupportResistanceLevel> GetSignificantLevels(int minTouches = 3) const;
```

```cpp
bool IsNearSupport(double price, double tolerance = 0.02) const;
bool IsNearResistance(double price, double tolerance = 0.02) const;
```

**SupportResistanceLevel Structure:**
```cpp
struct SupportResistanceLevel {
    double price;              // Price level
    int touchCount;            // Number of times level was touched
    bool isSupport;            // true for support, false for resistance
    int firstColumnIndex;      // First column where level appeared
    int lastColumnIndex;       // Last column where level was touched
};
```

### PnFPriceObjective Class

Calculates price objectives using vertical count method.

```cpp
PnFPriceObjective();
void CalculateVerticalCount(const PointAndFigureChart* chart, int columnIndex);
```

```cpp
const std::vector<Objective>& GetObjectives() const;
Objective GetLatestObjective() const;
```

**Objective Structure:**
```cpp
struct Objective {
    double targetPrice;        // Calculated target price
    int baseColumnIndex;       // Column used for calculation
    int extensionBoxes;        // Number of boxes in extension
    bool isBullish;           // true for bullish, false for bearish objective
};
```

## Pattern Recognition

The pattern recognition system automatically identifies classical Point and Figure patterns as they form on the chart.

### Pattern Types

**PatternType Enumeration:**
- `PATTERN_NONE`
- `PATTERN_DOUBLE_TOP_BREAKOUT`
- `PATTERN_DOUBLE_BOTTOM_BREAKDOWN`
- `PATTERN_TRIPLE_TOP_BREAKOUT`
- `PATTERN_TRIPLE_BOTTOM_BREAKDOWN`
- `PATTERN_QUADRUPLE_TOP_BREAKOUT`
- `PATTERN_QUADRUPLE_BOTTOM_BREAKDOWN`
- `PATTERN_ASCENDING_TRIPLE_TOP`
- `PATTERN_DESCENDING_TRIPLE_BOTTOM`
- `PATTERN_BULLISH_CATAPULT`
- `PATTERN_BEARISH_CATAPULT`
- `PATTERN_BULLISH_SIGNAL_REVERSED`
- `PATTERN_BEARISH_SIGNAL_REVERSED`
- `PATTERN_BULLISH_TRIANGLE`
- `PATTERN_BEARISH_TRIANGLE`
- `PATTERN_LONG_TAIL_DOWN`
- `PATTERN_HIGH_POLE`
- `PATTERN_LOW_POLE`
- `PATTERN_BULL_TRAP`
- `PATTERN_BEAR_TRAP`
- `PATTERN_SPREAD_TRIPLE_TOP`
- `PATTERN_SPREAD_TRIPLE_BOTTOM`

### Usage Example

```cpp
PnFIndicators indicators;
indicators.Calculate(&chart);

PnFPatternRecognizer* patterns = indicators.GetPatternRecognizer();

// Check for specific pattern
if (patterns->HasPattern(PATTERN_DOUBLE_TOP_BREAKOUT)) {
    std::cout << "Double top breakout detected" << std::endl;
}

// Get all patterns
const std::vector<Pattern>& allPatterns = patterns->GetAllPatterns();
for (const auto& pattern : allPatterns) {
    std::cout << PnFPatternRecognizer::PatternTypeToString(pattern.type)
              << " at columns " << pattern.startColumnIndex
              << " to " << pattern.endColumnIndex << std::endl;
}

// Get only bullish patterns
std::vector<Pattern> bullishPatterns = patterns->GetBullishPatterns();
```

## Visualization

The `PnFChartVisualizer` class provides interactive visualization capabilities using ImGui and ImPlot.

### Basic Visualization

```cpp
#include "pnf_chart.h"

// Create chart and add data
PointAndFigureChart chart(CLOSING_PRICE, FIXED, 1.0, 3);
// ... add data ...

// Calculate indicators
PnFIndicators indicators;
indicators.Calculate(&chart);

// Create visualizer
PnFChartVisualizer visualizer(&chart, &indicators);

// Configure display options
visualizer.SetShowMovingAverages(true);
visualizer.SetShowBollingerBands(true);
visualizer.SetShowSupportResistance(true);

// Display interactive chart window
visualizer.Show();
```

### Display Options

```cpp
void SetShowMovingAverages(bool show);
```
Toggles display of moving averages on chart.

```cpp
void SetShowBollingerBands(bool show);
```
Toggles display of Bollinger Bands.

```cpp
void SetShowSupportResistance(bool show);
```
Toggles display of support and resistance levels.

The visualizer automatically displays:
- X and O columns
- Trend lines with touch counts
- Month markers
- Selected technical indicators

## Data Integration

### CSV Data Loading

The `CSVLoader` class provides convenient CSV data import functionality.

```cpp
#include "csv_loader.h"

// Load OHLC data from CSV
std::vector<OHLCData> data = CSVLoader::LoadFromCSV("market_data.csv");

// Add to chart
PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);
for (const auto& bar : data) {
    chart.AddData(bar.high, bar.low, bar.close, bar.timestamp);
}
```

**OHLCData Structure:**
```cpp
struct OHLCData {
    std::chrono::system_clock::time_point timestamp;
    double open;
    double high;
    double low;
    double close;
};
```

### External Data Source Integration

The library accepts data from any source that provides OHLC format or closing prices.

```cpp
// Example: Integration with external market data API
void ProcessMarketData() {
    PointAndFigureChart chart(HIGH_LOW, FIXED, 1.0, 3);

    // Fetch data from your data provider
    auto marketData = YourDataProvider::GetOHLC("EURUSD", "1H");

    for (const auto& candle : marketData) {
        chart.AddData(
            candle.high,
            candle.low,
            candle.close,
            candle.timestamp
        );
    }

    // Analyze results
    PnFIndicators indicators;
    indicators.Calculate(&chart);
    // ...
}
```

## Testing

The project uses Google Test for comprehensive unit testing.

### Running Tests

```bash
# Build and run all tests
cd build
ctest --output-on-failure

# Run test executable directly
./test/ChartSystemTests

# Run specific test suite
./test/ChartSystemTests --gtest_filter=ChartTest.*

# Run with verbose output
./test/ChartSystemTests --gtest_filter=* --gtest_print_time=1
```

### Test Coverage

The test suite includes comprehensive coverage of:

- Box creation and manipulation (`test_pnf_box.cpp`)
- Column operations (`test_pnf_column.cpp`)
- Chart construction and data processing (`test_pnf_chart.cpp`)
- Moving average calculations (`test_pnf_moving_average.cpp`)
- Bollinger Bands (`test_pnf_bollinger_bands.cpp`)
- Signal detection (`test_pnf_signal_detector.cpp`)
- Pattern recognition (`test_pnf_pattern_recognizer.cpp`)
- Support and resistance identification (`test_pnf_support_resistance.cpp`)
- Price objectives (`test_pnf_price_objective.cpp`)
- Trend line detection and tracking (`test_pnf_trendline.cpp`)

### Writing Tests

Tests follow Google Test conventions:

```cpp
#include <gtest/gtest.h>
#include "pnf_chart.h"

TEST(ChartTest, BasicConstruction) {
    ChartSystem::PointAndFigureChart chart(
        ChartSystem::CLOSING_PRICE,
        ChartSystem::FIXED,
        1.0,
        3
    );

    EXPECT_EQ(chart.GetColumnCount(), 0);
    EXPECT_EQ(chart.GetBoxSize(), 1.0);
}
```

## Examples

The `examples/example_usage.cpp` file contains 10 comprehensive examples demonstrating all library features.

### Running Examples

```bash
# Build project
cd build
cmake ..
make

# Run example application
./ChartSystem
```

### Example Topics

1. **Basic Chart Creation**: Fundamental chart creation and data addition
2. **High-Low Construction**: Using OHLC data for comprehensive chart updates
3. **Trend Line Analysis**: Working with automatic trend line detection
4. **Column Analysis**: Accessing and analyzing individual columns
5. **Technical Indicators**: Using the complete indicators suite
6. **Signal Detection**: Identifying buy and sell signals
7. **Pattern Recognition**: Detecting and working with chart patterns
8. **Support and Resistance**: Identifying key price levels
9. **Price Objectives**: Calculating target prices using vertical count
10. **Complete Summary**: Comprehensive analysis using all features

## Performance Characteristics

- Handles 100,000+ data points efficiently
- Real-time data processing capability
- Low memory footprint (approximately 50MB typical usage)
- Optimized rendering at 60+ frames per second
- Column-based architecture for efficient price level queries

## CMake Integration

### Using as Submodule

```cmake
add_subdirectory(ChartSystem)

target_link_libraries(YourProject
    PRIVATE ChartSystemCore
)
```

### Core Library (No Visualization)

The `ChartSystemCore` library provides all analysis capabilities without visualization dependencies:

```cmake
target_link_libraries(YourProject
    PRIVATE ChartSystemCore
)
```

This includes:
- Chart construction and analysis
- All technical indicators
- Pattern recognition
- Support and resistance analysis
- Excel export capabilities

## Contributing

Contributions are welcome. Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/enhancement`)
3. Write or update tests for new functionality
4. Ensure all tests pass (`ctest --output-on-failure`)
5. Follow existing code style and conventions
6. Commit changes with clear, descriptive messages
7. Push to branch (`git push origin feature/enhancement`)
8. Open a Pull Request with detailed description

### Code Standards

- Use C++20 features appropriately
- Follow RAII principles for resource management
- Prefer standard library containers and algorithms
- Use `const` correctness throughout
- Provide `[[nodiscard]]` attributes for getters
- Include comprehensive documentation for public APIs
- Write unit tests for new features
- Maintain backwards compatibility when possible

### Development Build

```bash
# Clone with submodules
git clone --recursive https://github.com/gregorian-09/ChartSystem.git
cd ChartSystem

# Create debug build
mkdir build-dev && cd build-dev
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Run tests
ctest --output-on-failure
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for complete details.

## Acknowledgments

- Point and Figure charting methodology developed by Charles Dow
- Technical analysis concepts based on work by Tom Dorsey
- **ImGui** by Omar Cornut
- **ImPlot** by Evan Pezent
- **GLFW** development team
- **OpenXLSX** by Kenneth Troldal
- Community contributors and testers

## Contact

**Author**: Gregorian Rayne
**Email**: gregorianrayne09@gmail.com
**GitHub**: [@gregorian-09](https://github.com/gregorian-09)
**Issues**: [GitHub Issues](https://github.com/gregorian-09/ChartSystem/issues)

## References

For deeper understanding of Point and Figure charting:

- Dorsey, Thomas J. "Point and Figure Charting: The Essential Application for Forecasting and Tracking Market Prices"
- Du Plessis, Jeremy. "The Definitive Guide to Point and Figure"
- Aby, Carroll D. "Point and Figure Charting: The Complete Guide"