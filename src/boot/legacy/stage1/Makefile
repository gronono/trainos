include ../../../../Makefile.inc

.PHONY: build
build: ${BUILD_DIR}/boot/legacy/stage1.bin

${BUILD_DIR}/boot/legacy/stage1.bin: stage1.asm
	nasm -f bin stage1.asm -o ${BUILD_DIR}/boot/legacy/stage1.bin -l ${BUILD_DIR}/boot/legacy/stage1.lst -w+orphan-labels
