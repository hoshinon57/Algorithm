@echo off
echo [test1]Excepted:0:0, 1:3, 2:5, 3:14, 4:9, 5:16
..\code14-3.exe < code14-3_in.txt
echo.
echo [test2]Excepted:0:0, 1:3, 2:5, 3:13, 4:INF, 5:INF
..\code14-3.exe < code14-3_in2.txt
