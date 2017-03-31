#!/usr/bin/make -f
# -*- mode:makefile -*-

red-toroide:
	rm red_toroide
	mpicc red_toroide.c -o red_toroide

red-hipercubo:
	rm red_hipecubo
	mpicc red_hipecubo.c -o red_hipecubo

clean:
	rm red_toroide
	rm red_hipecubo

all: red-toroide red-hipercubo
