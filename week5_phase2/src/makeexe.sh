#!/bin/bash

# spede-mkmf execution and flashing GidOS.dli to target 
spede-mkmf -q
sed -ie 's/MyOS/GidOS/g' Makefile
make
flash GidOS.dli
