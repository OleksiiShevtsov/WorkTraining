@echo off
setlocal EnableExtensions

rem BattleOfTheFlatland\ -> Junior\ -> WorkTraining\ -> <parent>\LIB
set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..\..") do set "PARENT_DIR=%%~fI"
set "LIB_DIR=%PARENT_DIR%\LIB"
set "PROTO_FRESH=0"

echo [botf] LIB directory: %LIB_DIR%
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"

pushd "%LIB_DIR%"

if exist "SFML\" (
  echo [botf] SFML already present — skip clone
) else (
  echo [botf] Cloning SFML...
  git clone --depth 1 https://github.com/SFML/SFML.git
  if errorlevel 1 goto :fail
)

if exist "googletest\" (
  echo [botf] googletest already present — skip clone
) else (
  echo [botf] Cloning googletest...
  git clone --depth 1 https://github.com/google/googletest.git
  if errorlevel 1 goto :fail
)

if exist "protobuf\" (
  echo [botf] protobuf already present — skip clone
) else (
  echo [botf] Cloning protobuf...
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
echo [botf] Done. CMake will use BOTF_LIB_ROOT=%LIB_DIR%
endlocal
exit /b 0

:fail
popd
echo [botf] Setup failed.
endlocal
exit /b 1
