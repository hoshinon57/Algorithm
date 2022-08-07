@echo off
echo [test1]Excepted:Yes
..\problem13-2.exe < code13-4_in_Yes.txt
echo.
echo [test2]Excepted:No
..\problem13-2.exe < code13-4_in2_No.txt
