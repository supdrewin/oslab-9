#
# GNUmakefile
#

CXXFLAGS += \
	-std=c++2a \
	-pedantic \
	-Werror \
	-Wall \
	-Wextra \
	-Weffc++ \
	-Wsign-conversion \
	-pipe \
	-O2

include tools/$(shell uname)-$(CXX).mk
include tools/ncurses6.mk

CXXFLAGS += $(shell $(NCURSES_CONFIG) --cflags)
LDLIBS += $(shell $(NCURSES_CONFIG) --libs)

all: impl1.exe impl2.exe

impl1.exe.: impl1.cc cxxrt.cc
impl2.exe: impl2.cc cxxrt.cc

clean:
	$(RM) *.exe

.PHONY: clean

%.exe: %.cc cxxrt.cc
	$(LINK.cc) -o $@ $^ $(LDLIBS)
