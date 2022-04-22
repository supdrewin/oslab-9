CXXFLAGS += \
	-Wno-unknown-pragmas

LDFLAGS += \
	-flto=$(shell nproc) \
	-pthread
