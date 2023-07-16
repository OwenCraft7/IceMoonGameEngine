@echo off
PATH C:\WATCOM\BINW
SET INCLUDE=C:\WATCOM\H
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
cd bin
cls
wcl386 /fe=icemoon -l=dos32a -onatx -oh -ot -oi -ou -ob -om -ol+ -wx -zp4 -et ..\src\main.c
pause