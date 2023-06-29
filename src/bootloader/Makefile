include ../../Makefile.inc

BOOTLOADER_SOURCE_DIR := ${SOURCE_DIR}/bootloader
BOOTLOADER_BUILD_DIR  := ${BUILD_DIR}/bootloader

FILES        := mbr stage1
SOURCE_FILES := $(addprefix ${BOOTLOADER_SOURCE_DIR}/, $(addsuffix .asm, ${FILES}))
BUILD_FILES  := $(patsubst ${BOOTLOADER_SOURCE_DIR}/%.asm, ${BOOTLOADER_BUILD_DIR}/%.bin, ${SOURCE_FILES})

.PHONY: build
build: .prebuild ${BUILD_FILES}

.PHONY: clean
clean:
	rm -rf ${BOOTLOADER_BUILD_DIR}

${BUILD_FILES} : ${SOURCE_FILES}
	nasm -f bin $(patsubst ${BOOTLOADER_BUILD_DIR}/%.bin, ${BOOTLOADER_SOURCE_DIR}/%.asm, $@) -o $@

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BOOTLOADER_BUILD_DIR}
