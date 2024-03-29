include ../Makefile.inc

# -std=c18 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
# -mgeneral-regs-only don't use FPU instructions in ISR
COMPILE_FLAGS=-std=c18 -nostdinc -ffreestanding -c -fno-ident -mgeneral-regs-only -I./include
# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS=-nodefaultlibs -nostdlib

SOURCES_C=$(shell find src -type f -name '*.c')
OBJECTS_C=$(patsubst src/%.c, $(BUILD_DIR)/libc/%.o, $(SOURCES_C))

.PHONY: build
build: ${OBJECTS_C}

$(BUILD_DIR)/libc/%.o: src/%.c
	@mkdir -p $(@D)
	${GCC_COMPILER} ${COMPILE_FLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/libc
