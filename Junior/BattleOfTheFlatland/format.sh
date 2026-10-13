#!/usr/bin/env bash
set -euo pipefail

# Format all BOTF C++ sources with clang-format (.clang-format in this folder).

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

if ! command -v clang-format >/dev/null 2>&1; then
  echo "[botf] clang-format not found on PATH." >&2
  echo "[botf] e.g. sudo apt-get install -y clang-format" >&2
  exit 1
fi

echo "[botf] Formatting with:"
clang-format --version

mapfile -t files < <(find . -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) ! -path './build/*')
if [[ ${#files[@]} -eq 0 ]]; then
  echo "[botf] No sources found."
  exit 0
fi

clang-format -i "${files[@]}"
echo "[botf] Formatted ${#files[@]} file(s)."
