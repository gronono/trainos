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
OS_NAME             := trainos
BUILD_IMAGE_NAME    := ${OS_NAME}-toolchain:${OS_TARGET}
GCC_COMPILER        := /opt/trainos/${OS_TARGET}/bin/${OS_TARGET}-gcc

# -Wall display all warnings
# -std=c18 use C version defined in ISO/IEC 9899:2018
# -nostdinc do not seach for std directories for headers files
# -ffreestanding freestanding environnement
# -c do not link
# -mgeneral-regs-only don't use FPU instructions in ISR
COMPILE_FLAGS       := -Wall -std=c18 -nostdinc -ffreestanding -c -fno-ident -mgeneral-regs-only -I./include -I../libc/include

# -nodefaultlibs do not use standard system librairies
# -nostdlib do not use standard system startup file (cri0.o)
LINK_FLAGS          := -nodefaultlibs -nostdlib
NASM_COMPILER       := /usr/bin/nasm

BOCHS_EXE           := /usr/bin/bochs
QEMU_EXE            := /usr/bin/qemu-system-i386
GDB_EXE             := /usr/bin/gdb
