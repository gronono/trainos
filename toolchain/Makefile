include ../Makefile.inc

# Docker image contains an user named 'trainos' with this ids
# Setting the ids from your host user guaranteed that you have access to the generated files.
USER_ID := $(shell id -u)
GROUP_ID := $(shell id -g)

.PHONY: build
build:
	docker build --build-arg USER_ID=${USER_ID} --build-arg GROUP_ID=${GROUP_ID} -t ${BUILD_IMAGE_NAME} .
