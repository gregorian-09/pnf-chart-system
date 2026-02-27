"""
Unit and Integration Tests for Python PnF Bindings

Run with: pytest test_pypnf.py -v
"""

import pytest
import sys
import os
import sysconfig
from pathlib import Path

project_root = Path(__file__).parent.parent.parent
override_dir = Path(os.environ["PNF_PYTHON_BUILD_DIR"]) if "PNF_PYTHON_BUILD_DIR" in os.environ else None

candidate_dirs = []
if override_dir:
    candidate_dirs.append(override_dir)
if sys.platform.startswith("linux"):
    candidate_dirs.append(project_root / "build-linux" / "python")
candidate_dirs.append(project_root / "build" / "python")

build_python_dir = next((d for d in candidate_dirs if d.exists()), None)

if build_python_dir is None:
    pytest.skip("Build directory not found in expected locations", allow_module_level=True)

ext_suffix = sysconfig.get_config_var("EXT_SUFFIX") or ".so"
module_files = list(build_python_dir.glob(f"pypnf*{ext_suffix}"))
if not module_files:
    pytest.skip(f"pypnf module not built in: {build_python_dir}", allow_module_level=True)

sys.path.insert(0, str(build_python_dir))

try:
    import pypnf
except ImportError as e:
    pytest.skip(f"pypnf module not built: {e}", allow_module_level=True)


class TestVersion:
    """Test version information functions"""

    def test_version_string_exists(self):
        """Version string should be non-empty"""
        v = pypnf.version()
        assert v is not None
        assert len(v) > 0

    def test_version_major(self):
        """Major version should be non-negative"""
        major = pypnf.version_major()
        assert major >= 0

    def test_version_minor(self):
        """Minor version should be non-negative"""
        minor = pypnf.version_minor()
        assert minor >= 0

    def test_version_patch(self):
        """Patch version should be non-negative"""
        patch = pypnf.version_patch()
        assert patch >= 0

    def test_version_format(self):
        """Version should be in major.minor.patch format"""
        v = pypnf.version()
        parts = v.split('.')
        assert len(parts) == 3


class TestEnums:
    """Test enum types are properly exposed"""

    def test_box_type_enum(self):
        """BoxType enum should have X and O values"""
        assert hasattr(pypnf, 'BoxType')
        assert pypnf.BoxType.X is not None
        assert pypnf.BoxType.O is not None

    def test_column_type_enum(self):
        """ColumnType enum should have X, O, and Mixed values"""
        assert hasattr(pypnf, 'ColumnType')
        assert pypnf.ColumnType.X is not None
        assert pypnf.ColumnType.O is not None
        assert pypnf.ColumnType.Mixed is not None

    def test_construction_method_enum(self):
        """ConstructionMethod should have Close and HighLow values"""
        assert hasattr(pypnf, 'ConstructionMethod')
        assert pypnf.ConstructionMethod.Close is not None
        assert pypnf.ConstructionMethod.HighLow is not None

    def test_box_size_method_enum(self):
        """BoxSizeMethod should have Fixed, Traditional, Percentage, Points"""
        assert hasattr(pypnf, 'BoxSizeMethod')
        assert pypnf.BoxSizeMethod.Fixed is not None
        assert pypnf.BoxSizeMethod.Traditional is not None
        assert pypnf.BoxSizeMethod.Percentage is not None
        assert pypnf.BoxSizeMethod.Points is not None

    def test_signal_type_enum(self):
        """SignalType should have None, Buy, Sell values"""
        assert hasattr(pypnf, 'SignalType')
        assert pypnf.SignalType.NONE is not None
        assert pypnf.SignalType.Buy is not None
        assert pypnf.SignalType.Sell is not None

    def test_pattern_type_enum(self):
        """PatternType should have various pattern values"""
        assert hasattr(pypnf, 'PatternType')
        assert pypnf.PatternType.NONE is not None
        assert pypnf.PatternType.DoubleTopBreakout is not None
        assert pypnf.PatternType.DoubleBottomBreakdown is not None
        assert pypnf.PatternType.TripleTopBreakout is not None
        assert pypnf.PatternType.BullishCatapult is not None
        assert pypnf.PatternType.BearTrap is not None


