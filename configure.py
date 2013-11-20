#!/usr/bin/env python
# -*- coding: utf-8 -*-

def write_node_config(backend):
    f = open("config.js", "w")
    f.write("exports.BACKEND = '%s';\n" % backend);
    f.close()

def write_android_config(backend):
    f = open("android/src/de/tuc/barcodescanner/Config.java", "w")
    f.write("package de.tuc.barcodescanner;\n")
    f.write("\n")
    f.write("public class Config {\n")
    f.write("\n")
    f.write("    public static final String BACKEND = \"%s\";\n" % backend)
    f.write("\n")
    f.write("}\n")
    f.close()

def write_client_config(backend):
    f = open("client/config.h", "w")
    f.write("#define BACKEND \"%s\"\n" % backend)
    f.close()

if __name__ == "__main__":
    backend = open("backend.txt").read().strip()
    write_node_config(backend)
    write_android_config(backend)
    write_client_config(backend)