#
# Makefile inside trainos:build Docker image.
#

include ./Makefile.inc

DISK := ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: build
build:
	mkdir -p ${BUILD_DIR}
	@make --directory ${SOURCE_DIR} build

.PHONY: clean
clean:
	@make --directory ${SOURCE_DIR} clean
