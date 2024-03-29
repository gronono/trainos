#
# Makefile inside trainos:build Docker image.
#

include ./Makefile.inc


.PHONY: build
build:
	mkdir -p ${BUILD_DIR}
	@make --directory ${ROOT_DIR}/boot build
	@make --directory ${ROOT_DIR}/libc build
	@make --directory ${ROOT_DIR}/kernel build
	@make --directory ${ROOT_DIR}/disk build

.PHONY: clean
clean:
	@make --directory ${ROOT_DIR}/disk clean
	@make --directory ${ROOT_DIR}/libc clean
	@make --directory ${ROOT_DIR}/kernel clean
	@make --directory ${ROOT_DIR}/boot clean
