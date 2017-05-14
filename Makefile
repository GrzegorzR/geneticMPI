CC      = g++
CFLAGS  = -lm -std=gnu++11
LDFLAGS = -lfltk

all: simple

simple:
	g++ $(CFLAGS) main.cpp
