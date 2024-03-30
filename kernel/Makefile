include ../Makefile.inc

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
