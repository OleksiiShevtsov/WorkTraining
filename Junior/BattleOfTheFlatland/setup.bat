@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem Resolve LIB the same way as cmake/BotfLibRoot.cmake:
rem   1) BOTF_LIB_ROOT env
rem   2) <parent>/LIB  (WorkTraining/Junior/BattleOfTheFlatland -> ../../../LIB)
rem   3) <repo>/LIB    (flat Cursor layout -> ../LIB)

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

if defined BOTF_LIB_ROOT (
  set "LIB_DIR=%BOTF_LIB_ROOT%"
  goto :lib_ready
)

for %%I in ("%SCRIPT_DIR%\..\..\..\LIB") do set "LIB_CANONICAL=%%~fI"
for %%I in ("%SCRIPT_DIR%\..\LIB") do set "LIB_FLAT=%%~fI"

call :lib_looks_ok "%LIB_CANONICAL%"
if "!LIB_OK!"=="1" (
  set "LIB_DIR=%LIB_CANONICAL%"
  goto :lib_ready
)

call :lib_looks_ok "%LIB_FLAT%"
if "!LIB_OK!"=="1" (
  set "LIB_DIR=%LIB_FLAT%"
  goto :lib_ready
)

rem Prefer canonical next to WorkTraining
set "LIB_DIR=%LIB_CANONICAL%"

:lib_ready
for %%I in ("%LIB_DIR%\..") do set "PARENT_DIR=%%~fI"
set "BUILD_DIR=%PARENT_DIR%\BUILD"
if defined CMAKE_BUILDS_ROOT set "BUILD_DIR=%CMAKE_BUILDS_ROOT%"

set "PROTO_FRESH=0"

echo [botf] LIB directory:   %LIB_DIR%
echo [botf] BUILD directory: %BUILD_DIR%
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

pushd "%LIB_DIR%"

call :ensure_dep SFML "https://github.com/SFML/SFML.git"
if errorlevel 1 goto :fail

call :ensure_dep googletest "https://github.com/google/googletest.git"
if errorlevel 1 goto :fail

if exist "protobuf\" (
  echo [botf] protobuf already present — skip clone
) else (
  echo [botf] Cloning protobuf into %LIB_DIR%\protobuf ...
  git clone --depth 1 https://github.com/protocolbuffers/protobuf.git
  if errorlevel 1 goto :fail
  set "PROTO_FRESH=1"
)

if "%PROTO_FRESH%"=="1" (
  echo [botf] Initializing protobuf submodules...
  pushd protobuf
  git submodule update --init --recursive
  if errorlevel 1 (
    popd
    goto :fail
  )
  popd
)

popd
echo [botf] Done.
echo [botf] LIB=%LIB_DIR%
if defined CMAKE_BUILDS_ROOT (
  echo [botf] CMAKE_BUILDS_ROOT=%CMAKE_BUILDS_ROOT%
  echo [botf] Use preset: cmake --preset debug
) else (
  echo [botf] WARNING: CMAKE_BUILDS_ROOT is not set — set it to your BUILD folder.
)
endlocal
exit /b 0

:fail
popd 2>nul
echo [botf] Setup failed. Check that LIB path above is your real LIB folder.
endlocal
exit /b 1

:lib_looks_ok
set "LIB_OK=0"
if exist "%~1\SFML\" set "LIB_OK=1"
if exist "%~1\sfml\" set "LIB_OK=1"
if exist "%~1\googletest\" set "LIB_OK=1"
if exist "%~1\protobuf\" set "LIB_OK=1"
if exist "%~1\README.md" set "LIB_OK=1"
exit /b 0

:ensure_dep
set "DEP_NAME=%~1"
set "DEP_URL=%~2"
if exist "%DEP_NAME%\" (
  echo [botf] %DEP_NAME% already present in %LIB_DIR% — skip clone
  exit /b 0
)
rem Accept alternate folder names for SFML (e.g. SFML-2.6.1)
if /I "%DEP_NAME%"=="SFML" (
  for /d %%D in (SFML-*) do (
    echo [botf] Found %%D — treat as SFML, skip clone
    exit /b 0
  )
  if exist "sfml\" (
    echo [botf] sfml already present — skip clone
    exit /b 0
  )
)
echo [botf] Cloning %DEP_NAME% into %LIB_DIR%\%DEP_NAME% ...
git clone --depth 1 "%DEP_URL%" "%DEP_NAME%"
exit /b %ERRORLEVEL%
