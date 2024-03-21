include ../../../../Makefile.inc

# -std=c17 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
COMPILE_FLAGS=-std=c17 -nostdinc -ffreestanding -c
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_C=$(wildcard *.c)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/boot/legacy/%.o, $(SOURCES_C))

.PHONY: build
build: ${BUILD_DIR}/boot/legacy/vbr.bin

${BUILD_DIR}/boot/legacy/vbr.bin: $(OBJECTS_C) $(BUILD_DIR)/boot/legacy/boot.o
	${GCC_COMPILER} -T linker.ld ${LINK_FLAGS} -Wl,-Map=$(BUILD_DIR)/boot/legacy/entry.map -o $@ $^

$(BUILD_DIR)/boot/legacy/boot.o: boot.asm
	${NASM_COMPILER} -f ${EXE_FORMAT} boot.asm -o ${BUILD_DIR}/boot/legacy/boot.o -l ${BUILD_DIR}/boot/legacy/boot.lst -w+orphan-labels

$(BUILD_DIR)/boot/legacy/%.o: %.c
	${GCC_COMPILER} ${COMPILE_FLAGS} -o $@ $<
