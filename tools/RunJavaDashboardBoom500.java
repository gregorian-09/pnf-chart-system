import com.pnf.Chart;
import com.pnf.ChartConfig;
import com.pnf.ConstructionMethod;
import com.pnf.DashboardServer;
import com.pnf.IndicatorConfig;
import com.pnf.Indicators;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public final class RunJavaDashboardBoom500 {
    private RunJavaDashboardBoom500() {}

    public static void main(String[] args) throws Exception {
        final String host = args.length > 0 ? args[0] : "0.0.0.0";
        final int port = args.length > 1 ? Integer.parseInt(args[1]) : 8763;
        final Path csv = Path.of("/home/gregorian-rayne/ChartSystem/fixtures/Boom_500_Index_PERIOD_H1.csv");

        final ChartConfig cfg = new ChartConfig();
        cfg.method = ConstructionMethod.HIGH_LOW;

        final Chart chart = new Chart(cfg);
        final List<String> lines = Files.readAllLines(csv);
        for (int i = 1; i < lines.size(); i++) {
            final String[] p = lines.get(i).split(",");
            if (p.length < 6) {
                continue;
            }
            final double high = Double.parseDouble(p[3]);
            final double low = Double.parseDouble(p[4]);
            final double close = Double.parseDouble(p[5]);
            chart.addData(high, low, close, i);
        }

        final Indicators indicators = new Indicators(new IndicatorConfig());
        indicators.calculate(chart);

        final DashboardServer server = new DashboardServer(chart, indicators);
        final String url = server.start(host, port);
        server.publish();
        System.out.println(url);
        System.out.flush();
        Thread.sleep(Long.MAX_VALUE);
    }
}
