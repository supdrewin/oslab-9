override CXX = clang++

CXXFLAGS += \
	-fuse-ld=lld \
	-flto=thin \
	-std=c++20 \
	-pedantic \
	-Wall \
	-Wextra \
	-Weffc++ \
	-O2

all: src_1 src_2

src_1: src_1.cc
src_2: src_2.cc

clean:
	$(RM) src_1 src_2

.PHONY: clean
