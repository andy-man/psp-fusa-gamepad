@echo off

SET root=%cd%
SET bin_path=%root%\bin
SET src_path=%root%\src

ECHO Cleaning...
ECHO.
call clear.bat

ECHO.
ECHO Compiling PRX
ECHO.
cd %src_path%/prx
C:/pspsdk/bin/make
ECHO %src_path%\prx\usbgamepad.prx
copy %src_path%\prx\usbgamepad.prx %bin_path%\usbgamepad.prx

ECHO.
ECHO Compiling EBOOT
ECHO.
cd %src_path%/app
C:/pspsdk/bin/make
copy %src_path%\app\EBOOT.pbp %bin_path%\EBOOT.pbp

ECHO.
ECHO All done. Look at BIN folder.
ECHO.

pause