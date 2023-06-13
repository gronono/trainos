include ../../Makefile.inc

BOOTLOADER_SOURCE_DIR := ${SOURCE_DIR}/bootloader
BOOTLOADER_BUILD_DIR  := ${BUILD_DIR}/bootloader

FILES        := mbr_part1 mbr_part2
SOURCE_FILES := $(addprefix ${BOOTLOADER_SOURCE_DIR}/, $(addsuffix .asm, ${FILES}))
BUILD_FILES  := $(patsubst ${BOOTLOADER_SOURCE_DIR}/%.asm, ${BOOTLOADER_BUILD_DIR}/%.bin, ${SOURCE_FILES})

.PHONY: build
build: .prebuild ${BOOTLOADER_BUILD_DIR}/mbr.bin

.PHONY: clean
clean:
	rm -rf ${BOOTLOADER_BUILD_DIR}

${BOOTLOADER_BUILD_DIR}/mbr.bin : ${BOOTLOADER_SOURCE_DIR}/mbr.asm
	nasm -f bin $(patsubst ${BOOTLOADER_BUILD_DIR}/%.bin, ${BOOTLOADER_SOURCE_DIR}/%.asm, $@) -o $@

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BOOTLOADER_BUILD_DIR}
