//! Localhost dashboard server for Rust bindings.
use crate::{version, Chart, Indicators};
use std::fs;
use std::io::{self, Read, Write};
use std::net::{Shutdown, TcpListener, TcpStream};
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};
use std::sync::atomic::{AtomicBool, AtomicU64, Ordering};
use std::thread::{self, JoinHandle};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

struct SharedState {
    running: AtomicBool,
    latest: Mutex<String>,
    clients: Mutex<Vec<TcpStream>>,
}

/// Localhost dashboard server.
pub struct DashboardServer {
    host: String,
    port: u16,
    shared: Arc<SharedState>,
    sequence: AtomicU64,
    accept_thread: Option<JoinHandle<()>>,
}

impl DashboardServer {
    /// Creates a new dashboard server.
    pub fn new() -> Self {
        Self {
            host: "127.0.0.1".to_string(),
            port: 0,
            shared: Arc::new(SharedState {
                running: AtomicBool::new(false),
                latest: Mutex::new(empty_snapshot()),
                clients: Mutex::new(Vec::new()),
            }),
            sequence: AtomicU64::new(0),
            accept_thread: None,
        }
    }

    /// Starts the localhost dashboard server.
    pub fn start(&mut self, host: &str, port: u16) -> io::Result<String> {
        if self.shared.running.load(Ordering::SeqCst) {
            return Ok(self.url());
        }
        let listener = TcpListener::bind((host, port))?;
        self.host = host.to_string();
        self.port = listener.local_addr()?.port();
        self.shared.running.store(true, Ordering::SeqCst);
        let shared = Arc::clone(&self.shared);
        let host_string = self.host.clone();
        let port_num = self.port;
        self.accept_thread = Some(thread::spawn(move || {
            for stream in listener.incoming() {
                if !shared.running.load(Ordering::SeqCst) {
                    break;
                }
                if let Ok(stream) = stream {
                    let shared = Arc::clone(&shared);
                    let host = host_string.clone();
                    thread::spawn(move || handle_connection(stream, shared, &host, port_num));
                }
            }
        }));
        Ok(self.url())
    }

    /// Returns the dashboard URL.
    pub fn url(&self) -> String {
        format!("http://{}:{}/", self.host, self.port)
    }

    /// Returns the latest snapshot JSON.
    pub fn snapshot_json(&self) -> String {
        self.shared.latest.lock().unwrap().clone()
    }

    /// Builds and publishes the latest snapshot.
    pub fn publish(&self, chart: &Chart, indicators: Option<&Indicators>) -> String {
        let next = self.sequence.fetch_add(1, Ordering::SeqCst) + 1;
        let snapshot = build_snapshot_json(chart, indicators, next);
        *self.shared.latest.lock().unwrap() = snapshot.clone();
        broadcast(&self.shared, &snapshot);
        snapshot
    }

    /// Stops the dashboard server.
    pub fn stop(&mut self) {
        self.shared.running.store(false, Ordering::SeqCst);
        let _ = TcpStream::connect((self.host.as_str(), self.port));
        if let Some(handle) = self.accept_thread.take() {
            let _ = handle.join();
        }
        let mut clients = self.shared.clients.lock().unwrap();
        for stream in clients.drain(..) {
            let _ = stream.shutdown(Shutdown::Both);
        }
    }
}

impl Default for DashboardServer {
    fn default() -> Self {
        Self::new()
    }
}

impl Drop for DashboardServer {
    fn drop(&mut self) {
        self.stop();
    }
}

