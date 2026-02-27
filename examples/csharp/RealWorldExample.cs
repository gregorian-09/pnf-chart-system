using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using PnF;

class OHLCBar
{
    public string Timestamp { get; set; }
    public double Open { get; set; }
    public double High { get; set; }
    public double Low { get; set; }
    public double Close { get; set; }
}

class RealWorldExample
{
    static List<OHLCBar> LoadCSV(string filename)
    {
        var data = new List<OHLCBar>();

        using (var reader = new StreamReader(filename))
        {
            reader.ReadLine();

            string line;
            while ((line = reader.ReadLine()) != null)
            {
                var fields = line.Split(',');
                if (fields.Length >= 6)
                {
                    data.Add(new OHLCBar
                    {
                        Timestamp = fields[0],
                        Open = double.Parse(fields[2]),
                        High = double.Parse(fields[3]),
                        Low = double.Parse(fields[4]),
                        Close = double.Parse(fields[5])
                    });
                }
            }
        }

        Console.WriteLine($"Loaded {data.Count} bars from {filename}");
        return data;
    }

    static void ProcessDataset(string csvFile, string outputPrefix)
    {
        Console.WriteLine($"\n{new string('=', 60)}");
        Console.WriteLine($"Processing: {csvFile}");
        Console.WriteLine(new string('=', 60));

        var data = LoadCSV(csvFile);
        if (data.Count == 0)
        {
            Console.Error.WriteLine("No data loaded, skipping...");
            return;
        }

        var isCurrencyPair = csvFile.Contains("GBPUSD") || csvFile.Contains("EURUSD");
        var config = new ChartConfig
        {
            Method = ConstructionMethod.HighLow,
            BoxSizeMethod = isCurrencyPair ? BoxSizeMethod.Points : BoxSizeMethod.Traditional,
            BoxSize = isCurrencyPair ? 0.0020 : 0.0,
            Reversal = 3
        };

        using var chart = new Chart(config);

        long timestamp = 1640000000;
        foreach (var bar in data)
        {
            chart.AddData(bar.High, bar.Low, bar.Close, timestamp++);
        }

        Console.WriteLine("\nChart Statistics:");
        Console.WriteLine($"  Total columns: {chart.ColumnCount}");
        Console.WriteLine($"  X columns: {chart.XColumnCount}");
        Console.WriteLine($"  O columns: {chart.OColumnCount}");
        Console.WriteLine($"  Box size: {chart.BoxSize}");
        Console.WriteLine($"  Has bullish bias: {(chart.HasBullishBias ? "Yes" : "No")}");
        Console.WriteLine($"  Has bearish bias: {(chart.HasBearishBias ? "Yes" : "No")}");

        var indConfig = new IndicatorConfig
        {
            SmaShortPeriod = 5,
            SmaMediumPeriod = 10,
            SmaLongPeriod = 20,
            BollingerPeriod = 20,
            BollingerStdDevs = 2.0,
            RsiPeriod = 14,
            RsiOverbought = 70.0,
            RsiOversold = 30.0
        };

        using var indicators = new Indicators(indConfig);
        indicators.Calculate(chart);

        Console.WriteLine("\nIndicator Results:");
        Console.WriteLine($"  Bullish Percent: {indicators.BullishPercent:F2}%");
        Console.WriteLine($"  Signals: {indicators.SignalCount}");
        Console.WriteLine($"  Patterns: {indicators.PatternCount}");
        Console.WriteLine($"  Buy signals: {indicators.BuySignalCount}");
        Console.WriteLine($"  Sell signals: {indicators.SellSignalCount}");
        Console.WriteLine($"  Support levels: {indicators.SupportLevelCount}");
        Console.WriteLine($"  Resistance levels: {indicators.ResistanceLevelCount}");

        var signals = indicators.GetSignals();
        Console.WriteLine("\nRecent Signals:");
        foreach (var signal in signals.Take(5))
        {
            var signalType = signal.Type == SignalType.Buy ? "BUY" : "SELL";
            Console.WriteLine($"  Column {signal.ColumnIndex}: {signalType} (Price: {signal.Price:F5})");
        }

        var patterns = indicators.GetPatterns();
        Console.WriteLine("\nRecent Patterns:");
        foreach (var pattern in patterns.Take(5))
        {
            var sentiment = pattern.IsBullish ? "BULLISH" : "BEARISH";
            Console.WriteLine($"  Columns {pattern.StartColumn}-{pattern.EndColumn}: {pattern.Type} ({sentiment}, Price: {pattern.Price:F5})");
        }

        Console.WriteLine("\nGenerating visualizations...");

        Directory.CreateDirectory(Path.GetDirectoryName(outputPrefix));

        File.WriteAllText($"{outputPrefix}_ascii.txt", chart.ToAscii());
        Console.WriteLine($"  ASCII chart saved to: {outputPrefix}_ascii.txt");

        File.WriteAllText($"{outputPrefix}_chart.json", chart.ToJson());
        Console.WriteLine($"  JSON chart saved to: {outputPrefix}_chart.json");

        Console.WriteLine("\nProcessing complete!");
    }

    static void Main(string[] args)
    {
        Console.WriteLine("PnF Chart Library - Real World Data Example (C#)");
        Console.WriteLine($"Library Version: {PnF.Version.String}");

        try
        {
            ProcessDataset("../../fixtures/GBPUSD_PERIOD_M1.csv", "output/gbpusd_m1");
            ProcessDataset("../../fixtures/Boom_500_Index_PERIOD_H1.csv", "output/boom500_h1");
            ProcessDataset("../../fixtures/Volatility_75_Index_PERIOD_D1.csv", "output/volatility75_d1");

            Console.WriteLine($"\n{new string('=', 60)}");
            Console.WriteLine("All datasets processed successfully!");
            Console.WriteLine(new string('=', 60));
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error: {ex.Message}");
            Console.Error.WriteLine(ex.StackTrace);
            Environment.Exit(1);
        }
    }
}
