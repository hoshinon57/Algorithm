@echo off
echo [test1]Excepted:2
..\problem7-1.exe < problem7-1-test1.txt
echo.
echo [test2]Excepted:0
..\problem7-1.exe < problem7-1-test2.txt
