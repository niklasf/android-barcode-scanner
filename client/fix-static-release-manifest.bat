rem Build a static version of Qt
rem ============================
rem
rem 1. Download the Qt library source code to C:\Qt\4.8.5
rem    Source: http://qt-project.org/downloads#qt-lib
rem
rem 2. Download OpenSSL to C:\OpenSSL
rem    Source: http://slproweb.com/products/Win32OpenSSL.html
rem
rem 3. Build in C:\Qt\4.8.5
rem
rem    configure -openssl -I C:\OpenSSL\include -L C:\OpenSSL -static -release -platform win32-msvc2012
rem    nmake sub-src

rem Build a static application
rem ==========================
rem qmake.exe -spec win32-msvc2012 android-barcode-scanner-client.pro 
rem nmake release

rem Fix the manifest
rem ================
cd release
mt.exe -manifest android-barcode-scanner-client.exe.manifest -outputresource:android-barcode-scanner-client.exe;1
cd ..