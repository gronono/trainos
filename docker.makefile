include ./Makefile.inc

MODULES := "bootloader"

.PHONY: build
build: .prebuild .build_modules ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: clean
clean:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module clean ; \
	done
	make --directory toolchain clean
	rm -rf ${BUILD_DIR}

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}

.PHONY: .build_modules
.build_modules:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module build ; \
	done

${BUILD_DIR}/${OS_NAME}.bin:
	# Create empty disk
	dd if=/dev/zero of=$@ bs=1M count=${DISK_SIZE}
	# Format disk into ${DISK_FORMAT}
ifeq ($(DISK_FORMAT),FAT12)
	mkfs.fat -F 12 -n $(shell echo ${OS_NAME} | tr a-z A-Z) $@
	# copy stage1 in boot code
	dd if=${BUILD_DIR}/bootloader/stage1.bin of=$@ bs=1 seek=62 conv=notrunc
else ifeq ($(DISK_FORMAT),FAT16)
	mkfs.fat -F 16 -n $(shell echo ${OS_NAME} | tr a-z A-Z) $@
	# copy stage1 in boot code
	dd if=${BUILD_DIR}/bootloader/stage1.bin of=$@ bs=1 seek=62 conv=notrunc
else ifeq ($(DISK_FORMAT),FAT32)
	mkfs.fat -F 32 -n $(shell echo ${OS_NAME} | tr a-z A-Z) $@
	# copy stage1 in boot code
	dd if=${BUILD_DIR}/bootloader/stage1.bin of=$@ bs=1 seek=90 conv=notrunc
	# and this backup
	dd if=${BUILD_DIR}/bootloader/stage1.bin of=$@ bs=1 seek=3162 conv=notrunc
else 
	$(error unsupported disk format: ${DISK_FORMAT} )
endif
	# Copy stage2 into disk
	mcopy -i $@ $(BUILD_DIR)/bootloader/stage2.bin "::stage2.bin"
