@echo off
echo [test1]Excepted:0:0, 1:3 2:53, 3:24, 4:-1, 5:7
..\code14-2.exe < code14-2_in.txt
echo.
echo [test2]Excepted:exist negative cycle
..\code14-2.exe < code14-2_in2.txt
echo.
echo [test3]Excepted:0:0, 1:3, 2:5, 3:13, 4:INF, 5:INF
..\code14-2.exe < code14-2_in3.txt
