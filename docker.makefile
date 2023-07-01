include ./Makefile.inc

MODULES := "bootloader"
DISK := ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: build
build: .prebuild .build_modules disk

.PHONY: clean
clean:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module clean ; \
	done
	@rm -rf ${DISK}

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}

.PHONY: .build_modules
.build_modules:
	@for module in ${MODULES}; do \
		make --directory ${SOURCE_DIR}/$$module build ; \
	done

.PHONY: disk
disk:
	./scripts/create_disk.sh ${DISK} ${DISK_SIZE}
