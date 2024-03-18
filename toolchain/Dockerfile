FROM debian:bookworm

# Target OS architecture
ARG TARGET=i386-elf
# Trainos user id and group id
# Same as your current user for Docker volume permissions
# See Makefile
ARG USER_ID=1000
ARG GROUP_ID=1000

# Binutils version
# See https://ftp.gnu.org/gnu/binutils/
ARG BINUTILS_VERSION=2.42
# GGC version
# See https://bigsearcher.com/mirrors/gcc/releases/?C=M;O=D
ARG GGC_VERSION=13.2.0
# Path of cross compiler tools
ARG TARGET_PATH=/opt/trainos/${TARGET}

# Adding cross compiler tools into $PATH
ENV PATH="${TARGET_PATH}/bin:$PATH"

# Update system
RUN apt-get update &&   \
    apt-get upgrade -y

# Install building tools
RUN apt-get install -y  \
        bison           \
        build-essential \
        curl            \
        flex            \
        libgmp3-dev     \
        libmpc-dev      \
        libmpfr-dev     \
        texinfo

# Compile and install Binutils
RUN mkdir -p /usr/src/binutils-build
RUN curl -SL https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz | tar -xzC /usr/src/
RUN cd /usr/src/binutils-build                      \
    && ../binutils-${BINUTILS_VERSION}/configure    \
		--disable-nls					            \
		--disable-werror                            \
        --prefix=${TARGET_PATH}                     \
        --target=${TARGET}                          \
		--with-sysroot					            \
    && make -j 9                                    \
    && make install

# Compile and install GCC
RUN mkdir -p /usr/src/gcc-build
RUN curl -SL https://ftp.gnu.org/gnu/gcc/gcc-${GGC_VERSION}/gcc-${GGC_VERSION}.tar.gz | tar -xzC /usr/src/
RUN cd /usr/src/gcc-build                           \
    && ../gcc-${GGC_VERSION}/configure              \
        --disable-nls		                        \
        --enable-languages=c                        \
        --disable-bootstrap                         \
        --without-headers                           \
        --prefix=${TARGET_PATH}                     \
        --target=${TARGET}                          \
    && make -j 9 all-gcc all-target-libgcc          \
    && make install-gcc install-target-libgcc

# Install additionnal tools
RUN apt-get install -y bochs bochsbios bochs-x vgabios
RUN apt-get install -y dosfstools
RUN apt-get install -y file
RUN apt-get install -y gdb
RUN apt-get install -y nasm
RUN apt-get install -y mtools
RUN apt-get install -y parted
RUN apt-get install -y qemu-system-x86
RUN apt-get install -y sudo
RUN apt-get install -y xxd
RUN apt-get install -y udev
RUN apt-get install -y vim
RUN apt-get install -y git

WORKDIR /trainos
VOLUME [ "/trainos" ]

# Add trainos user
RUN groupadd --gid $GROUP_ID trainos
RUN useradd  --uid $USER_ID trainos --gid trainos --create-home
# Sudo rights for this user
COPY trainos.sudoer /etc/sudoers.d/trainos
# Switch to trainos user
USER trainos
