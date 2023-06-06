ROOT_DIR            := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR           := $(abspath ${ROOT_DIR}/build)
SOURCE_DIR          := $(abspath ${ROOT_DIR}/src)
BUILD_IMAGE_NAME    := trainos
DISK_SIZE           := 128