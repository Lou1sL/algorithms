#!/bin/sh
make clean && make && gdbserver localhost:2000 ./build/app.elf