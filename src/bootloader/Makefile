include ../../Makefile.inc

BOOTLOADER_SOURCE_DIR := ${SOURCE_DIR}/bootloader
BOOTLOADER_BUILD_DIR := ${BUILD_DIR}/bootloader

.PHONY: build
build: .prebuild ${BOOTLOADER_BUILD_DIR}/stage1.bin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}

${BOOTLOADER_BUILD_DIR}/stage1.bin: ${BOOTLOADER_SOURCE_DIR}/stage1.asm
	nasm -f bin ${BOOTLOADER_SOURCE_DIR}/stage1.asm -o ${BOOTLOADER_BUILD_DIR}/stage1.bin

.PHONY: .prebuild
.prebuild:
	mkdir -p ${BOOTLOADER_BUILD_DIR}
