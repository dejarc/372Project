@echo off
 
set gtk_ver=gtk+-3.0
pkg-config %gtk_ver% --cflags --libs >tmp.txt
set /p pkg-info= <tmp.txt
del tmp.txt
 
rem echo %pkg-info%
gcc main.c pc.c reg.c test.c ir.c fsm.c alu.c mem.c gui.c LC2200.c bit.c asm.c -o main.exe -Wall %pkg-info% -std=c99