#!/usr/bin/env python3
import csv
import signal
import sys
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "build-linux" / "python"))
sys.path.insert(0, str(ROOT / "bindings" / "python"))

import pypnf
from pypnf_dashboard import DashboardServer


def main() -> None:
    csv_file = ROOT / "fixtures" / "Boom_500_Index_PERIOD_H1.csv"

    cfg = pypnf.ChartConfig()
    cfg.method = pypnf.ConstructionMethod.HighLow
    cfg.box_size_method = pypnf.BoxSizeMethod.Traditional
    cfg.box_size = 0.0
    cfg.reversal = 3

    chart = pypnf.Chart(cfg)
    with csv_file.open() as f:
        reader = csv.DictReader(f)
        for row in reader:
            ts = datetime.strptime(row["Timestamp"], "%Y.%m.%d %H:%M:%S")
            chart.add_data(float(row["High"]), float(row["Low"]), float(row["Close"]), ts)

    indicators = pypnf.Indicators(pypnf.IndicatorConfig())
    indicators.calculate(chart)

    server = DashboardServer(chart, indicators)
    server.start("127.0.0.1", 8761)
    server.publish()
    print(server.url(), flush=True)
    signal.pause()


if __name__ == "__main__":
    main()
