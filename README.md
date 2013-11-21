Android Barcode Scanner
=======================

Use an android device as a wireless barcode scanner.

Install on your smartphone
--------------------------

 1. Enable unknown sources in security settings.

 2. Download and install [BarcodeScanner.apk](https://android-barcode-scanner.herokuapp.com/BarcodeScanner.apk).

Install client on windows
-------------------------

 1. Install [32-bit Visual Studio 2012 redistributables](https://android-barcode-scanner.herokuapp.com/vcredist.exe).

 2. Start the [client app](https://android-barcode-scanner.herokuapp.com/android-barcode-scanner-client.exe).

REST API
--------

``/channel/<random-64-hex-character-token>/qr.png`` is an image of a QR code
ending with ``#<random-64-hex-character-token>``.

The windows client generates a random token and keeps sending ``DELETE``
requests to ``/channel/<token>/`` (long polling). The response body
has one scanned code per line as unicode plain text.
It is also possible to ``GET`` the last few codes from ``/channel/<token>/``.
The timeout is between 15 and 30 seconds.

The android app gets the channel token from the inital QR code. All
following scanned codes will be sent via ``PUT`` to ``/channel/<token>/``.

HTTPS is recommended as there are no other security measures.

Author, license, bug reports
----------------------------
Report bugs to niklas (dot) fiekas (at) tu-clausthal (dot) de or
[fork me on GitHub](https://github.com/niklasf/android-barcode-scanner/).
android-barcode-scanner is licensed under the GPL-3.
