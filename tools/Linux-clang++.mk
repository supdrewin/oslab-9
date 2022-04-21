LDFLAGS += \
	-flto=thin \
	-rtlib=compiler-rt \
	-stdlib=libc++ \
	-fuse-ld=lld \
	-Wl,--as-needed
