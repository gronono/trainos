include ../../Makefile.inc

.PHONY: build
build:
	@make --directory ${SOURCE_DIR}/boot/legacy build
	@make --directory ${SOURCE_DIR}/boot/uefi build

.PHONY: clean
clean:
	@make --directory ${SOURCE_DIR}/boot/legacy clean
	@make --directory ${SOURCE_DIR}/boot/uefi clean
	rm -rf ${BUILD_DIR}/boot/
