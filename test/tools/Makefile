name-to-bin = $(patsubst %,$(TARGET_BIN_DIR)/%$(TARGET_EXEEXT),$(1))

VPATH = ~/src/XCSoar/xcsoar/
topdir = ~/src/XCSoar/xcsoar
SRC = $(topdir)/src

INCLUDES = -I $(SRC)
CXXFLAGS = -ggdb $(INCLUDES) -std=gnu++11
LDLIBS = -lstdc++

HEADERS = eqn-expansion.hpp
SOURCES = APAt.cpp OtPO eqn-expansion.cpp 3x3.cpp 3x3x3t.cpp

TOOL_PROGRAMS = APAt OtPO # 3x3 3x3x3t


TOOLS_LDADD = $(MATH_LIBS)

build-tools: dependencies.make $(TOOL_PROGRAMS)

APAt: eqn-expansion.o APAt.o

OtPO: eqn-expansion.o OtPO.o

3x3: eqn-expansion.o 3x3.o

3x3x3t: eqn-expansion.o 3x3x3t.o

dependencies: dependencies.make

dependencies.make: $(SOURCES) $(HEADERS)
	gcc -M $(INCLUDES) $^ > $@ || rm $@

clean:
	rm -f *.o $(TOOL_PROGRAMS)

include ./dependencies.make
