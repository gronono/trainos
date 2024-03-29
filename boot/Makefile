include ../Makefile.inc

.PHONY: build
build:
	@make --directory ${ROOT_DIR}/boot/legacy build
	@make --directory ${ROOT_DIR}/boot/uefi build

.PHONY: clean
clean:
	@make --directory ${ROOT_DIR}/boot/legacy clean
	@make --directory ${ROOT_DIR}/boot/uefi clean
	rm -rf ${BUILD_DIR}/boot/
