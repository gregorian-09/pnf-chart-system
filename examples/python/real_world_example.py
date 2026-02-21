#!/usr/bin/env python3
import sys
import os
import csv
from pathlib import Path
from datetime import datetime, timedelta

sys.path.insert(0, str(Path(__file__).parent.parent.parent / 'build' / 'python'))

import pypnf

def load_csv(filename):
    data = []
    with open(filename, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append({
                'timestamp': row['Timestamp'],
                'open': float(row['Open']),
                'high': float(row['High']),
                'low': float(row['Low']),
                'close': float(row['Close'])
            })
    print(f"Loaded {len(data)} bars from {filename}")
    return data

def process_dataset(csv_file, output_prefix):
    print("\n" + "=" * 60)
    print(f"Processing: {csv_file}")
    print("=" * 60)

    data = load_csv(csv_file)
    if not data:
        print("No data loaded, skipping...")
        return

    config = pypnf.ChartConfig()
    config.method = pypnf.ConstructionMethod.HighLow

    if 'GBPUSD' in csv_file or 'EURUSD' in csv_file:
        config.box_size_method = pypnf.BoxSizeMethod.Points
        config.box_size = 0.0020
    else:
        config.box_size_method = pypnf.BoxSizeMethod.Traditional
        config.box_size = 0.0
    config.reversal = 3

    chart = pypnf.Chart(config)

    timestamp = datetime(2021, 1, 1)
    for bar in data:
        chart.add_data(bar['high'], bar['low'], bar['close'], timestamp)
        timestamp += timedelta(seconds=1)

    print("\nChart Statistics:")
    print(f"  Total columns: {chart.column_count()}")
    print(f"  X columns: {chart.x_column_count()}")
    print(f"  O columns: {chart.o_column_count()}")

    ind_config = pypnf.IndicatorConfig()
    ind_config.sma_short_period = 5
    ind_config.sma_medium_period = 10
    ind_config.sma_long_period = 20
    ind_config.bollinger_period = 20
    ind_config.bollinger_std_devs = 2.0
    ind_config.rsi_period = 14
    ind_config.rsi_overbought = 70.0
    ind_config.rsi_oversold = 30.0

    indicators = pypnf.Indicators(ind_config)
    indicators.calculate(chart)

    print("\nIndicator calculation complete!")

    print("\nGenerating visualizations...")

    os.makedirs(os.path.dirname(output_prefix), exist_ok=True)

    with open(f"{output_prefix}_ascii.txt", 'w') as f:
        f.write(pypnf.Visualization.to_ascii(chart))
    print(f"  ASCII chart saved to: {output_prefix}_ascii.txt")

    with open(f"{output_prefix}_chart.json", 'w') as f:
        f.write(pypnf.Visualization.to_json(chart))
    print(f"  JSON chart saved to: {output_prefix}_chart.json")

    with open(f"{output_prefix}_columns.csv", 'w') as f:
        f.write(pypnf.Visualization.to_csv_columns(chart))
    print(f"  CSV columns saved to: {output_prefix}_columns.csv")

    print("\nProcessing complete!")

def main():
    print("PnF Chart Library - Real World Data Example (Python)")
    print(f"Library Version: {pypnf.version()}")

    base_path = Path(__file__).parent.parent.parent / 'fixtures'
    output_path = Path(__file__).parent / 'output'

    process_dataset(str(base_path / 'GBPUSD_PERIOD_M1.csv'), str(output_path / 'gbpusd_m1'))
    process_dataset(str(base_path / 'Boom_500_Index_PERIOD_H1.csv'), str(output_path / 'boom500_h1'))
    process_dataset(str(base_path / 'Volatility_75_Index_PERIOD_D1.csv'), str(output_path / 'volatility75_d1'))

    print("\n" + "=" * 60)
    print("All datasets processed successfully!")
    print("=" * 60)

if __name__ == '__main__':
    main()