class TestChartConfig:
    """Test ChartConfig structure"""

    def test_create_default(self):
        """Should create ChartConfig with default values"""
        config = pypnf.ChartConfig()
        assert config is not None

    def test_config_fields(self):
        """ChartConfig should have accessible fields"""
        config = pypnf.ChartConfig()
        assert hasattr(config, 'method')
        assert hasattr(config, 'box_size_method')
        assert hasattr(config, 'box_size')
        assert hasattr(config, 'reversal')

    def test_config_modification(self):
        """Config fields should be modifiable"""
        config = pypnf.ChartConfig()
        config.method = pypnf.ConstructionMethod.HighLow
        config.box_size_method = pypnf.BoxSizeMethod.Fixed
        config.box_size = 2.0
        config.reversal = 2

        assert config.method == pypnf.ConstructionMethod.HighLow
        assert config.box_size == 2.0
        assert config.reversal == 2


class TestIndicatorConfig:
    """Test IndicatorConfig structure"""

    def test_create_default(self):
        """Should create IndicatorConfig with defaults"""
        config = pypnf.IndicatorConfig()
        assert config is not None

    def test_sma_periods(self):
        """SMA periods should be configurable"""
        config = pypnf.IndicatorConfig()
        config.sma_short_period = 3
        config.sma_medium_period = 7
        config.sma_long_period = 14

        assert config.sma_short_period == 3
        assert config.sma_medium_period == 7
        assert config.sma_long_period == 14

    def test_bollinger_params(self):
        """Bollinger params should be configurable"""
        config = pypnf.IndicatorConfig()
        config.bollinger_period = 15
        config.bollinger_std_devs = 2.5

        assert config.bollinger_period == 15
        assert config.bollinger_std_devs == 2.5

    def test_rsi_params(self):
        """RSI params should be configurable"""
        config = pypnf.IndicatorConfig()
        config.rsi_period = 10
        config.rsi_overbought = 80.0
        config.rsi_oversold = 20.0

        assert config.rsi_period == 10
        assert config.rsi_overbought == 80.0
        assert config.rsi_oversold == 20.0


class TestChart:
    """Test Chart class"""

    def test_create_default(self):
        """Should create empty chart"""
        chart = pypnf.Chart()
        assert chart is not None
        assert chart.column_count() == 0

    def test_create_with_config(self):
        """Should create chart with custom config"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3

        chart = pypnf.Chart(config)
        assert chart is not None
        assert chart.column_count() == 0

    def test_add_price_data(self):
        """Should add price data to chart"""
        chart = pypnf.Chart()
        from datetime import datetime
        now = datetime.now()

        # Add some price data
        chart.add_price(100.0, now)
        # Note: May not create column immediately depending on implementation

    def test_add_ohlc_data(self):
        """Should add OHLC data to chart"""
        chart = pypnf.Chart()
        from datetime import datetime
        base_time = datetime(2024, 1, 1)

        chart.add_data(101.0, 99.0, 100.0, base_time)
        # Result indicates if boxes were added

    def test_column_operations(self):
        """Test column access operations"""
        chart = pypnf.Chart()

        # Initially empty
        assert chart.column_count() == 0
        assert chart.x_column_count() == 0
        assert chart.o_column_count() == 0

    def test_clear(self):
        """Should clear chart data"""
        chart = pypnf.Chart()
        from datetime import datetime

        # Add data
        for i in range(10):
            chart.add_data(100.0 + i, 99.0 + i, 100.0 + i, datetime(2024, 1, i+1))

        # Clear
        chart.clear()
        assert chart.column_count() == 0

    def test_to_string(self):
        """Should convert chart to string"""
        chart = pypnf.Chart()
        s = str(chart)
        assert s is not None

    def test_len(self):
        """__len__ should return column count"""
        chart = pypnf.Chart()
        assert len(chart) == 0

    def test_to_ascii(self):
        """Should render ASCII representation"""
        chart = pypnf.Chart()
        ascii_str = chart.to_ascii()
        assert ascii_str is not None

    def test_to_json(self):
        """Should export to JSON"""
        chart = pypnf.Chart()
        json_str = chart.to_json()
        assert json_str is not None


class TestChartWithData:
    """Test Chart with actual price data"""

    @pytest.fixture
    def chart_with_data(self):
        """Create a chart with sample price data"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        from datetime import datetime, timedelta

        base_time = datetime(2024, 1, 1)
        prices = [100, 101, 102, 103, 104, 105, 106, 107,  # Up trend
                  104, 103, 102, 101, 100, 99, 98,  # Down trend
                  99, 100, 101, 102, 103, 104, 105, 106, 107, 108]  # Up again

        for i, price in enumerate(prices):
            chart.add_data(float(price) + 0.5, float(price) - 0.5, float(price),
                           base_time + timedelta(days=i))

        return chart

    def test_has_columns(self, chart_with_data):
        """Chart should have columns after data"""
        assert chart_with_data.column_count() > 0

    def test_column_access(self, chart_with_data):
        """Should access column properties"""
        if chart_with_data.column_count() > 0:
            col_type = chart_with_data.column_type(0)
            box_count = chart_with_data.column_box_count(0)
            high = chart_with_data.column_high(0)
            low = chart_with_data.column_low(0)
            assert col_type in [pypnf.ColumnType.X, pypnf.ColumnType.O, pypnf.ColumnType.Mixed]
            assert box_count > 0
            assert high >= low

    def test_last_column(self, chart_with_data):
        """Should access last column via index"""
        if chart_with_data.column_count() > 0:
            last_idx = chart_with_data.column_count() - 1
            col_type = chart_with_data.column_type(last_idx)
            assert col_type in [pypnf.ColumnType.X, pypnf.ColumnType.O, pypnf.ColumnType.Mixed]

    def test_all_prices(self, chart_with_data):
        """Should return all price levels"""
        prices = chart_with_data.all_prices()
        assert len(prices) > 0

    def test_box_size(self, chart_with_data):
        """Should return current box size"""
        box_size = chart_with_data.current_box_size()
        assert box_size > 0


