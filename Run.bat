@echo off
set platformExe=%USERPROFILE%\.platformio\penv\Scripts\platformio.exe

if not Exist "BuildNum.txt" (
    echo 10 > BuildNum.txt
)
%platformExe% run --target clean 
%platformExe% run
