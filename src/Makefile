include ../Makefile.inc

.PHONY: build
build:
	@make --directory ${SOURCE_DIR}/boot build
	@make --directory ${SOURCE_DIR}/kernel build

.PHONY: clean
clean:
	@make --directory ${SOURCE_DIR}/boot clean
	@make --directory ${SOURCE_DIR}/kernel clean
	rm -rf ${BUILD_DIR}
