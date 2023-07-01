include ../../Makefile.inc

.PHONY: build
build: .prebuild mbr.bin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/mbr.bin

mbr.bin: mbr.asm
	nasm -f bin mbr.asm -o ${BUILD_DIR}/mbr.bin 

.prebuild:
	@mkdir -p ${BUILD_DIR}