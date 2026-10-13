#!/usr/bin/env bash
set -euo pipefail

# Resolve LIB the same way as cmake/BotfLibRoot.cmake

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

lib_looks_ok() {
  local d="$1"
  [[ -d "${d}/SFML" || -d "${d}/sfml" || -d "${d}/googletest" || -d "${d}/protobuf" || -f "${d}/README.md" ]]
}

if [[ -n "${BOTF_LIB_ROOT:-}" ]]; then
  LIB_DIR="${BOTF_LIB_ROOT}"
else
  LIB_CANONICAL="$(cd "${SCRIPT_DIR}/../../.." && pwd)/LIB"
  LIB_FLAT="$(cd "${SCRIPT_DIR}/.." && pwd)/LIB"
  if lib_looks_ok "${LIB_CANONICAL}"; then
    LIB_DIR="${LIB_CANONICAL}"
  elif lib_looks_ok "${LIB_FLAT}"; then
    LIB_DIR="${LIB_FLAT}"
  else
    LIB_DIR="${LIB_CANONICAL}"
  fi
fi

PARENT_DIR="$(cd "${LIB_DIR}/.." && pwd)"
if [[ -n "${CMAKE_BUILDS_ROOT:-}" ]]; then
  BUILD_DIR="${CMAKE_BUILDS_ROOT}"
else
  BUILD_DIR="${PARENT_DIR}/BUILD"
fi

echo "[botf] LIB directory:   ${LIB_DIR}"
echo "[botf] BUILD directory: ${BUILD_DIR}"
mkdir -p "${LIB_DIR}" "${BUILD_DIR}"
cd "${LIB_DIR}"

sfml_present() {
  [[ -d SFML || -d sfml ]] && return 0
  compgen -G 'SFML-*' >/dev/null 2>&1
}

clone_if_missing() {
  local name="$1"
  local url="$2"
  if [[ "${name}" == "SFML" ]] && sfml_present; then
    echo "[botf] SFML already present in ${LIB_DIR} — skip clone"
    return 0
  fi
  if [[ -d "${name}" ]]; then
    echo "[botf] ${name} already present in ${LIB_DIR} — skip clone"
    return 0
  fi
  echo "[botf] Cloning ${name} into ${LIB_DIR}/${name} ..."
  git clone --depth 1 "${url}" "${name}"
}

clone_if_missing SFML https://github.com/SFML/SFML.git
clone_if_missing googletest https://github.com/google/googletest.git

if [[ -d protobuf ]]; then
  echo "[botf] protobuf already present — skip clone"
else
  echo "[botf] Cloning protobuf into ${LIB_DIR}/protobuf ..."
  git clone --depth 1 https://github.com/protocolbuffers/protobuf.git
  echo "[botf] Initializing protobuf submodules..."
  (cd protobuf && git submodule update --init --recursive)
fi

if command -v apt-get >/dev/null 2>&1; then
  echo "[botf] Optional: sudo apt-get install -y libsfml-dev protobuf-compiler libprotobuf-dev"
fi

echo "[botf] Done."
echo "[botf] LIB=${LIB_DIR}"
if [[ -n "${CMAKE_BUILDS_ROOT:-}" ]]; then
  echo "[botf] CMAKE_BUILDS_ROOT=${CMAKE_BUILDS_ROOT}"
  echo "[botf] Use preset: cmake --preset debug"
else
  echo "[botf] WARNING: CMAKE_BUILDS_ROOT is not set — set it to your BUILD folder."
fi
