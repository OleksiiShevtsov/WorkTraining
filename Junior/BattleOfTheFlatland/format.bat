@echo off
setlocal EnableExtensions

rem Format all BOTF C++ sources with clang-format (.clang-format in this folder).
rem Requires clang-format on PATH (LLVM installer or VS "C++ Clang tools").

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

where clang-format >nul 2>&1
if errorlevel 1 (
  echo [botf] clang-format not found on PATH.
  echo [botf] Install LLVM or enable "C++ Clang Compiler for Windows" in Visual Studio Installer.
  exit /b 1
)

echo [botf] Formatting with:
clang-format --version

set "COUNT=0"
for /r "%SCRIPT_DIR%" %%F in (*.cpp *.h *.hpp) do (
  echo   %%F
  clang-format -i "%%F"
  set /a COUNT+=1
)

echo [botf] Formatted %COUNT% file^(s^).
endlocal
exit /b 0
