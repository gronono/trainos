include ../Makefile.inc

# -std=c18 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
# -mgeneral-regs-only don't use FPU instructions in ISR
COMPILE_FLAGS=-std=c18 -nostdinc -ffreestanding -c -fno-ident -mgeneral-regs-only -I./include -I../libc/include
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_ASM=$(shell find src -type f -name '*.asm')
OBJECTS_ASM=$(patsubst src/%.asm, $(BUILD_DIR)/kernel/%.o, $(SOURCES_ASM))
SOURCES_C=$(shell find src -type f -name '*.c')
OBJECTS_C=$(patsubst src/%.c, $(BUILD_DIR)/kernel/%.o, $(SOURCES_C))
LIBC_OBJECTS=$(shell find ${BUILD_DIR}/libc -type f -name "*.o")

.PHONY: build
build: ${BUILD_DIR}/kernel/kernel.bin

${BUILD_DIR}/kernel/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	${GCC_COMPILER} -T linker.ld ${LINK_FLAGS} -Wl,-Map=$(BUILD_DIR)/kernel/kernel.map -o $@ ${LIBC_OBJECTS} $^ -lgcc

$(BUILD_DIR)/kernel/%.o: src/%.c
	@mkdir -p $(@D)
	${GCC_COMPILER} ${COMPILE_FLAGS} -o $@ $<

$(BUILD_DIR)/kernel/%.o: src/%.asm
	@mkdir -p $(@D)
	${NASM_COMPILER} -f elf32 -o $@ $<

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/kernel
