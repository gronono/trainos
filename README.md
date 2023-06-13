Requirements:
    docker
    make

Building:
    make toolchain
    make clean
    make build

TODO :
- [x] chore: setup build
- [ ] chore: bochs emulator
- [x] chore: build from docker
- [x] bootloader: hello world stage1
* [ ] bootloader: load stage2 from stage1
- [ ] chore: setup a toolchain for cross-compile
* [ ] bootloader: protected mode from stage 2
* [ ] bootloader: support FAT12/16/32 disks
* [ ] bootloader: run kernel from stage2
* [ ] kernel: make an hello world kernel
* [ ] bootloader / kernel : memory map from ACPI
* [ ] bootloader: support multiboot
* [ ] kernel: support multiboot
* [ ] bootloader: EFI
* [ ] kernel: support FAT12
* [ ] kernel: support FAT16
* [ ] kernel: support FAT32
* [ ] kernel: support exFAT
* [ ] kernel: support ext2
* [ ] kernel: support ext3
* [ ] kernel: support ext4
* [ ] kernel: multiple users
* [ ] kernel: multiple tasks
* [ ] os: tty
* [ ] os: gui
* [ ] kernel: TCP/IP stack
* [ ] os: webbrowser
* [ ] kernel / os : support Linux applications
* [ ] support ARM

