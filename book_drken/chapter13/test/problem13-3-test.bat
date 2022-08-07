@echo off
echo [test1]Excepted:Yes
..\problem13-3.exe < code13-5_in_Yes.txt
echo.
echo [test2]Excepted:No
..\problem13-3.exe < code13-5_in2_No.txt
echo.
echo [test3]Excepted:No
..\problem13-3.exe < code13-5_in3_No.txt
