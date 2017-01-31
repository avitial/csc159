#!/bin/bash

# remove files so env is clean for spede-mkmf execution
rm GDB159.RC
rm Makefile
rm make.orig
rm sample.o
rm MyOS.dli
spede-mkmf -q
make
flash MyOS.dli
