#!/usr/bin/make -f
# -*- mode:makefile -*-

DIREXE := exec/
CC := mpicc

all: red_toroide red_hipercubo

dirs:
	mkdir -p $(DIREXE)

red_toroide: dirs
	$(CC) red_toroide.c -o $(DIREXE)$@

red_hipercubo: dirs
	$(CC) red_hipercubo.c -o $(DIREXE)$@ -lm

test_red_toroide:
	mpirun -n 16 $(DIREXE)red_toroide

test_red_hipercubo:
	mpirun -n 16 $(DIREXE)red_hipercubo

clean:
	rm -rf $(DIREXE) *~
