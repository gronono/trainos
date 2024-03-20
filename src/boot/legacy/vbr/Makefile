include ../../../../Makefile.inc

.PHONY: build
build: ${BUILD_DIR}/boot/legacy/vbr.bin

${BUILD_DIR}/boot/legacy/vbr.bin: vbr.asm
	${NASM_COMPILER} -f bin vbr.asm -o ${BUILD_DIR}/boot/legacy/vbr.bin -l ${BUILD_DIR}/boot/legacy/vbr.lst -w+orphan-labels
