include ./Makefile.inc

MODULES := "bootloader"
DISK := ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: build
build: .prebuild .build_modules disk

.PHONY: clean
clean:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module clean ; \
	done

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}

.PHONY: .build_modules
.build_modules:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module build ; \
	done

.PHONY: disk
disk:
	# Create empty disk
	dd if=/dev/zero of=${DISK} bs=1M count=${DISK_SIZE}
	parted -s ${DISK} mktable msdos

	# Copy MBR
	dd if=${BUILD_DIR}/bootloader/mbr1.bin of=${DISK} bs=1 seek=0 conv=notrunc
	dd if=${BUILD_DIR}/bootloader/mbr2.bin of=${DISK} bs=1 seek=224 conv=notrunc

	# Format disk into ${DISK_FORMAT}
	parted -s ${DISK} mkpart primary fat32 1MiB 11MiB
	parted -s ${DISK} set 1 boot on
