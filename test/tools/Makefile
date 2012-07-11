name-to-bin = $(patsubst %,$(TARGET_BIN_DIR)/%$(TARGET_EXEEXT),$(1))

VPATH = ~/src/XCSoar/xcsoar/
topdir = ~/src/XCSoar/xcsoar
SRC = $(topdir)/src

INCLUDES = -I $(SRC)
CXXFLAGS = -g $(INCLUDES)
LDFLAGS = -lstdc++

SOURCES = APAt.cpp eqn-expansion.cpp 3x3.cpp 3x3x3t.cpp

TOOL_PROGRAMS = APAt 3x3 3x3x3t

TOOLS_LDADD = $(MATH_LIBS)

build-tools: dependencies.make $(TOOL_PROGRAMS)

APAt: eqn-expansion.o APAt.o

3x3: 3x3.o eqn-expansion.o

3x3x3t: 3x3x3t.o eqn-expansion.o

dependencies: dependencies.make

dependencies.make: $(SOURCES)
	gcc -M $(INCLUDES) $^ > $@ || rm $@

clean:
	rm *.o $(TOOL_PROGRAMS)

include ./dependencies.make
