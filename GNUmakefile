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

all: impl1.exe impl2.exe

impl1.exe.: impl1.cc cpprt.cc
impl2.exe: impl2.cc cpprt.cc

clean:
	$(RM) *.exe

.PHONY: clean

%.exe: %.cc cpprt.cc
	$(LINK.cc) -o $@ $^ $(LDLIBS)
