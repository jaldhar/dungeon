# dungeon -- An implementation of the dungeon building algorithm from rogue
#
# By Jaldhar H. Vyas <jaldhar@braincells.com>
# Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
# "Do what thou wilt" shall be the whole of the license.

AS=as
CXX=c++
CXXFLAGS=-std=c++14 -O2 -g -Wall -Wextra -Wpedantic -Wcast-qual -Wformat=2 -Wshadow -Wno-missing-field-initializers  -Wpointer-arith -Wcast-align -Wwrite-strings -Wno-unreachable-code -Wnon-virtual-dtor -Woverloaded-virtual
LDFLAGS=-lncurses
PROGRAM=dungeon
OBJECTS=dungeon.o \
	input.o \
	render.o \
	world.o

.POSIX:

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $^

all: $(PROGRAM) strip

memcheck: $(PROGRAM)
	valgrind --leak-check=full --show-leak-kinds=all -v --log-file=valgrind.log ./$(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

strip: $(PROGRAM)
	strip --strip-all $(PROGRAM)

clean:
	-rm *.o *.gch

distclean: clean
	-rm $(PROGRAM) valgrind.log

.PHONY: memcheck strip clean distclean