CXXFLAGS += \
	-Wno-unknown-pragmas

LDFLAGS += \
	-rtlib=compiler-rt \
	-stdlib=libc++ \
	-flto=thin
