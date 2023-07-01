#!/bin/sh

DISK=$1
DISK_SIZE=$2

NO_COLOR='\033[0m'
COLOR_GREEN='\033[0;32m'
COLOR_RED='\033[0;31m'
log() (
    color=$1
    shift 1
    printf "%b%s%b\n" "${color}" "$*" "${NO_COLOR}"
)
info()  { log "${COLOR_GREEN}" "$@"; }
error() { log "${COLOR_RED}" "$@" >&2; }
die() { error "$*"; exit 1; }
try() { "$@" || die "Command '$*' failed!"; }

info "Create empty disk size=${DISK_SIZE}MiB"
try dd if=/dev/zero of="${DISK}" bs=1M count="${DISK_SIZE}"

info "Create msdos table"
try parted -s "${DISK}" mktable msdos

info "Copy MBR"
try dd if=build/bootloader/mbr.bin of="${DISK}" bs=1 seek=0 conv=notrunc

PARTITION_START=2048  # Start of partition (in sectors)
PARTITION_SIZE=$(( 10 * 1024 * 1024 / 512 )) # Size of the partition (in sectors)
PARTITION_END=$(( PARTITION_START + PARTITION_SIZE ))
info "Create partition start=${PARTITION_START}s end=${PARTITION_END}s size=${PARTITION_SIZE}s"
try parted -s "${DISK}" mkpart primary fat16 ${PARTITION_START}s ${PARTITION_END}s
try parted -s "${DISK}" set 1 boot on

info "Format parition"
LOOP_DEVICE=$(sudo losetup -Pf --show "${DISK}")
echo "Loop device: ${LOOP_DEVICE}"
PARTITION_DEVICE="${LOOP_DEVICE}p1"
try sudo mkfs.fat -F 16 -n TRAINOS -f 1 "${PARTITION_DEVICE}"
try sudo losetup -d /dev/loop10

FAT16_HEADERS_SIZE=62 # Length of FAT16 headers before boot code (in bytes)
STAGE1_LOCATION=$(( PARTITION_START * 512 + FAT16_HEADERS_SIZE ))
info "Copy stage1 to ${STAGE1_LOCATION}"
try dd if=build/bootloader/stage1.bin of="${DISK}" bs=1 seek=${STAGE1_LOCATION} conv=notrunc
