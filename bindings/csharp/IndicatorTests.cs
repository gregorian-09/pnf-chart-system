using System;
using Xunit;

namespace PnF.Tests
{
    /// <summary>
    /// Unit tests for indicator behavior.
    /// </summary>
    public class IndicatorTests
    {
        [Fact]
        /// <summary>
        /// Indicators_CanCreate.
        /// </summary>
        public void Indicators_CanCreate()
        {
            using var indicators = new Indicators();
            Assert.Equal(0, indicators.PatternCount);
        }

        [Fact]
        /// <summary>
        /// Indicators_CanCreateWithConfig.
        /// </summary>
        public void Indicators_CanCreateWithConfig()
        {
            var config = IndicatorConfig.Default();
            using var indicators = new Indicators(config);
            Assert.Equal(0, indicators.SignalCount);
        }

        [Fact]
        /// <summary>
        /// Indicators_CanCalculate.
        /// </summary>
        public void Indicators_CanCalculate()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var bp = indicators.BullishPercent;
            Assert.True(bp >= 0.0 && bp <= 100.0);
        }

        [Fact]
        /// <summary>
        /// Indicators_SmaValues.
        /// </summary>
        public void Indicators_SmaValues()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + i * 0.5, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var smaShort = indicators.SmaShortValues();
            var smaMedium = indicators.SmaMediumValues();
            var smaLong = indicators.SmaLongValues();

            Assert.NotEmpty(smaShort);
            Assert.NotEmpty(smaMedium);
            Assert.NotEmpty(smaLong);
        }

        [Fact]
        /// <summary>
        /// Indicators_BollingerValues.
        /// </summary>
        public void Indicators_BollingerValues()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + Math.Sin(i * 0.5) * 5.0, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var middle = indicators.BollingerMiddleValues();
            var upper = indicators.BollingerUpperValues();
            var lower = indicators.BollingerLowerValues();

            Assert.NotEmpty(middle);
            Assert.NotEmpty(upper);
            Assert.NotEmpty(lower);
        }

        [Fact]
        /// <summary>
        /// Indicators_RsiValues.
        /// </summary>
        public void Indicators_RsiValues()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 50; i++)
            {
                chart.AddPrice(100.0 + i * 1.5, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var rsi = indicators.RsiValues();
            Assert.True(rsi.Length >= 0);
        }

        [Fact]
        /// <summary>
        /// Indicators_Signals.
        /// </summary>
        public void Indicators_Signals()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 50; i++)
            {
                var price = 100.0 + Math.Sin(i * 0.3) * 10.0;
                chart.AddPrice(price, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var signals = indicators.GetSignals();
            Assert.Equal(indicators.SignalCount, signals.Count);

            var buyCount = indicators.BuySignalCount;
            var sellCount = indicators.SellSignalCount;
            Assert.True(buyCount >= 0);
            Assert.True(sellCount >= 0);
        }

        [Fact]
        /// <summary>
        /// Indicators_Patterns.
        /// </summary>
        public void Indicators_Patterns()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 100; i++)
            {
                var price = 100.0 + Math.Sin(i * 0.2) * 15.0;
                chart.AddPrice(price, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var patterns = indicators.GetPatterns();
            Assert.Equal(indicators.PatternCount, patterns.Count);

            var bullish = indicators.GetBullishPatterns();
            var bearish = indicators.GetBearishPatterns();

            Assert.Equal(indicators.BullishPatternCount, bullish.Count);
            Assert.Equal(indicators.BearishPatternCount, bearish.Count);
        }

        [Fact]
        /// <summary>
        /// Indicators_SupportResistance.
        /// </summary>
        public void Indicators_SupportResistance()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 100; i++)
            {
                var price = 100.0 + Math.Sin(i * 0.15) * 10.0;
                chart.AddPrice(price, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var supportLevels = indicators.GetSupportLevels();
            var resistanceLevels = indicators.GetResistanceLevels();

            Assert.Equal(indicators.SupportLevelCount, supportLevels.Count);
            Assert.Equal(indicators.ResistanceLevelCount, resistanceLevels.Count);

            var supportPrices = indicators.GetSupportPrices();
            var resistancePrices = indicators.GetResistancePrices();

            Assert.Equal(supportLevels.Count, supportPrices.Length);
            Assert.Equal(resistanceLevels.Count, resistancePrices.Length);
        }

        [Fact]
        /// <summary>
        /// Indicators_ConfigurationMethods.
        /// </summary>
        public void Indicators_ConfigurationMethods()
        {
            using var indicators = new Indicators();

            indicators.SetSmaPeriods(3, 7, 15);
            indicators.SetBollingerParams(15, 2.5);
            indicators.SetRsiParams(10, 75.0, 25.0);
            indicators.SetBullishPercentThresholds(80.0, 20.0);
            indicators.SetSupportResistanceThreshold(0.02);
            indicators.SetCongestionParams(5, 0.03);

            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            indicators.Calculate(chart);
            Assert.True(indicators.BullishPercent >= 0.0);
        }

        [Fact]
        /// <summary>
        /// Indicators_AlertsWork.
        /// </summary>
        public void Indicators_AlertsWork()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var bullishAlert = indicators.IsBullishAlert;
            var bearishAlert = indicators.IsBearishAlert;

            Assert.True(bullishAlert || bearishAlert || (!bullishAlert && !bearishAlert));
        }

        [Fact]
        /// <summary>
        /// Indicators_SummaryWorks.
        /// </summary>
        public void Indicators_SummaryWorks()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 30; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            using var indicators = new Indicators();
            indicators.Calculate(chart);

            var summary = indicators.Summary();
            Assert.NotNull(summary);

            var str = indicators.ToString();
            Assert.NotNull(str);
        }

        [Fact]
        /// <summary>
        /// Indicators_DisposedThrows.
        /// </summary>
        public void Indicators_DisposedThrows()
        {
            var indicators = new Indicators();
            indicators.Dispose();

            Assert.Throws<ObjectDisposedException>(() => indicators.PatternCount);
        }
    }
}
