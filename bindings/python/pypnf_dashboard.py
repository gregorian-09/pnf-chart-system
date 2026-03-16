"""Localhost real-time dashboard for pypnf."""

from __future__ import annotations

import base64
import hashlib
import json
import os
import socket
import threading
import time
from dataclasses import dataclass
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Any

import pypnf

_WS_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


def _repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


def _asset_path(name: str) -> Path:
    return _repo_root() / "dashboard" / "web" / name


def _enum_name(value: Any) -> str:
    if hasattr(value, "name"):
        return str(value.name)
    return str(value)


def build_snapshot(chart: Any, indicators: Any | None = None, sequence: int = 1) -> dict[str, Any]:
    column_count = int(chart.column_count())
    columns = []
    for idx in range(column_count):
        box_count = int(chart.column_box_count(idx))
        boxes = []
        for box_idx in range(box_count):
            boxes.append(
                {
                    "index": box_idx,
                    "price": float(chart.box_price(idx, box_idx)),
                    "type": _enum_name(chart.box_type(idx, box_idx)),
                    "marker": str(chart.box_marker(idx, box_idx) or ""),
                }
            )
        columns.append(
            {
                "index": idx,
                "type": _enum_name(chart.column_type(idx)),
                "box_count": box_count,
                "highest": float(chart.column_high(idx)),
                "lowest": float(chart.column_low(idx)),
                "boxes": boxes,
            }
        )

    indicator_payload = {
        "summary": "",
        "detailed_summary": "",
        "bullish_percent": 0.0,
        "signal_count": 0,
        "buy_signal_count": 0,
        "sell_signal_count": 0,
        "pattern_count": 0,
        "support_level_count": 0,
        "resistance_level_count": 0,
        "congestion_zone_count": 0,
        "signals": [],
        "patterns": [],
        "support_levels": [],
        "resistance_levels": [],
        "price_objectives": [],
        "congestion_zones": [],
        "series": {
            "sma_short": [],
            "sma_medium": [],
            "sma_long": [],
            "bollinger_middle": [],
            "bollinger_upper": [],
            "bollinger_lower": [],
            "rsi": [],
        },
    }
    if indicators is not None:
        try:
            signal_detector = indicators.signals()
            pattern_recognizer = indicators.patterns()
            support_resistance = indicators.support_resistance()
            objectives = indicators.objectives()
            congestion = indicators.congestion()
            sma_short = [float(v) for v in indicators.sma_short().values_copy()]
            sma_medium = [float(v) for v in indicators.sma_medium().values_copy()]
            sma_long = [float(v) for v in indicators.sma_long().values_copy()]
            bollinger = indicators.bollinger()
            rsi = indicators.rsi()
            signals = [
                {
                    "type": _enum_name(signal.type),
                    "column_index": int(signal.column_index),
                    "price": float(signal.price),
                }
                for signal in signal_detector.signals()
            ]
            patterns = [
                {
                    "type": _enum_name(pattern.type),
                    "start_column": int(pattern.start_column),
                    "end_column": int(pattern.end_column),
                    "price": float(pattern.price),
                    "is_bullish": bool(pattern.is_bullish()),
                }
                for pattern in pattern_recognizer.patterns()
            ]
            support_levels = [
                {
                    "type": "Support",
                    "price": float(level.price),
                    "touches": int(level.touch_count),
                }
                for level in support_resistance.support_levels()
            ]
            resistance_levels = [
                {
                    "type": "Resistance",
                    "price": float(level.price),
                    "touches": int(level.touch_count),
                }
                for level in support_resistance.resistance_levels()
            ]
            price_objectives = [
                {
                    "target": float(objective.target_price),
                    "column_index": int(objective.base_column),
                    "box_count": int(objective.box_count),
                    "is_bullish": bool(objective.is_bullish),
                }
                for objective in objectives.objectives()
            ]
            congestion_zones = [
                {
                    "start_column": int(zone.start_column),
                    "end_column": int(zone.end_column),
                    "high_price": float(zone.high_price),
                    "low_price": float(zone.low_price),
                    "column_count": int(zone.column_count),
                }
                for zone in congestion.zones()
            ]
            indicator_payload.update(
                {
                    "summary": str(indicators.summary()),
                    "detailed_summary": str(indicators),
                    "bullish_percent": float(indicators.bullish_percent().value()),
                    "signal_count": int(signal_detector.buy_count() + signal_detector.sell_count()),
                    "buy_signal_count": int(signal_detector.buy_count()),
                    "sell_signal_count": int(signal_detector.sell_count()),
                    "pattern_count": int(pattern_recognizer.pattern_count()),
                    "support_level_count": len(support_levels),
                    "resistance_level_count": len(resistance_levels),
                    "congestion_zone_count": len(congestion_zones),
                    "signals": signals,
                    "patterns": patterns,
                    "support_levels": support_levels,
                    "resistance_levels": resistance_levels,
                    "price_objectives": price_objectives,
                    "congestion_zones": congestion_zones,
                    "series": {
                        "sma_short": sma_short,
                        "sma_medium": sma_medium,
                        "sma_long": sma_long,
                        "bollinger_middle": [float(v) for v in bollinger.middle_copy()],
                        "bollinger_upper": [float(v) for v in bollinger.upper_copy()],
                        "bollinger_lower": [float(v) for v in bollinger.lower_copy()],
                        "rsi": [float(v) for v in rsi.values_copy()],
                    },
                }
            )
        except Exception:
            indicator_payload["summary"] = "Indicator snapshot unavailable"
            indicator_payload["detailed_summary"] = "Detailed indicator snapshot unavailable"

    return {
        "type": "dashboard.snapshot",
        "version": 1,
        "timestamp": int(time.time() * 1000),
        "sequence": sequence,
        "payload": {
            "meta": {
                "binding": "python",
                "library_version": pypnf.version(),
                "server_version": "1",
                "stream_state": "running",
            },
            "chart": {
                "box_size": float(chart.current_box_size()),
                "column_count": column_count,
                "x_column_count": int(chart.x_column_count()),
                "o_column_count": int(chart.o_column_count()),
                "has_bullish_bias": bool(chart.has_bullish_bias()),
                "has_bearish_bias": bool(chart.has_bearish_bias()),
                "columns": columns,
            },
            "indicators": indicator_payload,
        },
    }


