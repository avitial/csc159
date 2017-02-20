#!/bin/bash

# remove files so env is clean for spede-mkmf execution
rm GDB159.RC
rm Makefile
rm make.orig
rm MyOS.dli
rm events.o
rm handlers.o
rm main.o
spede-mkmf -q
make
flash MyOS.dli
