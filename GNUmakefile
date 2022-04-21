include tools/$(shell uname)-$(CXX).mk

CXXFLAGS += \
	-std=c++20 \
	-pedantic \
	-Werror \
	-Wall \
	-Wextra \
	-Weffc++ \
	-Wsign-conversion \
	-Wno-unknown-pragmas \
	-O2

CXXFLAGS += $(shell ncurses6-config --cflags)
LDLIBS += $(shell ncurses6-config --libs)

all: src_1.exe src_2.exe

src_1.exe.: src_1.cc
src_2.exe: src_2.cc

clean:
	$(RM) *.exe

.PHONY: clean

%.exe: %.cc
	$(LINK.cc) -o $@ $^ $(LDLIBS)
