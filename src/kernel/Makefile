include ../../Makefile.inc

# -std=c17 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
COMPILE_FLAGS=-std=c18 -nostdinc -ffreestanding -c
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_C=$(shell find . -type f -name '*.c')
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/%.o, $(SOURCES_C))

.PHONY: build
build: ${BUILD_DIR}/kernel/kernel.bin

${BUILD_DIR}/kernel/kernel.bin: $(OBJECTS_C)
	${GCC_COMPILER} -T linker.ld ${LINK_FLAGS} -Wl,-Map=$(BUILD_DIR)/kernel/kernel.map -o $@ $^

$(BUILD_DIR)/kernel/%.o: %.c
	@mkdir -p $(@D)
	${GCC_COMPILER} ${COMPILE_FLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/kernel
