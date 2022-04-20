override CXX = clang++

CXXFLAGS += \
	-fuse-ld=lld \
	-flto=thin \
	-std=c++14 \
	-pedantic \
	-Wall \
	-Wextra \
	-Weffc++ \
	-O2

src_1: src_1.cc

clean:
	$(RM) src_1

.PHONY: clean
