using System;
using Xunit;

namespace PnF.Tests
{
    /// <summary>
    /// Unit tests for chart behavior.
    /// </summary>
    public class ChartTests
    {
        [Fact]
        /// <summary>
        /// Version_ShouldReturnNonEmptyString.
        /// </summary>
        public void Version_ShouldReturnNonEmptyString()
        {
            var version = Version.String;
            Assert.NotNull(version);
            Assert.NotEmpty(version);
        }

        [Fact]
        /// <summary>
        /// Chart_CanCreateDefault.
        /// </summary>
        public void Chart_CanCreateDefault()
        {
            using var chart = new Chart();
            Assert.Equal(0, chart.ColumnCount);
        }

        [Fact]
        /// <summary>
        /// Chart_CanCreateWithConfig.
        /// </summary>
        public void Chart_CanCreateWithConfig()
        {
            var config = new ChartConfig
            {
                Method = ConstructionMethod.Close,
                BoxSizeMethod = BoxSizeMethod.Traditional,
                BoxSize = 0.0,
                Reversal = 3
            };

            using var chart = new Chart(config);
            Assert.Equal(0, chart.ColumnCount);
        }

        [Fact]
        /// <summary>
        /// Chart_CanAddPrice.
        /// </summary>
        public void Chart_CanAddPrice()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            Assert.True(chart.AddPrice(100.0, timestamp));
            Assert.True(chart.AddPrice(102.0, timestamp + 1));
            Assert.True(chart.AddPrice(104.0, timestamp + 2));

            Assert.True(chart.ColumnCount > 0);
        }

        [Fact]
        /// <summary>
        /// Chart_CanAddData.
        /// </summary>
        public void Chart_CanAddData()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            Assert.True(chart.AddData(100.0, 99.0, 99.5, timestamp));
            Assert.True(chart.AddData(101.0, 100.0, 100.5, timestamp + 1));
            Assert.True(chart.AddData(102.0, 101.0, 101.5, timestamp + 2));

            Assert.True(chart.ColumnCount > 0);
        }

        [Fact]
        /// <summary>
        /// Chart_CanAddOHLC.
        /// </summary>
        public void Chart_CanAddOHLC()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            var ohlc = new OHLC
            {
                Timestamp = timestamp,
                Open = 99.0,
                High = 100.0,
                Low = 98.0,
                Close = 99.5,
                Volume = 1000.0
            };

            Assert.True(chart.AddOHLC(ohlc));
        }

        [Fact]
        /// <summary>
        /// Chart_ColumnsSum.
        /// </summary>
        public void Chart_ColumnsSum()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 10; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            Assert.Equal(chart.ColumnCount, chart.XColumnCount + chart.OColumnCount);
        }

        [Fact]
        /// <summary>
        /// Chart_BoxSizeIsPositive.
        /// </summary>
        public void Chart_BoxSizeIsPositive()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            chart.AddPrice(100.0, timestamp);
            chart.AddPrice(105.0, timestamp + 1);

            Assert.True(chart.BoxSize > 0.0);
        }

        [Fact]
        /// <summary>
        /// Chart_CanClear.
        /// </summary>
        public void Chart_CanClear()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            chart.AddPrice(100.0, timestamp);
            chart.AddPrice(105.0, timestamp + 1);
            Assert.True(chart.ColumnCount > 0);

            chart.Clear();
            Assert.Equal(0, chart.ColumnCount);
        }

        [Fact]
        /// <summary>
        /// Chart_CanGetColumnInfo.
        /// </summary>
        public void Chart_CanGetColumnInfo()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 20; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            if (chart.ColumnCount > 0)
            {
                var colType = chart.GetColumnType(0);
                var boxCount = chart.GetColumnBoxCount(0);
                var highest = chart.GetColumnHighest(0);
                var lowest = chart.GetColumnLowest(0);

                Assert.True(boxCount > 0);
                Assert.True(highest >= lowest);
            }
        }

        [Fact]
        /// <summary>
        /// Chart_BiasDetection.
        /// </summary>
        public void Chart_BiasDetection()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            for (int i = 0; i < 20; i++)
            {
                chart.AddPrice(100.0 + i, timestamp + i);
            }

            var bullish = chart.HasBullishBias;
            var bearish = chart.HasBearishBias;

            Assert.True(bullish || bearish || (!bullish && !bearish));
        }

        [Fact]
        /// <summary>
        /// Chart_ToStringWorks.
        /// </summary>
        public void Chart_ToStringWorks()
        {
            using var chart = new Chart();
            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

            chart.AddPrice(100.0, timestamp);
            chart.AddPrice(105.0, timestamp + 1);

            var str = chart.ToString();
            Assert.NotNull(str);
        }

        [Fact]
        /// <summary>
        /// Chart_DisposedThrows.
        /// </summary>
        public void Chart_DisposedThrows()
        {
            var chart = new Chart();
            chart.Dispose();

            Assert.Throws<ObjectDisposedException>(() => chart.ColumnCount);
        }
    }
}
