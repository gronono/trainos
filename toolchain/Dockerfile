FROM debian:bullseye


RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y dosfstools
RUN apt-get install -y file
RUN apt-get install -y nasm
RUN apt-get install -y make

RUN mkdir -p /trainos
WORKDIR /trainos
VOLUME [ "/trainos" ]

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd -r -g $GROUP_ID trainos
RUN useradd  -u $USER_ID trainos -g trainos
USER trainos

