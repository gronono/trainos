include ../../Makefile.inc

BOOTLOADER_SOURCE_DIR := ${SOURCE_DIR}/bootloader
BOOTLOADER_BUILD_DIR := ${BUILD_DIR}/bootloader

STAGES := stage1 stage2
STAGES_SOURCE_FILES := $(addprefix ${BOOTLOADER_SOURCE_DIR}/, $(addsuffix .asm, ${STAGES}))
STAGES_BUILD_FILES := $(patsubst ${BOOTLOADER_SOURCE_DIR}/%.asm, ${BOOTLOADER_BUILD_DIR}/%.bin, ${STAGES_SOURCE_FILES})

.PHONY: build
build: .prebuild ${STAGES_BUILD_FILES}

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}

${STAGES_BUILD_FILES}: ${STAGES_SOURCE_FILES}
	nasm -f bin $(patsubst ${BOOTLOADER_BUILD_DIR}/%.bin, ${BOOTLOADER_SOURCE_DIR}/%.asm, $@) -o $@

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BOOTLOADER_BUILD_DIR}
