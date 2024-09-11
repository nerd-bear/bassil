@echo off
cls
echo Compiling program...
g++ ./src/main.cpp ./src/cpp/utils.cpp ./src/cpp/lexer.cpp ./src/cpp/error_report.cpp -o C:/coding-projects/CPP-Dev/bassil/build/Bassil-Main-Build-ORS-A01 -lgdi32 -luser32 -lshell32
cls
echo Compiled program successfuly!
timeout /t 1 /nobreak
cls
C:/coding-projects/CPP-Dev/bassil/build/Bassil-Main-Build-ORS-A01
echo Program Ended