#
# Local Makefile
# Use docker trainos:build image.
#

include ./Makefile.inc
DOCKER_GUI_ARGS := -v /tmp/.X11-unix:/tmp/.X11-unix -v ${HOME}/.Xauthority:/home/trainos/.Xauthority -e DISPLAY=${DISPLAY}

.PHONY: build
build: .prebuild
	docker run --rm -v ${ROOT_DIR}:/trainos --privileged=true -v /dev:/dev ${BUILD_IMAGE_NAME} make -f docker.makefile build

.PHONY: run
run:
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} qemu-system-i386 -m 1G -drive format=raw,if=ide,index=0,media=disk,file=/trainos/build/${OS_NAME}.bin

.PHONY: bochs
bochs:
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} bochs -qf bochs.config

.PHONY: gdb
gdb:
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} gdb -x qemu.gdb

.PHONY: clean
clean:
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile clean

.PHONY: toolchain
toolchain:
	make --directory toolchain build

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}
