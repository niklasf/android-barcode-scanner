rem Build a static version of Qt
rem ============================
rem
rem 1. Download the Qt library source code to C:\Qt4.8.5.
rem    Source: http://qt-project.org/downloads#qt-lib
rem
rem 2. Download OpenSSL to C:\OpenSSL.
rem    Source: http://slproweb.com/products/Win32OpenSSL.html
rem
rem 3. Build in C:\Qt4.8.5:
rem
rem    configure -openssl -static -release -platform win32-msvc2012
rem    nmake sub-src

rem Build a static application
rem ==========================

C:\Qt4.8.5\bin\qmake.exe -spec win32-msvc2012 android-barcode-scanner-client.pro 
nmake release
cd release
mt.exe -manifest android-barcode-scanner-client.exe.manifest -outputresource:android-barcode-scanner-client.exe;1
cd ..