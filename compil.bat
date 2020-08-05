cls
@echo off
echo -------- Compilation of : 
echo %1
echo .
SET HEX2BINDIR=
SET HEX2BIN=hex2bin.exe 
SET ASM=sdasz80 
SET CC=sdcc 
SET DEST=dsk\

SET INCLUDEDIR=fusion-c\include\
SET LIBDIR=fusion-c\lib\

SET proga=main


rem archivo a unir
REM sdcc -mz80 --no-std-crt0 -c enemy.c
rem copy enemy.rel fusion-c\include\ /y
rem del enemy.asm
rem del enemy.lst
rem del enemy.sym
rem del enemy.rel
rem Final del archivo a unir


rem ******cabecera .com*******
SET INC1=%INCLUDEDIR%crt0_msxdos.rel
rem ******cabecera .bin*******
rem SET INC2=%INCLUDEDIR%crt0_bin.rel
rem SET INC3=%INCLUDEDIR%enemy.rel
REM SET INC4=%INCLUDEDIR%
REM SET INC5=%INCLUDEDIR%
REM SET INC6=%INCLUDEDIR%
REM SET INC7=%INCLUDEDIR%
REM SET INC8=%INCLUDEDIR%
REM SET INC9=%INCLUDEDIR%
REM SET INCA=%INCLUDEDIR%
REM SET INCB=%INCLUDEDIR%
REM SET INCC=%INCLUDEDIR%
REM SET INCD=%INCLUDEDIR%
REM SET INCE=%INCLUDEDIR%
REM SET INCF=%INCLUDEDIR%



rem ------com---------- 
SET ADDR_CODE=0x106
SET ADDR_DATA=0x0
rem ------com----------
rem *********BIN*********
rem SET ADDR_CODE=0x4020
rem SET ADDR_DATA=0xc000
rem ********/BIN*********



SDCC --code-loc %ADDR_CODE% --data-loc %ADDR_DATA% --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size  -L %LIBDIR% fusion.lib %INC1% %INC2% %INC3% %INC4% %INC5% %INC6% %INC7% %INC8% %INC9% %INCA% %INCB% %INCC% %INCD% %INCE% %INCF% %proga%.c



SET cpath=%~dp0


IF NOT EXIST %proga%.ihx GOTO _end_
echo ... Compilation OK
@echo on





rem ------com----------
Tools\hex2bin -e com %proga%.ihx
rem -----Fin com----------
rem ------bin----------
rem hex2bin -e bin %proga%.ihx
rem -----Fin bin----------





@echo off





rem *********COM*********
copy %proga%.com DSK\%proga%.com /y
rem ********/COM********
rem *********BIN*********
rem copy %proga%.bin dsk\%proga%.bin /y
rem ********/BIN*********
rem *********ROM*********
rem copy /b Tools\rom_header.bin + /b dsk\%proga%.bin /b dsk\%proga%.rom
rem Lo rellenamos de bytes hasta 32768 (32Kb)
rem start /wait Tools\fillfile dsk\%proga%.rom 32768 
rem ********Fin ROM*********



rem *Esta sección no hace falta porque los empuladores modernos 
rem *pueden trabajar sin .dsk en la opción dir as disk

rem *********COM*********
rem Tools\Disk-Manager\DISKMGR.exe -A -F -C main.dsk AUTOEXEC.bat
rem Tools\Disk-Manager\DISKMGR.exe -A -F -C main.dsk main.com
rem ********/BIN*********

rem *********BIN*********
rem Tools\Disk-Manager\DISKMGR.exe -A -F -C main.dsk AUTOEXEC.bas
rem Tools\Disk-Manager\DISKMGR.exe -A -F -C main.dsk main.bin
rem ********/BIN*********


del %proga%.com
del %proga%.asm
del %proga%.ihx
del %proga%.lk
del %proga%.lst
del %proga%.map
del %proga%.noi
del %proga%.sym
del %proga%.rel

echo Done.

:Emulator
Set MyProcess=openmsx.exe
tasklist | find /i "%MyProcess%">nul  && (echo %MyProcess% Already running) || start Tools\openmsx\openmsx.exe -script openMSX\emul_start_config.txt

:_end_