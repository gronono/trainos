include ../Makefile.inc

DISK := ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: build
build:
	./create_disk.sh ${DISK} ${DISK_SIZE}

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}/trainos.bin
