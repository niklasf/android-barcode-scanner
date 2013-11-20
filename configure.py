#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

def write_node_config(backend):
    f = open("config.js", "w")
    f.write("exports.BACKEND = '%s';" % backend);
    f.write(os.linesep)
    f.close()

def write_android_config(backend):
    f = open("android/src/de/tuc/barcodescanner/Config.java", "w")
    f.write("public sattic class Config {")
    f.write(os.linesep)
    f.write(os.linesep)
    f.write("    public static final String BACKEND = \"%s\";" % backend)
    f.write(os.linesep)
    f.write(os.linesep)
    f.write("}")
    f.write(os.linesep)
    f.close()

def write_client_config(backend):
    f = open("client/config.h", "w")
    f.write("#define BACKEND \"%s\"" % backend)
    f.write(os.linesep)
    f.close()

if __name__ == "__main__":
    backend = open("backend.txt").read().strip()
    write_node_config(backend)
    write_android_config(backend)
    write_client_config(backend)