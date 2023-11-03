@echo off
PATH C:\WATCOM\BINW
SET INCLUDE=C:\WATCOM\H
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
cd bin
cls
wcl386 /fe=icemoon -l=dos32a -onatx -oh -oi -on -ot -ei -et -zp8 -5 -fp5 -wx ..\src\main.c
pause