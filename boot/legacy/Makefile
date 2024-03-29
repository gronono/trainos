include ../../Makefile.inc

.PHONY: build
build:
	@mkdir -p ${BUILD_DIR}/boot/legacy
	@make --directory ${ROOT_DIR}/boot/legacy/mbr build
	@make --directory ${ROOT_DIR}/boot/legacy/vbr build

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/boot/legacy
