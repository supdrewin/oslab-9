LDFLAGS += \
	-flto=$(shell nproc) \
	-pthread

