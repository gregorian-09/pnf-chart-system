using System;
using System.Runtime.InteropServices;

namespace PnF
{
    /// <summary>
    /// Managed wrapper around a native Point and Figure chart.
    /// </summary>
    public class Chart : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed;

        public Chart()
        {
            _handle = NativeMethods.pnf_chart_create_default();
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create chart");
        }

        public Chart(ChartConfig config)
        {
            _handle = NativeMethods.pnf_chart_create(ref config);
            if (_handle == IntPtr.Zero)
                throw new InvalidOperationException("Failed to create chart with config");
        }

        /// <summary>
        /// AddData.
        /// </summary>
        /// <param name="high"></param>
        /// <param name="low"></param>
        /// <param name="close"></param>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public bool AddData(double high, double low, double close, long timestamp)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_add_data(_handle, high, low, close, timestamp);
        }

        /// <summary>
        /// AddPrice.
        /// </summary>
        /// <param name="price"></param>
        /// <param name="timestamp"></param>
        /// <returns></returns>
        public bool AddPrice(double price, long timestamp)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_add_price(_handle, price, timestamp);
        }

        /// <summary>
        /// AddOHLC.
        /// </summary>
        /// <param name="ohlc"></param>
        /// <returns></returns>
        public bool AddOHLC(OHLC ohlc)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_add_ohlc(_handle, ref ohlc);
        }

        public int ColumnCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_chart_column_count(_handle);
            }
        }

        public int XColumnCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_chart_x_column_count(_handle);
            }
        }

        public int OColumnCount
        {
            get
            {
                ThrowIfDisposed();
                return (int)NativeMethods.pnf_chart_o_column_count(_handle);
            }
        }

        public double BoxSize
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_chart_box_size(_handle);
            }
        }

        /// <summary>
        /// GetColumnType.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public ColumnType GetColumnType(int index)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_column_type(_handle, (UIntPtr)index);
        }

        /// <summary>
        /// GetColumnBoxCount.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public int GetColumnBoxCount(int index)
        {
            ThrowIfDisposed();
            return (int)NativeMethods.pnf_chart_column_box_count(_handle, (UIntPtr)index);
        }

        /// <summary>
        /// GetColumnHighest.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public double GetColumnHighest(int index)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_column_highest(_handle, (UIntPtr)index);
        }

        /// <summary>
        /// GetColumnLowest.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public double GetColumnLowest(int index)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_column_lowest(_handle, (UIntPtr)index);
        }

        /// <summary>
        /// GetBoxPrice.
        /// </summary>
        /// <param name="columnIndex"></param>
        /// <param name="boxIndex"></param>
        /// <returns></returns>
        public double GetBoxPrice(int columnIndex, int boxIndex)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_box_price(_handle, (UIntPtr)columnIndex, (UIntPtr)boxIndex);
        }

        /// <summary>
        /// GetBoxType.
        /// </summary>
        /// <param name="columnIndex"></param>
        /// <param name="boxIndex"></param>
        /// <returns></returns>
        public BoxType GetBoxType(int columnIndex, int boxIndex)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_box_type(_handle, (UIntPtr)columnIndex, (UIntPtr)boxIndex);
        }

        public string? GetBoxMarker(int columnIndex, int boxIndex)
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_chart_box_marker(_handle, (UIntPtr)columnIndex, (UIntPtr)boxIndex);
            if (ptr == IntPtr.Zero) return null;
            var result = Marshal.PtrToStringAnsi(ptr);
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        public bool HasBullishBias
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_chart_has_bullish_bias(_handle);
            }
        }

        public bool HasBearishBias
        {
            get
            {
                ThrowIfDisposed();
                return NativeMethods.pnf_chart_has_bearish_bias(_handle);
            }
        }

        /// <summary>
        /// IsAboveBullishSupport.
        /// </summary>
        /// <param name="price"></param>
        /// <returns></returns>
        public bool IsAboveBullishSupport(double price)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_is_above_bullish_support(_handle, price);
        }

        /// <summary>
        /// IsBelowBearishResistance.
        /// </summary>
        /// <param name="price"></param>
        /// <returns></returns>
        public bool IsBelowBearishResistance(double price)
        {
            ThrowIfDisposed();
            return NativeMethods.pnf_chart_is_below_bearish_resistance(_handle, price);
        }

        /// <summary>
        /// Clear.
        /// </summary>
        public void Clear()
        {
            ThrowIfDisposed();
            NativeMethods.pnf_chart_clear(_handle);
        }

        /// <summary>
        /// Returns a readable summary string.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_chart_to_string(_handle);
            if (ptr == IntPtr.Zero) return string.Empty;
            var result = Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        /// <summary>
        /// ToAscii.
        /// </summary>
        /// <returns></returns>
        public string ToAscii()
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_chart_to_ascii(_handle);
            if (ptr == IntPtr.Zero) return string.Empty;
            var result = Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        /// <summary>
        /// ToJson.
        /// </summary>
        /// <returns></returns>
        public string ToJson()
        {
            ThrowIfDisposed();
            var ptr = NativeMethods.pnf_chart_to_json(_handle);
            if (ptr == IntPtr.Zero) return string.Empty;
            var result = Marshal.PtrToStringAnsi(ptr) ?? string.Empty;
            NativeMethods.pnf_free_string(ptr);
            return result;
        }

        internal IntPtr Handle => _handle;

        private void ThrowIfDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(Chart));
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    NativeMethods.pnf_chart_destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        ~Chart()
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
