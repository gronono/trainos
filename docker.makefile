include ./Makefile.inc

MODULES := "bootloader"

.PHONY: build
build: .prebuild .build_modules .disk

.PHONY: clean
clean:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module clean ; \
	done
	make --directory toolchain clean
	rm -rf ${BUILD_DIR}

.PHONY: .prebuild
.prebuild:
	mkdir -p ${BUILD_DIR}

.PHONY: .build_modules
.build_modules:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module build ; \
	done

.PHONY: .disk
.disk: ${BUILD_DIR}/disk.bin ${BUILD_DIR}/.fat32 ${BUILD_DIR}/.stage1

${BUILD_DIR}/disk.bin:
	dd if=/dev/zero of=${BUILD_DIR}/disk.bin bs=1M count=${DISK_SIZE}

${BUILD_DIR}/.fat32:
	mkfs.fat -F 32 -n $(shell echo ${OS_NAME} | tr a-z A-Z) ${BUILD_DIR}/disk.bin
	touch ${BUILD_DIR}/.fat32

${BUILD_DIR}/.stage1:
	dd if=${BUILD_DIR}/bootloader/stage1.bin of=${BUILD_DIR}/disk.bin bs=1 seek=90 conv=notrunc
	#touch ${BUILD_DIR}/.stage1