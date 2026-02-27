/// <summary>
/// File: Types.cs
/// Types binding types and helpers.
/// </summary>
using System;
using System.Runtime.InteropServices;

namespace PnF
{
    /// <summary>
    /// BoxType binding type.
    /// </summary>
    public enum BoxType
    {
        X = 0,
        O = 1
    }

    /// <summary>
    /// ColumnType binding type.
    /// </summary>
    public enum ColumnType
    {
        X = 0,
        O = 1,
        Mixed = 2
    }

    /// <summary>
    /// ConstructionMethod binding type.
    /// </summary>
    public enum ConstructionMethod
    {
        Close = 0,
        HighLow = 1
    }

    /// <summary>
    /// BoxSizeMethod binding type.
    /// </summary>
    public enum BoxSizeMethod
    {
        Fixed = 0,
        Traditional = 1,
        Percentage = 2,
        Points = 3
    }

    /// <summary>
    /// SignalType binding type.
    /// </summary>
    public enum SignalType
    {
        None = 0,
        Buy = 1,
        Sell = 2
    }

    /// <summary>
    /// PatternType binding type.
    /// </summary>
    public enum PatternType
    {
        None = 0,
        DoubleTopBreakout,
        DoubleBottomBreakdown,
        TripleTopBreakout,
        TripleBottomBreakdown,
        QuadrupleTopBreakout,
        QuadrupleBottomBreakdown,
        AscendingTripleTop,
        DescendingTripleBottom,
        BullishCatapult,
        BearishCatapult,
        BullishSignalReversed,
        BearishSignalReversed,
        BullishTriangle,
        BearishTriangle,
        LongTailDown,
        HighPole,
        LowPole,
        BullTrap,
        BearTrap,
        SpreadTripleTop,
        SpreadTripleBottom
    }

    [StructLayout(LayoutKind.Sequential)]
    /// <summary>
    /// ChartConfig binding type.
    /// </summary>
    public struct ChartConfig
    {
        public ConstructionMethod Method;
        public BoxSizeMethod BoxSizeMethod;
        public double BoxSize;
        public int Reversal;

        /// <summary>
        /// Default API.
        /// </summary>
        /// <summary>
        /// Default API.
        /// </summary>
        /// <returns></returns>
        public static ChartConfig Default()
        {
            return new ChartConfig
            {
                Method = ConstructionMethod.Close,
                BoxSizeMethod = BoxSizeMethod.Traditional,
                BoxSize = 0.0,
                Reversal = 3
            };
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    /// <summary>
    /// IndicatorConfig binding type.
    /// </summary>
    public struct IndicatorConfig
    {
        public int SmaShortPeriod;
        public int SmaMediumPeriod;
        public int SmaLongPeriod;
        public int BollingerPeriod;
        public double BollingerStdDevs;
        public int RsiPeriod;
        public double RsiOverbought;
        public double RsiOversold;
        public double BullishAlertThreshold;
        public double BearishAlertThreshold;
        public double SupportResistanceThreshold;
        public int CongestionMinColumns;
        public double CongestionPriceRange;

        /// <summary>
        /// Default API.
        /// </summary>
        /// <summary>
        /// Default API.
        /// </summary>
        /// <returns></returns>
        public static IndicatorConfig Default()
        {
            return new IndicatorConfig
            {
                SmaShortPeriod = 5,
                SmaMediumPeriod = 10,
                SmaLongPeriod = 20,
                BollingerPeriod = 20,
                BollingerStdDevs = 2.0,
                RsiPeriod = 14,
                RsiOverbought = 70.0,
                RsiOversold = 30.0,
                BullishAlertThreshold = 70.0,
                BearishAlertThreshold = 30.0,
                SupportResistanceThreshold = 0.01,
                CongestionMinColumns = 4,
                CongestionPriceRange = 0.05
            };
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    /// <summary>
    /// OHLC binding type.
    /// </summary>
    public struct OHLC
    {
        public long Timestamp;
        public double Open;
        public double High;
        public double Low;
        public double Close;
        public double Volume;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfSignal
    {
        public SignalType Type;
        public int ColumnIndex;
        public double Price;
    }

    /// <summary>
    /// Signal binding type.
    /// </summary>
    public class Signal
    {
        public SignalType Type { get; set; }
        public int ColumnIndex { get; set; }
        public double Price { get; set; }

        internal Signal(PnfSignal signal)
        {
            Type = signal.Type;
            ColumnIndex = signal.ColumnIndex;
            Price = signal.Price;
        }

        /// <summary>
        /// ToString API.
        /// </summary>
        /// <summary>
        /// ToString API.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"Signal{{Type={Type}, Column={ColumnIndex}, Price={Price:F2}}}";
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfPattern
    {
        public PatternType Type;
        public int StartColumn;
        public int EndColumn;
        public double Price;
        [MarshalAs(UnmanagedType.I1)]
        public bool IsBullish;
    }

    /// <summary>
    /// Pattern binding type.
    /// </summary>
    public class Pattern
    {
        public PatternType Type { get; set; }
        public int StartColumn { get; set; }
        public int EndColumn { get; set; }
        public double Price { get; set; }
        public bool IsBullish { get; set; }

        internal Pattern(PnfPattern pattern)
        {
            Type = pattern.Type;
            StartColumn = pattern.StartColumn;
            EndColumn = pattern.EndColumn;
            Price = pattern.Price;
            IsBullish = pattern.IsBullish;
        }

        /// <summary>
        /// ToString API.
        /// </summary>
        /// <summary>
        /// ToString API.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"Pattern{{Type={Type}, Columns={StartColumn}-{EndColumn}, Price={Price:F2}, Bullish={IsBullish}}}";
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfSupportResistanceLevel
    {
        public double Price;
        public int TouchCount;
        [MarshalAs(UnmanagedType.I1)]
        public bool IsSupport;
    }

    /// <summary>
    /// SupportResistanceLevel binding type.
    /// </summary>
    public class SupportResistanceLevel
    {
        public double Price { get; set; }
        public int TouchCount { get; set; }
        public bool IsSupport { get; set; }

        internal SupportResistanceLevel(PnfSupportResistanceLevel level)
        {
            Price = level.Price;
            TouchCount = level.TouchCount;
            IsSupport = level.IsSupport;
        }

        /// <summary>
        /// ToString API.
        /// </summary>
        /// <summary>
        /// ToString API.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            var type = IsSupport ? "Support" : "Resistance";
            return $"{type}{{Price={Price:F2}, Touches={TouchCount}}}";
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfDoubleArray
    {
        public IntPtr Data;
        public UIntPtr Length;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfSignalArray
    {
        public IntPtr Data;
        public UIntPtr Length;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfPatternArray
    {
        public IntPtr Data;
        public UIntPtr Length;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct PnfLevelArray
    {
        public IntPtr Data;
        public UIntPtr Length;
    }
}
