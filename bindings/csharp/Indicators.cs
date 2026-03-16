using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PnF
{
    /// <summary>
    /// Managed wrapper around native indicator calculations.
    /// </summary>
    public class Indicators : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed;

        public Indicators()
        {
            _handle = NativeMethods.pnf_indicators_create_default();
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create indicators");
        }

        public Indicators(IndicatorConfig config)
        {
            _handle = NativeMethods.pnf_indicators_create_with_config(ref config);
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create indicators with config");
        }

        /// <summary>
        /// Configure.
        /// </summary>
        /// <param name="config"></param>
        public void Configure(IndicatorConfig config)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_configure(_handle, ref config);
        }

        /// <summary>
        /// SetSmaPeriods.
        /// </summary>
        /// <param name="shortPeriod"></param>
        /// <param name="mediumPeriod"></param>
        /// <param name="longPeriod"></param>
        public void SetSmaPeriods(int shortPeriod, int mediumPeriod, int longPeriod)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_sma_periods(_handle, shortPeriod, mediumPeriod, longPeriod);
        }

        /// <summary>
        /// SetBollingerParams.
        /// </summary>
        /// <param name="period"></param>
        /// <param name="stdDevs"></param>
        public void SetBollingerParams(int period, double stdDevs)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_bollinger_params(_handle, period, stdDevs);
        }

        /// <summary>
        /// SetRsiParams.
        /// </summary>
        /// <param name="period"></param>
        /// <param name="overbought"></param>
        /// <param name="oversold"></param>
        public void SetRsiParams(int period, double overbought, double oversold)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_rsi_params(_handle, period, overbought, oversold);
        }

        /// <summary>
        /// SetBullishPercentThresholds.
        /// </summary>
        /// <param name="bullish"></param>
        /// <param name="bearish"></param>
        public void SetBullishPercentThresholds(double bullish, double bearish)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_bullish_percent_thresholds(_handle, bullish, bearish);
        }

        /// <summary>
        /// SetSupportResistanceThreshold.
        /// </summary>
        /// <param name="threshold"></param>
        public void SetSupportResistanceThreshold(double threshold)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_support_resistance_threshold(_handle, threshold);
        }

        /// <summary>
        /// SetCongestionParams.
        /// </summary>
        /// <param name="minColumns"></param>
        /// <param name="priceRange"></param>
        public void SetCongestionParams(int minColumns, double priceRange)
        {
            ThrowIfDisposed();
            NativeMethods.pnf_indicators_set_congestion_params(_handle, minColumns, priceRange);
        }

        /// <summary>
        /// Calculate.
        /// </summary>
        /// <param name="chart"></param>
        public void Calculate(Chart chart)
        {
            ThrowIfDisposed();
            if (chart == null) throw new ArgumentNullException(nameof(chart));
            NativeMethods.pnf_indicators_calculate(_handle, chart.Handle);
        }

        /// <summary>
        /// SmaShort.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double SmaShort(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_sma_short(_handle, column);
        }

        /// <summary>
        /// SmaMedium.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double SmaMedium(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_sma_medium(_handle, column);
        }

        /// <summary>
        /// SmaLong.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double SmaLong(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_sma_long(_handle, column);
        }

        /// <summary>
        /// SmaShortValues.
        /// </summary>
        /// <returns></returns>
        public double[] SmaShortValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_sma_short_values(_handle));
        }

        /// <summary>
        /// SmaMediumValues.
        /// </summary>
        /// <returns></returns>
        public double[] SmaMediumValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_sma_medium_values(_handle));
        }

        /// <summary>
        /// SmaLongValues.
        /// </summary>
        /// <returns></returns>
        public double[] SmaLongValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_sma_long_values(_handle));
        }

        /// <summary>
        /// BollingerMiddle.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double BollingerMiddle(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_bollinger_middle(_handle, column);
        }

        /// <summary>
        /// BollingerUpper.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double BollingerUpper(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_bollinger_upper(_handle, column);
        }

        /// <summary>
        /// BollingerLower.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double BollingerLower(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_bollinger_lower(_handle, column);
        }

        /// <summary>
        /// BollingerMiddleValues.
        /// </summary>
        /// <returns></returns>
        public double[] BollingerMiddleValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_bollinger_middle_values(_handle));
        }

        /// <summary>
        /// BollingerUpperValues.
        /// </summary>
        /// <returns></returns>
        public double[] BollingerUpperValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_bollinger_upper_values(_handle));
        }

        /// <summary>
        /// BollingerLowerValues.
        /// </summary>
        /// <returns></returns>
        public double[] BollingerLowerValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_bollinger_lower_values(_handle));
        }

        /// <summary>
        /// Rsi.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double Rsi(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_rsi(_handle, column);
        }

        /// <summary>
        /// RsiIsOverbought.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public bool RsiIsOverbought(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_rsi_is_overbought(_handle, column);
        }

        /// <summary>
        /// RsiIsOversold.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public bool RsiIsOversold(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_rsi_is_oversold(_handle, column);
        }

        /// <summary>
        /// RsiValues.
        /// </summary>
        /// <returns></returns>
        public double[] RsiValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_rsi_values(_handle));
        }

        /// <summary>
        /// Obv.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public double Obv(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_obv(_handle, column);
        }

        /// <summary>
        /// ObvValues.
        /// </summary>
        /// <returns></returns>
        public double[] ObvValues()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_obv_values(_handle));
        }

        public double BullishPercent
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_bullish_percent(_handle);
            }
        }

        public bool IsBullishAlert
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_is_bullish_alert(_handle);
            }
        }

        public bool IsBearishAlert
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_is_bearish_alert(_handle);
            }
        }

        public SignalType CurrentSignal
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_current_signal(_handle);
            }
        }

        public int SignalCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_signal_count(_handle);
            }
        }

        public int BuySignalCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_buy_signal_count(_handle);
            }
        }

        public int SellSignalCount
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_indicators_sell_signal_count(_handle);
            }
        }

        /// <summary>
        /// GetSignals.
        /// </summary>
        /// <returns></returns>
        public List<Signal> GetSignals()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_signals(_handle);
            var result = new List<Signal>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfSignal*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new Signal(ptr[i]));
                }
            }

            NativeMethods.pnf_free_signal_array(arr);
            return result;
        }

        public int PatternCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_pattern_count(_handle);
            }
        }

        public int BullishPatternCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_bullish_pattern_count(_handle);
            }
        }

        public int BearishPatternCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_bearish_pattern_count(_handle);
            }
        }

        /// <summary>
        /// GetPatterns.
        /// </summary>
        /// <returns></returns>
        public List<Pattern> GetPatterns()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_patterns(_handle);
            var result = new List<Pattern>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfPattern*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new Pattern(ptr[i]));
                }
            }

            NativeMethods.pnf_free_pattern_array(arr);
            return result;
        }

        /// <summary>
        /// GetBullishPatterns.
        /// </summary>
        /// <returns></returns>
        public List<Pattern> GetBullishPatterns()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_bullish_patterns(_handle);
            var result = new List<Pattern>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfPattern*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new Pattern(ptr[i]));
                }
            }

            NativeMethods.pnf_free_pattern_array(arr);
            return result;
        }

        /// <summary>
        /// GetBearishPatterns.
        /// </summary>
        /// <returns></returns>
        public List<Pattern> GetBearishPatterns()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_bearish_patterns(_handle);
            var result = new List<Pattern>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfPattern*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new Pattern(ptr[i]));
                }
            }

            NativeMethods.pnf_free_pattern_array(arr);
            return result;
        }

        public int SupportLevelCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_support_level_count(_handle);
            }
        }

        public int ResistanceLevelCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_resistance_level_count(_handle);
            }
        }

        /// <summary>
        /// IsNearSupport.
        /// </summary>
        /// <param name="price"></param>
        /// <param name="tolerance"></param>
        /// <returns></returns>
        public bool IsNearSupport(double price, double tolerance)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_is_near_support(_handle, price, tolerance);
        }

        /// <summary>
        /// IsNearResistance.
        /// </summary>
        /// <param name="price"></param>
        /// <param name="tolerance"></param>
        /// <returns></returns>
        public bool IsNearResistance(double price, double tolerance)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_is_near_resistance(_handle, price, tolerance);
        }

        /// <summary>
        /// GetSupportLevels.
        /// </summary>
        /// <returns></returns>
        public List<SupportResistanceLevel> GetSupportLevels()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_support_levels(_handle);
            var result = new List<SupportResistanceLevel>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfSupportResistanceLevel*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new SupportResistanceLevel(ptr[i]));
                }
            }

            NativeMethods.pnf_free_level_array(arr);
            return result;
        }

        /// <summary>
        /// GetResistanceLevels.
        /// </summary>
        /// <returns></returns>
        public List<SupportResistanceLevel> GetResistanceLevels()
        {
            ThrowIfDisposed();
            var arr = NativeMethods.pnf_indicators_resistance_levels(_handle);
            var result = new List<SupportResistanceLevel>((int)arr.Length);

            unsafe
            {
                var ptr = (PnfSupportResistanceLevel*)arr.Data;
                for (int i = 0; i < (int)arr.Length; i++)
                {
                    result.Add(new SupportResistanceLevel(ptr[i]));
                }
            }

            NativeMethods.pnf_free_level_array(arr);
            return result;
        }

        /// <summary>
        /// GetSupportPrices.
        /// </summary>
        /// <returns></returns>
        public double[] GetSupportPrices()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_support_prices(_handle));
        }

        /// <summary>
        /// GetResistancePrices.
        /// </summary>
        /// <returns></returns>
        public double[] GetResistancePrices()
        {
            ThrowIfDisposed();
            return GetDoubleArray(NativeMethods.pnf_indicators_resistance_prices(_handle));
        }

        /// <summary>
        /// IsInCongestion.
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        public bool IsInCongestion(int column)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_indicators_is_in_congestion(_handle, column);
        }

        public int CongestionZoneCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_indicators_congestion_zone_count(_handle);
            }
        }

        /// <summary>
        /// Summary.
        /// </summary>
        /// <returns></returns>
        public string Summary()
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_indicators_summary(_handle);
            if (ptr == IntPtr.Zero) return string.Empty;
            var result = Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        /// <summary>
        /// Returns a readable summary string.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_indicators_to_string(_handle);
            if (ptr == IntPtr.Zero) return string.Empty;
            var result = Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        private double[] GetDoubleArray(PnfDoubleArray arr)
        {
            var result = new double[(int)arr.Length];
            if ((ulong)arr.Length > 0 && arr.Data != IntPtr.Zero)
            {
                Marshal.Copy(arr.Data, result, 0, (int)arr.Length);
            }
            NativeMethods.pnf_free_double_array(arr);
            return result;
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(Indicators));
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    NativeMethods.pnf_indicators_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~Indicators()
        {
            Dispose(false);
        }

        /// <summary>
        /// Releases unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
