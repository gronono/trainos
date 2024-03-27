#
# Customizable configuration
#

# Architecture target (i386-elf or x86_64-elf)
OS_TARGET           := i386-trainos-elf
# OS_TARGET         := x86_64-elf
# EXE format (elf or elf64)
EXE_FORMAT          := elf
# EXE_FORMAT        := elf64

# Define how big is generated disk (in MB)
DISK_SIZE           := 32

# Common configuration
ROOT_DIR            := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR           := $(abspath ${ROOT_DIR}/build)
SOURCE_DIR          := $(abspath ${ROOT_DIR}/src)
OS_NAME             := trainos
BUILD_IMAGE_NAME    := ${OS_NAME}-toolchain:${OS_TARGET}
GCC_COMPILER        := /opt/trainos/${OS_TARGET}/bin/${OS_TARGET}-gcc
NASM_COMPILER       := /usr/bin/nasm

BOCHS_EXE           := /usr/bin/bochs
QEMU_EXE            := /usr/bin/qemu-system-i386
GDB_EXE             := /usr/bin/gdb