/// Builds a snapshot JSON payload.
pub fn build_snapshot_json(chart: &Chart, indicators: Option<&Indicators>, sequence: u64) -> String {
    let mut columns = String::from("[");
    for idx in 0..chart.column_count() {
        if idx > 0 {
            columns.push(',');
        }
        let mut boxes = String::from("[");
        for box_idx in 0..chart.column_box_count(idx) {
            if box_idx > 0 {
                boxes.push(',');
            }
            let box_entry = format!(
                "{{\"index\":{},\"price\":{},\"type\":\"{}\",\"marker\":\"{}\"}}",
                box_idx,
                chart.box_price(idx, box_idx),
                json_escape(&format!("{:?}", chart.box_type(idx, box_idx))),
                json_escape(&chart.box_marker(idx, box_idx).unwrap_or_default()),
            );
            boxes.push_str(&box_entry);
        }
        boxes.push(']');
        let entry = format!(
            "{{\"index\":{},\"type\":\"{}\",\"box_count\":{},\"highest\":{},\"lowest\":{},\"boxes\":{}}}",
            idx,
            json_escape(&format!("{:?}", chart.column_type(idx))),
            chart.column_box_count(idx),
            chart.column_highest(idx),
            chart.column_lowest(idx),
            boxes
        );
        columns.push_str(&entry);
    }
    columns.push(']');

    let (summary, detailed_summary, bullish_percent, signal_count, buy_signal_count, sell_signal_count, pattern_count, support_level_count, resistance_level_count, congestion_zone_count, signals_json, patterns_json, support_levels_json, resistance_levels_json, series_json) =
        if let Some(ind) = indicators {
            (
                ind.summary().unwrap_or_default(),
                ind.to_string().unwrap_or_default(),
                ind.bullish_percent(),
                ind.signal_count(),
                ind.buy_signal_count() as usize,
                ind.sell_signal_count() as usize,
                ind.pattern_count(),
                ind.support_level_count(),
                ind.resistance_level_count(),
                ind.congestion_zone_count(),
                signals_json(ind),
                patterns_json(ind),
                support_levels_json(&ind.support_levels(), "Support"),
                support_levels_json(&ind.resistance_levels(), "Resistance"),
                series_json(ind),
            )
        } else {
            (String::new(), String::new(), 0.0, 0, 0, 0, 0, 0, 0, 0, String::from("[]"), String::from("[]"), String::from("[]"), String::from("[]"), String::from("{\"sma_short\":[],\"sma_medium\":[],\"sma_long\":[],\"bollinger_middle\":[],\"bollinger_upper\":[],\"bollinger_lower\":[],\"rsi\":[]}"))
        };

    let mut json = String::from("{\"type\":\"dashboard.snapshot\",\"version\":1,");
    json.push_str(&format!("\"timestamp\":{},\"sequence\":{},", now_millis(), sequence));
    json.push_str("\"payload\":{\"meta\":{");
    json.push_str(&format!(
        "\"binding\":\"rust\",\"library_version\":\"{}\",\"server_version\":\"1\",\"stream_state\":\"running\"",
        json_escape(&version())
    ));
    json.push_str("},\"chart\":{");
    json.push_str(&format!(
        "\"box_size\":{},\"column_count\":{},\"x_column_count\":{},\"o_column_count\":{},\"has_bullish_bias\":{},\"has_bearish_bias\":{},\"columns\":{}",
        chart.box_size(),
        chart.column_count(),
        chart.x_column_count(),
        chart.o_column_count(),
        chart.has_bullish_bias(),
        chart.has_bearish_bias(),
        columns
    ));
    json.push_str("},\"indicators\":{");
    json.push_str(&format!(
        "\"summary\":\"{}\",\"detailed_summary\":\"{}\",\"bullish_percent\":{},\"signal_count\":{},\"buy_signal_count\":{},\"sell_signal_count\":{},\"pattern_count\":{},\"support_level_count\":{},\"resistance_level_count\":{},\"congestion_zone_count\":{},\"signals\":{},\"patterns\":{},\"support_levels\":{},\"resistance_levels\":{},\"price_objectives\":[],\"congestion_zones\":[],\"series\":{}",
        json_escape(&summary),
        json_escape(&detailed_summary),
        bullish_percent,
        signal_count,
        buy_signal_count,
        sell_signal_count,
        pattern_count,
        support_level_count,
        resistance_level_count,
        congestion_zone_count,
        signals_json,
        patterns_json,
        support_levels_json,
        resistance_levels_json,
        series_json
    ));
    json.push_str("}}}");
    json
}

fn signals_json(ind: &Indicators) -> String {
    let mut json = String::from("[");
    for (idx, signal) in ind.signals().iter().enumerate() {
        if idx > 0 {
            json.push(',');
        }
        json.push_str(&format!(
            "{{\"type\":\"{}\",\"column_index\":{},\"price\":{}}}",
            json_escape(&format!("{:?}", signal.signal_type)),
            signal.column_index,
            signal.price,
        ));
    }
    json.push(']');
    json
}

