#!/usr/bin/make -f
# -*- mode:makefile -*-

red-toroide:
	mpicc red_toroide.c -o red_toroide

red-hipercubo:
	mpicc red_hipercubo.c -o red_hipercubo -lm

clean:
	rm red_toroide
	rm red_hipercubo

all: red-toroide red-hipercubo
