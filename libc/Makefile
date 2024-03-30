include ../Makefile.inc

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
