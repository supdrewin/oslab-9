LDFLAGS += \
	-flto=thin \
	-rtlib=compiler-rt \
	-stdlib=libc++ \
	-unwindlib=libunwind \
	-fuse-ld=lld \
	-Wl,--as-needed
