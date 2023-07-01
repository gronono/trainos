include ./Makefile.inc

DISK := ${BUILD_DIR}/${OS_NAME}.bin

.PHONY: build
build: .prebuild .build_modules disk

.PHONY: clean
clean:
	@rm -rf ${BUILD_DIR}

.PHONY: .prebuild
.prebuild:
	@mkdir -p ${BUILD_DIR}

.PHONY: .build_modules
.build_modules:
	@make --directory ${SOURCE_DIR}/mbr build
	@make --directory ${SOURCE_DIR}/stage1 build
	@make --directory ${SOURCE_DIR}/stage2 build

.PHONY: disk
disk:
	./scripts/create_disk.sh ${DISK} ${DISK_SIZE}
