#
# Local Makefile
# Use docker trainos:build image.
#

include ./Makefile.inc
USER_ID := $(shell id -u)
DOCKER_GUI_ARGS := -v /tmp/.X11-unix:/tmp/.X11-unix -v ${HOME}/.Xauthority:/home/trainos/.Xauthority -v /run/user/${USER_ID}/at-spi/bus_0:/run/user/${USER_ID}/at-spi/bus_0 -e DISPLAY=${DISPLAY}

.PHONY: build
build: clean
	docker run --rm -v ${ROOT_DIR}:/trainos --privileged=true -v /dev:/dev ${BUILD_IMAGE_NAME} make -f docker.makefile build

.PHONY: run
run: build
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} ${QEMU_EXE} -m 1G -drive format=raw,if=ide,index=0,media=disk,file=/trainos/build/${OS_NAME}.bin -serial stdio

.PHONY: bochs
bochs: build
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} ${BOCHS_EXE} -qf bochs.config

.PHONY: gdb
gdb: build
	docker run --rm -v ${ROOT_DIR}:/trainos ${DOCKER_GUI_ARGS} ${BUILD_IMAGE_NAME} ${GDB_EXE} -x qemu.gdb

.PHONY: clean
clean:
	docker run --rm -v ${ROOT_DIR}:/trainos ${BUILD_IMAGE_NAME} make -f docker.makefile clean

.PHONY: toolchain
toolchain:
	make --directory toolchain build