fn patterns_json(ind: &Indicators) -> String {
    let mut json = String::from("[");
    for (idx, pattern) in ind.patterns().iter().enumerate() {
        if idx > 0 {
            json.push(',');
        }
        json.push_str(&format!(
            "{{\"type\":\"{}\",\"start_column\":{},\"end_column\":{},\"price\":{},\"is_bullish\":{}}}",
            json_escape(&format!("{:?}", pattern.pattern_type)),
            pattern.start_column,
            pattern.end_column,
            pattern.price,
            pattern.is_bullish,
        ));
    }
    json.push(']');
    json
}

fn support_levels_json(levels: &[crate::types::SupportResistanceLevel], label: &str) -> String {
    let mut json = String::from("[");
    for (idx, level) in levels.iter().enumerate() {
        if idx > 0 {
            json.push(',');
        }
        json.push_str(&format!(
            "{{\"type\":\"{}\",\"price\":{},\"touches\":{}}}",
            json_escape(label),
            level.price,
            level.touch_count,
        ));
    }
    json.push(']');
    json
}

fn double_series_json(values: &[f64]) -> String {
    let mut json = String::from("[");
    for (idx, value) in values.iter().enumerate() {
        if idx > 0 {
            json.push(',');
        }
        json.push_str(&value.to_string());
    }
    json.push(']');
    json
}

fn series_json(ind: &Indicators) -> String {
    format!(
        "{{\"sma_short\":{},\"sma_medium\":{},\"sma_long\":{},\"bollinger_middle\":{},\"bollinger_upper\":{},\"bollinger_lower\":{},\"rsi\":{}}}",
        double_series_json(&ind.sma_short_values()),
        double_series_json(&ind.sma_medium_values()),
        double_series_json(&ind.sma_long_values()),
        double_series_json(&ind.bollinger_middle_values()),
        double_series_json(&ind.bollinger_upper_values()),
        double_series_json(&ind.bollinger_lower_values()),
        double_series_json(&ind.rsi_values()),
    )
}

fn empty_snapshot() -> String {
    let mut json = String::from("{\"type\":\"dashboard.snapshot\",\"version\":1,");
    json.push_str(&format!("\"timestamp\":{},\"sequence\":0,", now_millis()));
    json.push_str("\"payload\":{\"meta\":{");
    json.push_str(&format!(
        "\"binding\":\"rust\",\"library_version\":\"{}\",\"server_version\":\"1\",\"stream_state\":\"idle\"",
        json_escape(&version())
    ));
    json.push_str("},\"chart\":{\"box_size\":0,\"column_count\":0,\"x_column_count\":0,\"o_column_count\":0,\"has_bullish_bias\":false,\"has_bearish_bias\":false,\"columns\":[]},");
    json.push_str("\"indicators\":{\"summary\":\"\",\"detailed_summary\":\"\",\"bullish_percent\":0,\"signal_count\":0,\"buy_signal_count\":0,\"sell_signal_count\":0,\"pattern_count\":0,\"support_level_count\":0,\"resistance_level_count\":0,\"congestion_zone_count\":0,\"signals\":[],\"patterns\":[],\"support_levels\":[],\"resistance_levels\":[],\"price_objectives\":[],\"congestion_zones\":[],\"series\":{\"sma_short\":[],\"sma_medium\":[],\"sma_long\":[],\"bollinger_middle\":[],\"bollinger_upper\":[],\"bollinger_lower\":[],\"rsi\":[]}}}}");
    json
}

