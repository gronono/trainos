include ./Makefile.inc

.PHONY: build
build: .prebuild toolchain
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile build

.PHONY: run
run: 
	qemu-system-i386 -drive format=raw,if=ide,index=0,media=disk,file=${BUILD_DIR}/disk.bin

.PHONY: clean
clean:
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile clean

.PHONY: toolchain
toolchain:
	make --directory toolchain build

.PHONY: .prebuild
.prebuild:
	mkdir -p ${BUILD_DIR}