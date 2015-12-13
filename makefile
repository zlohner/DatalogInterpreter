CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := DatalogInterpreter.o
SRCEXT := cpp
CFLAGS := -Wall -g -std=c++11

SOURCES := $(shell find $(SRCDIR) -name "*.$(SRCEXT)")

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDDIR)/$(TARGET)
	
clean:
	rm -r $(BUILDDIR)/$(TARGET) $(BUILDDIR)/$(TARGET).dSYM
