#!/usr/bin/env bash
set -euo pipefail

# BattleOfTheFlatland/ -> Junior/ -> WorkTraining/ -> <parent>/LIB
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PARENT_DIR="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
LIB_DIR="${PARENT_DIR}/LIB"

echo "[botf] LIB directory: ${LIB_DIR}"
mkdir -p "${LIB_DIR}"
cd "${LIB_DIR}"

clone_if_missing() {
  local name="$1"
  local url="$2"
  if [[ -d "${name}" ]]; then
    echo "[botf] ${name} already present — skip clone"
    return 0
  fi
  echo "[botf] Cloning ${name}..."
  git clone --depth 1 "${url}" "${name}"
}

clone_if_missing SFML https://github.com/SFML/SFML.git
clone_if_missing googletest https://github.com/google/googletest.git

if [[ -d protobuf ]]; then
  echo "[botf] protobuf already present — skip clone"
else
  echo "[botf] Cloning protobuf..."
  git clone --depth 1 https://github.com/protocolbuffers/protobuf.git
  echo "[botf] Initializing protobuf submodules..."
  (cd protobuf && git submodule update --init --recursive)
fi

if command -v apt-get >/dev/null 2>&1; then
  echo "[botf] Optional: sudo apt-get install -y libsfml-dev protobuf-compiler libprotobuf-dev"
fi

echo "[botf] Done. CMake will use BOTF_LIB_ROOT=${LIB_DIR}"
