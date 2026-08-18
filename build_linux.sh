#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
build_dir="${1:-$root_dir/build-linux}"

qmake_bin="${QMAKE:-}"
if [[ -z "$qmake_bin" ]]; then
    if command -v qmake6 >/dev/null 2>&1; then
        qmake_bin=qmake6
    elif command -v qmake >/dev/null 2>&1; then
        qmake_bin=qmake
    else
        echo "Qt qmake was not found. Install Qt 6 development packages." >&2
        exit 1
    fi
fi

mkdir -p "$build_dir"
"$qmake_bin" "$root_dir/BDPatient.pro" -o "$build_dir/Makefile"
make -C "$build_dir" -j"${JOBS:-$(nproc)}"

echo "Build completed: $build_dir/base"
