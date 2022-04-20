override CXX = clang++

ifneq ($(shell uname),Darwin)
LDFLAGS += \
	-fuse-ld=lld \
	-Wl,--as-needed
endif

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

LDFLAGS += \
	-rtlib=compiler-rt \
	-stdlib=libc++ \
	-flto=thin \
	-pthread

CXXFLAGS += $(shell pkg-config --cflags ncurses)
LDFLAGS += $(shell pkg-config --libs ncurses)

all: src_1 src_2

src_1: src_1.cc
src_2: src_2.cc

clean:
	$(RM) src_1 src_2

.PHONY: clean
