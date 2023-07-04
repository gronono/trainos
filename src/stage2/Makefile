include ../../Makefile.inc

.PHONY: build
build: .prebuild stage2.bin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/stage2.bin

stage2.bin: stage2.asm
	nasm -f bin stage2.asm -o ${BUILD_DIR}/stage2.bin -l ${BUILD_DIR}/stage2.lst

.prebuild:
	@mkdir -p ${BUILD_DIR}