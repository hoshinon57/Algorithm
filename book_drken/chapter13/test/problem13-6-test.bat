@echo off
echo [test1]Excepted:"6->7->8->6"
..\problem13-6.exe < problem13-6-test1.txt
echo.
echo [test2]Excepted:"5->11->7->6->5"
..\problem13-6.exe < problem13-6-test2.txt
echo.
echo [test3]Excepted:No cycle
..\problem13-6.exe < problem13-6-test3.txt