class TestIndicators:
    """Test Indicators class"""

    def test_create_default(self):
        """Should create default indicators"""
        ind = pypnf.Indicators()
        assert ind is not None

    def test_create_with_config(self):
        """Should create with custom config"""
        config = pypnf.IndicatorConfig()
        config.sma_short_period = 3
        ind = pypnf.Indicators(config)
        assert ind is not None

    def test_configure(self):
        """Should accept configuration"""
        ind = pypnf.Indicators()
        config = pypnf.IndicatorConfig()
        config.rsi_period = 7
        ind.configure(config)

    def test_calculate_empty_chart(self):
        """Should handle empty chart"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)  # Should not crash

    def test_summary(self):
        """Should generate summary"""
        ind = pypnf.Indicators()
        summary = ind.summary()
        assert summary is not None


class TestIndicatorsWithData:
    """Test Indicators with chart data"""

    @pytest.fixture
    def indicators_with_data(self):
        """Create indicators calculated on sample data"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        from datetime import datetime, timedelta

        base_time = datetime(2024, 1, 1)
        prices = [100 + i * 0.5 + (i % 10) for i in range(50)]

        for i, price in enumerate(prices):
            chart.add_data(price + 1, price - 1, price,
                           base_time + timedelta(days=i))

        ind = pypnf.Indicators()
        ind.calculate(chart)

        return ind, chart

    def test_sma_access(self, indicators_with_data):
        """Should access SMA values"""
        ind, chart = indicators_with_data

        sma_short = ind.sma_short()
        assert sma_short is not None

    def test_bollinger_access(self, indicators_with_data):
        """Should access Bollinger Band values"""
        ind, chart = indicators_with_data

        bb = ind.bollinger()
        assert bb is not None

    def test_rsi_access(self, indicators_with_data):
        """Should access RSI values"""
        ind, chart = indicators_with_data

        rsi = ind.rsi()
        assert rsi is not None

    def test_signals_access(self, indicators_with_data):
        """Should access signal detector"""
        ind, chart = indicators_with_data

        signals = ind.signals()
        assert signals is not None

    def test_patterns_access(self, indicators_with_data):
        """Should access pattern recognizer"""
        ind, chart = indicators_with_data

        patterns = ind.patterns()
        assert patterns is not None

    def test_support_resistance_access(self, indicators_with_data):
        """Should access support/resistance"""
        ind, chart = indicators_with_data

        sr = ind.support_resistance()
        assert sr is not None

    def test_objectives_access(self, indicators_with_data):
        """Should access price objectives"""
        ind, chart = indicators_with_data

        obj = ind.objectives()
        assert obj is not None

    def test_congestion_access(self, indicators_with_data):
        """Should access congestion detector"""
        ind, chart = indicators_with_data

        cong = ind.congestion()
        assert cong is not None

    def test_export_data(self, indicators_with_data):
        """Should export indicator data"""
        ind, chart = indicators_with_data

        data = ind.export_data()
        assert data is not None


