#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import shutil
import sys

def dist_node():
    if not os.path.exists("dist"):
        os.makedirs("dist")

    if not os.path.isfile("config.js"):
        print "[ FAILURE ] Node app: Run ./configure.py to create config.js"
        sys.exit(1)

    shutil.copy(".buildpacks", "dist")
    shutil.copy("channels.js", "dist")
    shutil.copy("config.js", "dist")
    shutil.copy("LICENSE", "dist")
    shutil.copy("package.json", "dist")
    shutil.copy("Procfile", "dist")
    shutil.copy("web.js", "dist")

    print "[   NEW   ] Node app"

def dist_android():
    if not os.path.exists("dist/android/bin"):
        os.makedirs("dist/android/bin")

    if os.path.exists("android/bin/BarcodeScanner.apk"):
        shutil.copy("android/bin/BarcodeScanner.apk", "dist/android/bin")
        print "[   NEW   ] Android app"
    elif os.path.isfile("dist/android/bin/BarcodeScanner.apk"):
        print "[   OLD   ] Android app: dist/android/bin/BarcodeScanner.apk"
    else:
        print "[ WARNING ] Android app: Did not find android/bin/BarcodeScanner.apk"

def dist_windows():
    if not os.path.exists("dist/client/release"):
        os.makedirs("dist/client/release")

    if os.path.exists("client/release/android-barcode-scanner-client.exe"):
        shutil.copy("client/release/android-barcode-scanner-client.exe", "dist/client/release")
        print "[   NEW   ] Windows client"
    elif os.path.isfile("dist/client/release/android-barcode-scanner-client.exe"):
        print "[   OLD   ] Windows client: dist/client/release/android-...-client.exe"
    else:
        print "[ WARNING ] Windows client: Did not find client/release/android-...-client.exe"

if __name__ == "__main__":
    dist_node()
    dist_android()
    dist_windows()
