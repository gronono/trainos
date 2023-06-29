ROOT_DIR            := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR           := $(abspath ${ROOT_DIR}/build)
SOURCE_DIR          := $(abspath ${ROOT_DIR}/src)
OS_NAME             := trainos
BUILD_IMAGE_NAME    := ${OS_NAME}
DISK_SIZE           := 64