@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem Configure + build into <parent>\BUILD\BattleOfTheFlatland (sibling of LIB), not inside the project.

set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

if defined BOTF_LIB_ROOT (
  set "LIB_DIR=%BOTF_LIB_ROOT%"
) else (
  for %%I in ("%SCRIPT_DIR%\..\..\..\LIB") do set "LIB_CANONICAL=%%~fI"
  for %%I in ("%SCRIPT_DIR%\..\LIB") do set "LIB_FLAT=%%~fI"
  if exist "!LIB_CANONICAL!\SFML\" (
    set "LIB_DIR=!LIB_CANONICAL!"
  ) else if exist "!LIB_FLAT!\SFML\" (
    set "LIB_DIR=!LIB_FLAT!"
  ) else if exist "!LIB_CANONICAL!\" (
    set "LIB_DIR=!LIB_CANONICAL!"
  ) else (
    set "LIB_DIR=!LIB_FLAT!"
  )
)

for %%I in ("%LIB_DIR%\..") do set "PARENT_DIR=%%~fI"
if defined CMAKE_BUILDS_ROOT (
  set "BUILD_ROOT=%CMAKE_BUILDS_ROOT%"
) else (
  set "BUILD_ROOT=%PARENT_DIR%\BUILD"
)
set "BUILD_DIR=%BUILD_ROOT%\BattleOfTheFlatland"

echo [botf] LIB:   %LIB_DIR%
echo [botf] BUILD: %BUILD_DIR%

if not exist "%LIB_DIR%\SFML\" if not exist "%LIB_DIR%\sfml\" (
  echo [botf] SFML not found under LIB. Run setup.bat first — or set BOTF_LIB_ROOT to your LIB.
  exit /b 1
)

cmake -S "%SCRIPT_DIR%" -B "%BUILD_DIR%" -DBOTF_LIB_ROOT="%LIB_DIR%" %*
if errorlevel 1 exit /b 1
cmake --build "%BUILD_DIR%"
exit /b %ERRORLEVEL%
