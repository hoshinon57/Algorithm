@echo off
echo [test1]Excepted:Yes
..\code13-4.exe < code13-4_in_Yes.txt
echo.
echo [test2]Excepted:No
..\code13-4.exe < code13-4_in2_No.txt
