#!/bin/sh
gcc -O2 -g -o player player.c  -L. -lgcc_s -lvlc -lcurses -lpthread
