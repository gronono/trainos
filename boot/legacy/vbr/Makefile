include ../../../Makefile.inc

# -std=c18 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
COMPILE_FLAGS=-std=c18 -nostdinc -ffreestanding -c
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_C=$(wildcard *.c)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/boot/legacy/%.o, $(SOURCES_C))

.PHONY: build
build: ${BUILD_DIR}/boot/legacy/vbr.bin

${BUILD_DIR}/boot/legacy/vbr.bin: vbr.asm
	${NASM_COMPILER} -f bin vbr.asm -o ${BUILD_DIR}/boot/legacy/vbr.bin -l ${BUILD_DIR}/boot/legacy/vbr.lst -w+orphan-labels -w+number-overflow
