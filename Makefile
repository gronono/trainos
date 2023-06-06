include ./Makefile.inc

.PHONY: build
build: .prebuild .toolchain
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile build

.PHONY: clean
clean:
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile clean

.PHONY: .toolchain
.toolchain:
	make --directory toolchain build

.PHONY: .prebuild
.prebuild:
	mkdir -p ${BUILD_DIR}