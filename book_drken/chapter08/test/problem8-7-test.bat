@echo off
echo [test1]Excepted:Yes
..\problem8-7.exe < problem8-7-test1.txt
echo.
echo [test2]Excepted:No
..\problem8-7.exe < problem8-7-test2.txt
