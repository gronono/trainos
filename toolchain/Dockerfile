FROM debian:bookworm

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y curl           
RUN apt-get install -y bison build-essential flex libgmp3-dev libmpc-dev libmpfr-dev texinfo  

ARG TARGET_PATH=/opt/i686-elf
ENV PATH="${TARGET_PATH}/bin:$PATH"

RUN mkdir -p /usr/src/binutils-build
ARG BINUTILS_VERSION=2.40
RUN curl -SL https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz | tar -xzC /usr/src/
RUN cd /usr/src/binutils-build                      \
    && ../binutils-${BINUTILS_VERSION}/configure    \
		--disable-nls					            \
		--disable-werror                            \
        --prefix=${TARGET_PATH}                     \
        --target=i686-elf                           \
		--with-sysroot					            \
    && make -j 9                                    \
    && make install

ARG GGC_VERSION=13.1.0
RUN mkdir -p /usr/src/gcc-build
RUN curl -SL https://ftp.gnu.org/gnu/gcc/gcc-${GGC_VERSION}/gcc-${GGC_VERSION}.tar.gz | tar -xzC /usr/src/
RUN cd /usr/src/gcc-build                           \
    && ../gcc-${GGC_VERSION}/configure              \
        --disable-nls		                        \
        --enable-languages=c                        \
        --prefix=${TARGET_PATH}                     \
        --target=i686-elf                           \
        --without-headers                           \
    && make -j 9 all-gcc all-target-libgcc          \
    && make install-gcc install-target-libgcc                                 

RUN apt-get install -y bochs bochsbios bochs-x vgabios     
RUN apt-get install -y dosfstools     
RUN apt-get install -y file           
RUN apt-get install -y nasm           
RUN apt-get install -y mtools         
RUN apt-get install -y parted         
RUN apt-get install -y qemu-system-x86
RUN apt-get install -y sudo
RUN apt-get install -y xxd
RUN apt-get install -y udev

COPY trainos.sudoer /etc/sudoers.d/trainos

WORKDIR /trainos
VOLUME [ "/trainos" ]

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd --gid $GROUP_ID trainos
RUN useradd  --uid $USER_ID trainos --gid trainos --create-home
USER trainos