class _DashboardHandler(BaseHTTPRequestHandler):
    server_version = "PnFPythonDashboard/1.0"

    def do_GET(self) -> None:
        dashboard = self.server.dashboard_server  # type: ignore[attr-defined]
        if self.headers.get("Upgrade", "").lower() == "websocket" and self.path == "/ws":
            self._handle_ws(dashboard)
            return
        if self.path == "/healthz":
            self._send_bytes(200, b"ok", "text/plain; charset=utf-8")
            return
        if self.path == "/snapshot":
            self._send_bytes(200, dashboard.snapshot_json().encode("utf-8"), "application/json; charset=utf-8")
            return
        if self.path == "/app.js":
            self._send_file(_asset_path("app.js"), "application/javascript; charset=utf-8")
            return
        if self.path == "/styles.css":
            self._send_file(_asset_path("styles.css"), "text/css; charset=utf-8")
            return
        if self.path == "/" or self.path == "/index.html":
            html = _asset_path("index.html").read_text(encoding="utf-8")
            ws_url = f"ws://{dashboard.host}:{dashboard.port}/ws"
            html = html.replace(
                "window.PNF_DASHBOARD_CONFIG = window.PNF_DASHBOARD_CONFIG || {};",
                f"window.PNF_DASHBOARD_CONFIG = {{ wsUrl: {json.dumps(ws_url)} }};",
            )
            self._send_bytes(200, html.encode("utf-8"), "text/html; charset=utf-8")
            return
        self._send_bytes(404, b"not found", "text/plain; charset=utf-8")

    def log_message(self, format: str, *args: Any) -> None:  # noqa: A003
        return

    def _send_file(self, path: Path, content_type: str) -> None:
        self._send_bytes(200, path.read_bytes(), content_type)

    def _send_bytes(self, status: int, payload: bytes, content_type: str) -> None:
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(payload)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(payload)

    def _handle_ws(self, dashboard: "DashboardServer") -> None:
        key = self.headers.get("Sec-WebSocket-Key")
        if not key:
            self.send_error(400, "Missing Sec-WebSocket-Key")
            return
        accept = base64.b64encode(hashlib.sha1((key + _WS_GUID).encode("utf-8")).digest()).decode("ascii")
        self.connection.sendall(
            (
                "HTTP/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
                f"Sec-WebSocket-Accept: {accept}\r\n\r\n"
            ).encode("utf-8")
        )
        self.connection.setblocking(True)
        dashboard._register_client(self.connection)
        try:
            while dashboard.is_running:
                time.sleep(1.0)
        finally:
            dashboard._unregister_client(self.connection)
            try:
                self.connection.close()
            except OSError:
                pass


