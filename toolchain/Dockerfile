FROM debian:bullseye

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y curl
RUN apt-get install -y file
RUN apt-get install -y nasm
RUN apt-get install -y make
RUN apt-get install -y mtools
RUN apt-get install -y parted
RUN apt-get install -y qemu-system-x86
RUN apt-get install -y udev
RUN apt-get install -y bochs bochsbios bochs-x vgabios
RUN apt-get install -y sudo
RUN apt-get install -y dosfstools
COPY trainos.sudoer /etc/sudoers.d/trainos

RUN mkdir -p /trainos
WORKDIR /trainos
VOLUME [ "/trainos" ]

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd --gid $GROUP_ID trainos
RUN useradd  --uid $USER_ID trainos --gid trainos --create-home
USER trainos

