#!/bin/bash

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

SECTOR_SIZE=512

info "Create empty disk size=${DISK_SIZE}MiB"
try dd if=/dev/zero of="${DISK}" bs=1M count="${DISK_SIZE}"

info "Create msdos table"
try parted -s "${DISK}" mktable msdos

info "Copy MBR"
try dd if=/trainos/build/boot/legacy/mbr.bin of="${DISK}" bs=1 seek=0 conv=notrunc

DISK_SECTORS=$(( DISK_SIZE * 1024 * 1024 / SECTOR_SIZE ))
PARTITION_START=2048  # Start of partition (in sectors)
PARTITION_LENGTH=$(( DISK_SECTORS - PARTITION_START - 1 ))
PARTITION_END=$(( PARTITION_START + PARTITION_LENGTH ))
info "Create partition start=${PARTITION_START}s end=${PARTITION_END}s length=${PARTITION_LENGTH}s"
try parted -s "${DISK}" mkpart primary fat16 ${PARTITION_START}s ${PARTITION_END}s
try parted -s "${DISK}" set 1 boot on

info "Format partition"
LOOP_DEVICE=$(sudo losetup -Pf --show "${DISK}")
echo "Loop device: ${LOOP_DEVICE}"
PARTITION_DEVICE="${LOOP_DEVICE}p1"
try sudo mkfs.fat -F 16 -n TRAINOS -f 1 "${PARTITION_DEVICE}"

info "Copy VBR into partition"
FAT_HEADER_SIZE=62 # 90 (for fat32)
VBR_START=$((PARTITION_START * SECTOR_SIZE + FAT_HEADER_SIZE))
try dd if=/trainos/build/boot/legacy/vbr.bin of="${DISK}" bs=1 seek=${VBR_START} conv=notrunc

info "Copy OS files"
try mkdir -p /trainos/build/disk
try sudo mount "${PARTITION_DEVICE}" /trainos/build/disk
try echo "TOTO" | sudo tee /trainos/build/disk/hello
try sudo umount /trainos/build/disk
try sudo losetup -d "${LOOP_DEVICE}"
try rm -rf /trainos/build/disk
