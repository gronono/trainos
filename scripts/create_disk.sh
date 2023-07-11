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
STAGE2_SIZE=$(stat build/stage2.bin -c %s)
STAGE2_SECTORS=$(( (STAGE2_SIZE + SECTOR_SIZE - 1 ) / SECTOR_SIZE ))
info "Append stage2 size into stage1: size=${STAGE2_SECTORS}s"
hexa=$(printf "%04x" "${STAGE2_SECTORS}" | fold -2 | sed 's/^/\\x/g' | tr '\n' ' ' | awk '{print $2$1}')
printf "%b" "$hexa" >> build/stage1.bin

info "Create empty disk size=${DISK_SIZE}MiB"
try dd if=/dev/zero of="${DISK}" bs=1M count="${DISK_SIZE}"

info "Create msdos table"
try parted -s "${DISK}" mktable msdos

info "Copy Stage 1"
try dd if=build/stage1.bin of="${DISK}" bs=1 seek=0 conv=notrunc

info "Copy Stage 2"
try dd if=build/stage2.bin of="${DISK}" bs=512 seek=1 conv=notrunc

DISK_SECTORS=$(( DISK_SIZE * 1024 * 1024 / SECTOR_SIZE ))
PARTITION_START=2048  # Start of partition (in sectors)
PARTITION_LENGTH=$(( DISK_SECTORS - PARTITION_START - 1 ))
PARTITION_END=$(( PARTITION_START + PARTITION_LENGTH ))
info "Create partition start=${PARTITION_START}s end=${PARTITION_END}s length=${PARTITION_LENGTH}s"
try parted -s "${DISK}" mkpart primary fat32 ${PARTITION_START}s ${PARTITION_END}s
try parted -s "${DISK}" set 1 boot on

info "Format parition"
LOOP_DEVICE=$(sudo losetup -Pf --show "${DISK}")
echo "Loop device: ${LOOP_DEVICE}"
PARTITION_DEVICE="${LOOP_DEVICE}p1"
try sudo mkfs.fat -F 32 -n TRAINOS -f 1 "${PARTITION_DEVICE}"
try mkdir -p build/disk
try sudo mount "${PARTITION_DEVICE}" build/disk
try sudo umount build/disk
try sudo losetup -d "${LOOP_DEVICE}"
try rm -rf build/disk