fn handle_connection(mut stream: TcpStream, shared: Arc<SharedState>, host: &str, port: u16) {
    let mut buf = [0_u8; 8192];
    let Ok(size) = stream.read(&mut buf) else { return; };
    if size == 0 {
        return;
    }
    let request = String::from_utf8_lossy(&buf[..size]);
    let mut lines = request.lines();
    let first_line = lines.next().unwrap_or_default();
    let mut parts = first_line.split_whitespace();
    let method = parts.next().unwrap_or_default();
    let path = parts.next().unwrap_or("/");
    let is_ws = request.to_ascii_lowercase().contains("upgrade: websocket");

    if method != "GET" {
        let _ = write_response(&mut stream, 405, "text/plain; charset=utf-8", b"method not allowed");
        return;
    }

    if is_ws && path == "/ws" {
        if let Some(key) = websocket_key(&request) {
            let accept = websocket_accept(&key);
            let response = format!(
                "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: {}\r\n\r\n",
                accept
            );
            if stream.write_all(response.as_bytes()).is_err() {
                return;
            }
            if let Ok(client) = stream.try_clone() {
                shared.clients.lock().unwrap().push(client);
            }
            let latest = shared.latest.lock().unwrap().clone();
            let _ = send_ws_text(&mut stream, &latest);
            while shared.running.load(Ordering::SeqCst) {
                thread::sleep(Duration::from_secs(1));
            }
        }
        return;
    }

    match path {
        "/" | "/index.html" => {
            if let Ok(mut html) = fs::read_to_string(resolve_asset("index.html")) {
                let config = format!(
                    "window.PNF_DASHBOARD_CONFIG = {{ wsUrl: \"ws://{}:{}/ws\" }};",
                    host, port
                );
                html = html.replace(
                    "window.PNF_DASHBOARD_CONFIG = window.PNF_DASHBOARD_CONFIG || {};",
                    &config,
                );
                let _ = write_response(&mut stream, 200, "text/html; charset=utf-8", html.as_bytes());
            }
        }
        "/app.js" => {
            if let Ok(body) = fs::read(resolve_asset("app.js")) {
                let _ = write_response(&mut stream, 200, "application/javascript; charset=utf-8", &body);
            }
        }
        "/styles.css" => {
            if let Ok(body) = fs::read(resolve_asset("styles.css")) {
                let _ = write_response(&mut stream, 200, "text/css; charset=utf-8", &body);
            }
        }
        "/healthz" => {
            let _ = write_response(&mut stream, 200, "text/plain; charset=utf-8", b"ok");
        }
        "/snapshot" => {
            let body = shared.latest.lock().unwrap().clone();
            let _ = write_response(&mut stream, 200, "application/json; charset=utf-8", body.as_bytes());
        }
        _ => {
            let _ = write_response(&mut stream, 404, "text/plain; charset=utf-8", b"not found");
        }
    }
}

fn write_response(stream: &mut TcpStream, status: u16, content_type: &str, body: &[u8]) -> io::Result<()> {
    let status_text = match status {
        200 => "OK",
        404 => "Not Found",
        405 => "Method Not Allowed",
        _ => "OK",
    };
    let header = format!(
        "HTTP/1.1 {} {}\r\nContent-Type: {}\r\nCache-Control: no-store\r\nContent-Length: {}\r\nConnection: close\r\n\r\n",
        status,
        status_text,
        content_type,
        body.len()
    );
    stream.write_all(header.as_bytes())?;
    stream.write_all(body)
}

fn broadcast(shared: &Arc<SharedState>, message: &str) {
    let mut stale = Vec::new();
    let mut clients = shared.clients.lock().unwrap();
    for (index, stream) in clients.iter_mut().enumerate() {
        if send_ws_text(stream, message).is_err() {
            stale.push(index);
        }
    }
    for index in stale.into_iter().rev() {
        let stream = clients.remove(index);
        let _ = stream.shutdown(Shutdown::Both);
    }
}

fn send_ws_text(stream: &mut TcpStream, message: &str) -> io::Result<()> {
    let payload = message.as_bytes();
    let mut frame = Vec::with_capacity(payload.len() + 10);
    frame.push(0x81);
    if payload.len() < 126 {
        frame.push(payload.len() as u8);
    } else if payload.len() < 65536 {
        frame.push(126);
        frame.extend_from_slice(&(payload.len() as u16).to_be_bytes());
    } else {
        frame.push(127);
        frame.extend_from_slice(&(payload.len() as u64).to_be_bytes());
    }
    frame.extend_from_slice(payload);
    stream.write_all(&frame)
}

fn websocket_key(request: &str) -> Option<String> {
    for line in request.lines() {
        let lower = line.to_ascii_lowercase();
        if lower.starts_with("sec-websocket-key:") {
            return Some(line.split_once(':')?.1.trim().to_string());
        }
    }
    None
}

fn websocket_accept(key: &str) -> String {
    let digest = sha1_digest(format!("{}258EAFA5-E914-47DA-95CA-C5AB0DC85B11", key).as_bytes());
    base64_encode(&digest)
}

