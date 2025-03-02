@echo off
set PROTO_PATH=messages
set OUT_PATH=notifications

if not exist %OUT_PATH% (
    mkdir %OUT_PATH%
)

for %%f in (%PROTO_PATH%\*.proto) do (
    echo Generating C++ files for %%f
    protoc -I=%PROTO_PATH% --cpp_out=%OUT_PATH% %%f
)

echo Generation completed.
pause