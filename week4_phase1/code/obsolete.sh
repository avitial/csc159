#!/bin/bash

# remove files so env is clean for spede-mkmf execution
rm GDB159.RC
rm Makefile
rm make.orig
rm events.o
rm main.o
rm handlers.o
rm GidOS.dli
spede-mkmf -q
make
flash GidOS.dli
