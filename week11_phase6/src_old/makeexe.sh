#!/bin/bash

# make files with spede-mkmf execution
spede-mkmf -q
sed -ie 's/MyOS/GidOS/g' Makefile
make
flash GidOS.dli
