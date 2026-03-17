/**
 * Java API for the PnF (Point-and-Figure) chart engine.
 *
 * <p>This package provides:</p>
 * <ul>
 *   <li>Chart construction via {@link com.pnf.Chart} and {@link com.pnf.ChartConfig}</li>
 *   <li>Indicator and structure analysis via {@link com.pnf.Indicators} and {@link com.pnf.IndicatorConfig}</li>
 *   <li>Signal and pattern models ({@link com.pnf.Signal}, {@link com.pnf.Pattern})</li>
 *   <li>Support/resistance, objectives, and congestion models</li>
 *   <li>Localhost real-time visualization via {@link com.pnf.DashboardServer}</li>
 * </ul>
 *
 * <p>Typical workflow:</p>
 * <ol>
 *   <li>Create and populate a {@code Chart} with OHLC/price inputs.</li>
 *   <li>Run {@code Indicators.calculate(chart)}.</li>
 *   <li>Read trend context, indicators, signals, patterns, and structure levels.</li>
 *   <li>Optionally stream snapshots to a browser dashboard.</li>
 * </ol>
 *
 * <p>Runtime requirements:</p>
 * <ul>
 *   <li>Java 21 or newer</li>
 *   <li>Native JNI library available through {@code -Djava.library.path}</li>
 * </ul>
 */
package com.pnf;
