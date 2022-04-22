LDFLAGS += \
	-flto=thin \
	-stdlib=libc++ \
	-fuse-ld=lld \
	-Wl,--as-needed