class TestMovingAverage:
    """Test MovingAverage indicator"""

    @pytest.fixture
    def chart_and_indicators(self):
        """Create chart and calculate indicators (return both to keep alive)"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        for i in range(30):
            price = 100.0 + i * 0.5
            chart.add_data(price + 1, price - 1, price, base_time + timedelta(days=i))

        ind = pypnf.Indicators()
        ind.calculate(chart)

        return chart, ind

    def test_period(self, chart_and_indicators):
        """Should return configured period"""
        chart, ind = chart_and_indicators
        sma = ind.sma_short()
        period = sma.period()
        assert period > 0

    def test_set_period(self, chart_and_indicators):
        """Should allow setting period"""
        chart, ind = chart_and_indicators
        sma = ind.sma_short()
        sma.set_period(10)
        assert sma.period() == 10

    def test_values(self, chart_and_indicators):
        """Should return SMA values"""
        chart, ind = chart_and_indicators
        sma = ind.sma_short()
        sma.values_copy()

    def test_values_copy(self, chart_and_indicators):
        """Should return copy of values"""
        chart, ind = chart_and_indicators
        sma = ind.sma_short()
        values = sma.values_copy()
        assert isinstance(values, list)


class TestBollingerBands:
    """Test BollingerBands indicator"""

    @pytest.fixture
    def chart_and_indicators(self):
        """Create chart and calculate indicators (return both to keep alive)"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        for i in range(50):
            price = 100.0 + i * 0.3 + (i % 5) * 0.2
            chart.add_data(price + 1, price - 1, price, base_time + timedelta(days=i))

        ind = pypnf.Indicators()
        ind.calculate(chart)

        return chart, ind

    def test_period(self, chart_and_indicators):
        """Should return configured period"""
        chart, ind = chart_and_indicators
        bb = ind.bollinger()
        period = bb.period()
        assert period > 0

    def test_std_devs(self, chart_and_indicators):
        """Should return std deviation multiplier"""
        chart, ind = chart_and_indicators
        bb = ind.bollinger()
        std = bb.std_devs()
        assert std > 0

    def test_band_values(self, chart_and_indicators):
        """Should access band values"""
        chart, ind = chart_and_indicators
        bb = ind.bollinger()

        bb.middle_copy()
        bb.upper_copy()
        bb.lower_copy()


class TestRSI:
    """Test RSI indicator"""

    @pytest.fixture
    def chart_and_indicators(self):
        """Create chart and calculate indicators (return both to keep alive)"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        for i in range(50):
            # Create oscillating prices for RSI calculation
            price = 100.0 + (i % 10) - 5
            chart.add_data(price + 1, price - 1, price, base_time + timedelta(days=i))

        ind = pypnf.Indicators()
        ind.calculate(chart)

        return chart, ind

    def test_period(self, chart_and_indicators):
        """Should return configured period"""
        chart, ind = chart_and_indicators
        rsi = ind.rsi()
        period = rsi.period()
        assert period > 0

    def test_thresholds(self, chart_and_indicators):
        """Should return overbought/oversold thresholds"""
        chart, ind = chart_and_indicators
        rsi = ind.rsi()

        overbought = rsi.overbought_threshold()
        oversold = rsi.oversold_threshold()

        assert overbought > oversold

    def test_values(self, chart_and_indicators):
        """Should access RSI values"""
        chart, ind = chart_and_indicators
        rsi = ind.rsi()
        rsi.values_copy()


class TestSignalDetector:
    """Test SignalDetector"""

    def test_current_signal(self):
        """Should return current signal type"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        signals = ind.signals()
        current = signals.current_signal()
        assert current in [pypnf.SignalType.NONE, pypnf.SignalType.Buy, pypnf.SignalType.Sell]

    def test_signal_counts(self):
        """Should track buy/sell counts"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        signals = ind.signals()
        buy_count = signals.buy_count()
        sell_count = signals.sell_count()

        assert buy_count >= 0
        assert sell_count >= 0


class TestPatternRecognizer:
    """Test PatternRecognizer"""

    def test_pattern_count(self):
        """Should return pattern count"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        patterns = ind.patterns()
        count = patterns.pattern_count()
        assert count >= 0

    def test_bullish_bearish_counts(self):
        """Should track bullish/bearish pattern counts"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        patterns = ind.patterns()
        bullish = patterns.bullish_count()
        bearish = patterns.bearish_count()

        assert bullish >= 0
        assert bearish >= 0


class TestSupportResistance:
    """Test SupportResistance detector"""

    def test_levels_access(self):
        """Should access support and resistance levels"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        sr = ind.support_resistance()
        sr.support_levels()
        sr.resistance_levels()

    def test_threshold(self):
        """Should access and modify threshold"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        sr = ind.support_resistance()
        sr.threshold()
        sr.set_threshold(0.02)


class TestCongestionDetector:
    """Test CongestionDetector"""

    def test_zones(self):
        """Should access congestion zones"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        cong = ind.congestion()
        zones = cong.zones_copy()
        assert isinstance(zones, list)

    def test_min_columns(self):
        """Should access min columns setting"""
        chart = pypnf.Chart()
        ind = pypnf.Indicators()
        ind.calculate(chart)

        cong = ind.congestion()
        min_cols = cong.min_columns()
        assert min_cols > 0


