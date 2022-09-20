@echo off
rem 図16.7のグラフ
echo [test1]Excepted:9
..\code16-1.exe < code16-1_in.txt
echo.

rem 16.5章「二部マッチング」 図16.15をグラフ化したもの
echo [test2]Excepted:4
..\code16-1.exe < code16-1_in2.txt
echo.
