include ./Makefile.inc
DOCKER_GUI_ARGS := -v /tmp/.X11-unix:/tmp/.X11-unix -v ${HOME}/.Xauthority:/home/trainos/.Xauthority -e DISPLAY=${DISPLAY}


.PHONY: build
build: .prebuild toolchain
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile build

.PHONY: run
run: build
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} qemu-system-i386 -drive format=raw,if=ide,index=0,media=disk,file=/trainos/build/${OS_NAME}.bin

.PHONY: clean
clean:
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile clean

.PHONY: toolchain
toolchain:
	make --directory toolchain build

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}