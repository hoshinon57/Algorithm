@echo off
echo [test1]
..\code14-5.exe < code14-5_in.txt
echo.
echo [test2]
..\code14-5.exe < code14-5_in2.txt
echo.
echo [test3]Expected:exist negative cycle
..\code14-5.exe < code14-5_in3.txt
echo.
echo [test4]
..\code14-5.exe < code14-5_in4.txt
