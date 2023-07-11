include ../../Makefile.inc

# -std=c17 use C version defined in ISO/IEC 9899:2018
# -g add debug symbols
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
COMPILE_FLAGS=-std=c17 -nostdinc -ffreestanding -c
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/stage2/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.o, $(SOURCES_ASM))

.PHONY: build
build: .prebuild ${BUILD_DIR}/stage2.bin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/stage2.bin

${BUILD_DIR}/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	i686-elf-gcc -T linker.ld ${LINK_FLAGS} -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^

$(BUILD_DIR)/stage2/c/%.o: %.c
	@mkdir -p $(@D)
	i686-elf-gcc ${COMPILE_FLAGS} -o $@ $<

$(BUILD_DIR)/stage2/asm/%.o: %.asm
	@mkdir -p $(@D)
	nasm -f elf -o $@ $< -w+orphan-labels

.prebuild:
	@mkdir -p ${BUILD_DIR}