class TestVisualization:
    """Test Visualization static methods"""

    def test_to_ascii(self):
        """Should render ASCII"""
        chart = pypnf.Chart()
        ascii_str = pypnf.Visualization.to_ascii(chart)
        assert ascii_str is not None

    def test_to_json(self):
        """Should export JSON"""
        chart = pypnf.Chart()
        json_str = pypnf.Visualization.to_json(chart)
        assert json_str is not None

    def test_to_csv_columns(self):
        """Should export columns as CSV"""
        chart = pypnf.Chart()
        csv_str = pypnf.Visualization.to_csv_columns(chart)
        assert csv_str is not None

    def test_to_csv_boxes(self):
        """Should export boxes as CSV"""
        chart = pypnf.Chart()
        csv_str = pypnf.Visualization.to_csv_boxes(chart)
        assert csv_str is not None


class TestIntegration:
    """Integration tests covering full workflows"""

    def test_full_analysis_workflow(self):
        """Test complete P&F analysis workflow"""
        # Configure chart
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed
        config.method = pypnf.ConstructionMethod.HighLow

        chart = pypnf.Chart(config)

        # Add realistic price data
        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        prices = [
            100, 101, 102, 103, 104, 105,  # Up trend
            104, 103, 102, 101, 100,       # Down trend
            101, 102, 103, 104, 105, 106, 107,  # Up trend
            106, 105, 104, 103,            # Small pullback
            104, 105, 106, 107, 108, 109, 110  # Continue up
        ]

        for i, price in enumerate(prices):
            chart.add_data(float(price) + 0.5, float(price) - 0.5, float(price),
                           base_time + timedelta(days=i))

        # Configure indicators
        ind_config = pypnf.IndicatorConfig()
        ind_config.sma_short_period = 3
        ind_config.sma_medium_period = 5
        ind_config.sma_long_period = 10
        ind_config.rsi_period = 7

        # Calculate indicators
        ind = pypnf.Indicators(ind_config)
        ind.calculate(chart)

        # Access all indicator components
        ind.sma_short()
        ind.bollinger()
        ind.rsi()
        ind.signals()
        ind.patterns()
        ind.support_resistance()
        ind.objectives()
        ind.congestion()

        # Generate outputs
        ascii_chart = chart.to_ascii()
        json_data = chart.to_json()
        summary = ind.summary()

        # Export data
        ind.export_data()
        pypnf.Indicators.export_chart_data(chart)

        # Verify outputs
        assert len(ascii_chart) > 0
        assert len(json_data) > 0
        assert len(summary) > 0

    def test_multiple_charts_concurrent(self):
        """Test creating and using multiple charts"""
        charts = []

        for i in range(5):
            config = pypnf.ChartConfig()
            config.box_size = float(i + 1)
            config.reversal = 3
            config.box_size_method = pypnf.BoxSizeMethod.Fixed

            chart = pypnf.Chart(config)
            charts.append(chart)

        # Add different data to each
        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        for idx, chart in enumerate(charts):
            for i in range(20):
                price = 100.0 + idx * 10 + i * 0.5
                chart.add_data(price + 1, price - 1, price,
                               base_time + timedelta(days=i))

        # Calculate indicators for each
        indicators = []
        for chart in charts:
            ind = pypnf.Indicators()
            ind.calculate(chart)
            indicators.append(ind)

        # Verify independence
        for i, (chart, ind) in enumerate(zip(charts, indicators)):
            assert chart is not None
            assert ind is not None

    def test_recalculate_after_data_change(self):
        """Test recalculating indicators after adding more data"""
        config = pypnf.ChartConfig()
        config.box_size = 1.0
        config.reversal = 3
        config.box_size_method = pypnf.BoxSizeMethod.Fixed

        chart = pypnf.Chart(config)
        ind = pypnf.Indicators()

        from datetime import datetime, timedelta
        base_time = datetime(2024, 1, 1)

        # Initial data
        for i in range(20):
            price = 100.0 + i * 0.5
            chart.add_data(price + 1, price - 1, price, base_time + timedelta(days=i))

        ind.calculate(chart)
        initial_column_count = chart.column_count()

        # Add more data
        for i in range(20, 40):
            price = 110.0 + i * 0.3
            chart.add_data(price + 1, price - 1, price, base_time + timedelta(days=i))

        # Recalculate
        ind.calculate(chart)
        new_column_count = chart.column_count()

        assert new_column_count >= initial_column_count


if __name__ == '__main__':
    pytest.main([__file__, '-v'])
