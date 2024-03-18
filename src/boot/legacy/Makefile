include ../../../Makefile.inc

.PHONY: build
build:
	@mkdir -p ${BUILD_DIR}/boot/legacy
	@make --directory ${SOURCE_DIR}/boot/legacy/stage1 build
	@make --directory ${SOURCE_DIR}/boot/legacy/stage2 build

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/boot/legacy
