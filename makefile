CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := DatalogInterpreter.o
SRCEXT := cpp
CFLAGS := -Wall -Werror -g -std=c++17

SOURCES := $(shell find $(SRCDIR) -name "*.$(SRCEXT)")

all:
	if [ ! -d "build" ]; then mkdir build; fi
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDDIR)/$(TARGET)

clean:
	rm -r ./$(BUILDDIR)/*
