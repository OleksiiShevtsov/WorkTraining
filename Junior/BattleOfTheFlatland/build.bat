@echo off
setlocal EnableExtensions

rem Build via CMAKE_BUILDS_ROOT (same as CrashCourse CMakePresets.json).

if not defined CMAKE_BUILDS_ROOT (
  echo [botf] CMAKE_BUILDS_ROOT is not set. Set the global env var to your BUILD folder.
  exit /b 1
)

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "PRESET=debug"
if /I "%~1"=="release" set "PRESET=release"
if /I "%~1"=="debug" set "PRESET=debug"

echo [botf] CMAKE_BUILDS_ROOT=%CMAKE_BUILDS_ROOT%
echo [botf] preset=%PRESET%

cmake --preset %PRESET% -S "%SCRIPT_DIR%"
if errorlevel 1 exit /b 1
cmake --build --preset %PRESET% 2>nul
if errorlevel 1 cmake --build "%CMAKE_BUILDS_ROOT%\BattleOfTheFlatland\%PRESET%"
exit /b %ERRORLEVEL%
