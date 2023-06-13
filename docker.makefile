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

	# Copy MBR
	dd if=${BUILD_DIR}/bootloader/mbr.bin of=${DISK} bs=1 seek=0 conv=notrunc

	# Format disk into ${DISK_FORMAT}
ifeq ($(DISK_FORMAT),FAT12)
#	mkfs.fat -F 12 -n $(shell echo ${OS_NAME} | tr a-z A-Z) ${DISK}
else ifeq ($(DISK_FORMAT),FAT16)
#	mkfs.fat -F 16 -n $(shell echo ${OS_NAME} | tr a-z A-Z) ${DISK}
else ifeq ($(DISK_FORMAT),FAT32)
#	mkfs.fat -F 32 -n $(shell echo ${OS_NAME} | tr a-z A-Z) ${DISK}
else 
	$(error unsupported disk format: ${DISK_FORMAT} )
endif