fn resolve_asset(name: &str) -> PathBuf {
    let mut current = std::env::current_dir().unwrap_or_else(|_| PathBuf::from("."));
    loop {
        let candidate = current.join("dashboard").join("web").join(name);
        if candidate.exists() {
            return candidate;
        }
        if !current.pop() {
            break;
        }
    }
    Path::new(name).to_path_buf()
}

fn json_escape(value: &str) -> String {
    value
        .replace('\\', "\\\\")
        .replace('"', "\\\"")
        .replace('\n', "\\n")
        .replace('\r', "\\r")
}

fn now_millis() -> u128 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap_or_default()
        .as_millis()
}

fn base64_encode(bytes: &[u8]) -> String {
    const TABLE: &[u8; 64] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    let mut out = String::new();
    let mut i = 0;
    while i < bytes.len() {
        let b0 = bytes[i];
        let b1 = if i + 1 < bytes.len() { bytes[i + 1] } else { 0 };
        let b2 = if i + 2 < bytes.len() { bytes[i + 2] } else { 0 };

        out.push(TABLE[(b0 >> 2) as usize] as char);
        out.push(TABLE[(((b0 & 0x03) << 4) | (b1 >> 4)) as usize] as char);
        if i + 1 < bytes.len() {
            out.push(TABLE[(((b1 & 0x0f) << 2) | (b2 >> 6)) as usize] as char);
        } else {
            out.push('=');
        }
        if i + 2 < bytes.len() {
            out.push(TABLE[(b2 & 0x3f) as usize] as char);
        } else {
            out.push('=');
        }
        i += 3;
    }
    out
}

fn sha1_digest(data: &[u8]) -> [u8; 20] {
    let mut h0: u32 = 0x67452301;
    let mut h1: u32 = 0xEFCDAB89;
    let mut h2: u32 = 0x98BADCFE;
    let mut h3: u32 = 0x10325476;
    let mut h4: u32 = 0xC3D2E1F0;

    let mut msg = data.to_vec();
    let bit_len = (msg.len() as u64) * 8;
    msg.push(0x80);
    while (msg.len() % 64) != 56 {
        msg.push(0);
    }
    msg.extend_from_slice(&bit_len.to_be_bytes());

    for chunk in msg.chunks(64) {
        let mut w = [0_u32; 80];
        for i in 0..16 {
            let j = i * 4;
            w[i] = u32::from_be_bytes([chunk[j], chunk[j + 1], chunk[j + 2], chunk[j + 3]]);
        }
        for i in 16..80 {
            w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]).rotate_left(1);
        }

        let mut a = h0;
        let mut b = h1;
        let mut c = h2;
        let mut d = h3;
        let mut e = h4;

        for i in 0..80 {
            let (f, k) = if i < 20 {
                (((b & c) | ((!b) & d)), 0x5A827999)
            } else if i < 40 {
                (b ^ c ^ d, 0x6ED9EBA1)
            } else if i < 60 {
                (((b & c) | (b & d) | (c & d)), 0x8F1BBCDC)
            } else {
                (b ^ c ^ d, 0xCA62C1D6)
            };
            let temp = a
                .rotate_left(5)
                .wrapping_add(f)
                .wrapping_add(e)
                .wrapping_add(k)
                .wrapping_add(w[i]);
            e = d;
            d = c;
            c = b.rotate_left(30);
            b = a;
            a = temp;
        }

        h0 = h0.wrapping_add(a);
        h1 = h1.wrapping_add(b);
        h2 = h2.wrapping_add(c);
        h3 = h3.wrapping_add(d);
        h4 = h4.wrapping_add(e);
    }

    let mut out = [0_u8; 20];
    out[..4].copy_from_slice(&h0.to_be_bytes());
    out[4..8].copy_from_slice(&h1.to_be_bytes());
    out[8..12].copy_from_slice(&h2.to_be_bytes());
    out[12..16].copy_from_slice(&h3.to_be_bytes());
    out[16..20].copy_from_slice(&h4.to_be_bytes());
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_snapshot_builder_contains_expected_fields() {
        let chart = Chart::new();
        let snapshot = build_snapshot_json(&chart, None, 1);
        assert!(snapshot.contains("\"dashboard.snapshot\""));
        assert!(snapshot.contains("\"binding\":\"rust\""));
        assert!(snapshot.contains("\"column_count\":0"));
    }
}
