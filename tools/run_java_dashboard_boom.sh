#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HOST="${1:-0.0.0.0}"
PORT="${2:-8763}"
JAR="$ROOT/bindings/java/target/pnf-chart-system-0.1.0.jar"
JAVA_CLASSES="$ROOT/bindings/java/target/classes"
LIB_CP="$JAVA_CLASSES:$JAR"
CLASS_DIR="$ROOT/build-linux/java-dashboard"
SRC="$ROOT/tools/RunJavaDashboardBoom500.java"

mkdir -p "$CLASS_DIR"

javac -cp "$LIB_CP" -d "$CLASS_DIR" "$SRC"

export LD_LIBRARY_PATH="$ROOT/build-linux/lib:${LD_LIBRARY_PATH:-}"
exec java \
  -Djava.library.path="$ROOT/build-linux/lib" \
  -cp "$CLASS_DIR:$LIB_CP" \
  RunJavaDashboardBoom500 "$HOST" "$PORT"
