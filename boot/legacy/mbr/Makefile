include ../../../Makefile.inc

.PHONY: build
build: ${BUILD_DIR}/boot/legacy/mbr.bin

${BUILD_DIR}/boot/legacy/mbr.bin: mbr.asm
	${NASM_COMPILER} -f bin mbr.asm -o ${BUILD_DIR}/boot/legacy/mbr.bin -l ${BUILD_DIR}/boot/legacy/mbr.lst -w+orphan-labels
