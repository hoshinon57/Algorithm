@echo off
echo [test1]Excepted:48
..\problem6-3.exe < problem6-3_in1.txt
echo.
echo [test2]Excepted:20
..\problem6-3.exe < problem6-3_in2.txt
echo.
echo [test3]Excepted:9999999
..\problem6-3.exe < problem6-3_in3.txt
echo.
echo [test4]Excepted:99999992
..\problem6-3.exe < problem6-3_in4.txt
echo.
