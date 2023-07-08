include ../../Makefile.inc

.PHONY: build
build: .prebuild ${BUILD_DIR}/stage1.bin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/stage1.bin

${BUILD_DIR}/stage1.bin: stage1.asm
	nasm -f bin stage1.asm -o ${BUILD_DIR}/stage1.bin -l ${BUILD_DIR}/stage1.lst

.prebuild:
	@mkdir -p ${BUILD_DIR}