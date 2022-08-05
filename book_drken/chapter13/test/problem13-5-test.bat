@echo off
echo [test1]Excepted:Yes
..\code13-5.exe < code13-5_in_Yes.txt
echo.
echo [test2]Excepted:No
..\code13-5.exe < code13-5_in2_No.txt
echo.
echo [test3]Excepted:No
..\code13-5.exe < code13-5_in3_No.txt
