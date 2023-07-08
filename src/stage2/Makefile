include ../../Makefile.inc

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
	@echo "--> Created  stage2.bin"
	i686-elf-gcc -T linker.ld -nostdlib -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^ -lgcc

$(BUILD_DIR)/stage2/c/%.o: %.c
	@mkdir -p $(@D)
	i686-elf-gcc -std=c99 -g -ffreestanding -nostdlib -c -o $@ $<
	@echo "--> Compiled: " $<

$(BUILD_DIR)/stage2/asm/%.o: %.asm
	@mkdir -p $(@D)
	nasm -f elf -o $@ $<
	@echo "--> Compiled: " $<

.prebuild:
	@mkdir -p ${BUILD_DIR}