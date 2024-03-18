#
# Makefile inside trainos:build Docker image.
#

include ./Makefile.inc


.PHONY: build
build:
	mkdir -p ${BUILD_DIR}
	@make --directory ${SOURCE_DIR} build
	@make --directory ${ROOT_DIR}/disk build

.PHONY: clean
clean:
	@make --directory ${SOURCE_DIR} clean
	@make --directory ${ROOT_DIR}/disk clean
