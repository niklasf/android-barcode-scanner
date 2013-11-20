C:\Qt\4.8.5\bin\qmake.exe android-barcode-scanner-client.pro
nmake release
cd release
mt.exe -manifest android-barcode-scanner-client.exe.manifest -outputresource:android-barcode-scanner-client.exe;1
cd ..