@dataclass
class DashboardServer:
    chart: Any | None = None
    indicators: Any | None = None

    def __post_init__(self) -> None:
        self.host = "127.0.0.1"
        self.port = 0
        self._server: ThreadingHTTPServer | None = None
        self._thread: threading.Thread | None = None
        self._clients: list[socket.socket] = []
        self._clients_lock = threading.Lock()
        self._sequence = 0
        self._latest = json.dumps({
            "type": "dashboard.snapshot",
            "version": 1,
            "timestamp": int(time.time() * 1000),
            "sequence": 0,
            "payload": {
                "meta": {"binding": "python", "library_version": pypnf.version(), "server_version": "1", "stream_state": "idle"},
                "chart": {"box_size": 0, "column_count": 0, "x_column_count": 0, "o_column_count": 0, "has_bullish_bias": False, "has_bearish_bias": False, "columns": []},
                "indicators": {"summary": "", "detailed_summary": "", "bullish_percent": 0, "signal_count": 0, "buy_signal_count": 0, "sell_signal_count": 0, "pattern_count": 0, "support_level_count": 0, "resistance_level_count": 0, "congestion_zone_count": 0, "signals": [], "patterns": [], "support_levels": [], "resistance_levels": [], "price_objectives": [], "congestion_zones": [], "series": {"sma_short": [], "sma_medium": [], "sma_long": [], "bollinger_middle": [], "bollinger_upper": [], "bollinger_lower": [], "rsi": []}},
            },
        })
        self._running = threading.Event()
        self._publisher_stop = threading.Event()
        self._publisher_thread: threading.Thread | None = None

    @property
    def is_running(self) -> bool:
        return self._running.is_set()

    def set_chart(self, chart: Any) -> None:
        self.chart = chart

    def set_indicators(self, indicators: Any) -> None:
        self.indicators = indicators

    def start(self, host: str = "127.0.0.1", port: int = 0) -> str:
        if self._server is not None:
            return self.url()
        self.host = host
        self._server = ThreadingHTTPServer((host, port), _DashboardHandler)
        self._server.dashboard_server = self  # type: ignore[attr-defined]
        self.port = int(self._server.server_address[1])
        self._running.set()
        self._thread = threading.Thread(target=self._server.serve_forever, name="pypnf-dashboard", daemon=True)
        self._thread.start()
        return self.url()

    def url(self) -> str:
        return f"http://{self.host}:{self.port}/"

    def snapshot(self) -> dict[str, Any]:
        if self.chart is None:
            return json.loads(self._latest)
        self._sequence += 1
        return build_snapshot(self.chart, self.indicators, self._sequence)

    def snapshot_json(self) -> str:
        return self._latest

    def publish(self) -> str:
        snap = self.snapshot()
        self._latest = json.dumps(snap)
        self._broadcast(self._latest)
        return self._latest

    def start_auto_publish(self, interval_ms: int = 250) -> None:
        self.stop_auto_publish()
        self._publisher_stop.clear()

        def _loop() -> None:
            while not self._publisher_stop.wait(interval_ms / 1000.0):
                if self.is_running and self.chart is not None:
                    self.publish()

        self._publisher_thread = threading.Thread(target=_loop, name="pypnf-dashboard-publisher", daemon=True)
        self._publisher_thread.start()

    def stop_auto_publish(self) -> None:
        self._publisher_stop.set()
        self._publisher_thread = None

    def stop(self) -> None:
        self.stop_auto_publish()
        self._running.clear()
        if self._server is not None:
            self._server.shutdown()
            self._server.server_close()
            self._server = None
        with self._clients_lock:
            clients = list(self._clients)
            self._clients.clear()
        for sock in clients:
            try:
                sock.close()
            except OSError:
                pass

    def _register_client(self, sock: socket.socket) -> None:
        with self._clients_lock:
            self._clients.append(sock)
        if self._latest:
            self._send_ws_text(sock, self._latest)

    def _unregister_client(self, sock: socket.socket) -> None:
        with self._clients_lock:
            self._clients = [client for client in self._clients if client is not sock]

    def _broadcast(self, message: str) -> None:
        with self._clients_lock:
            clients = list(self._clients)
        stale = []
        for sock in clients:
            try:
                self._send_ws_text(sock, message)
            except OSError:
                stale.append(sock)
        for sock in stale:
            self._unregister_client(sock)
            try:
                sock.close()
            except OSError:
                pass

    @staticmethod
    def _send_ws_text(sock: socket.socket, message: str) -> None:
        payload = message.encode("utf-8")
        length = len(payload)
        if length < 126:
            header = bytes([0x81, length])
        elif length < 65536:
            header = bytes([0x81, 126]) + length.to_bytes(2, "big")
        else:
            header = bytes([0x81, 127]) + length.to_bytes(8, "big")
        sock.sendall(header + payload)


__all__ = ["DashboardServer", "build_snapshot"]
