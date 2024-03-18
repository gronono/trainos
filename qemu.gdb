#add-symbol-file /trainos/build/boot/legacy/stage2.bin 0x700
set disassembly-flavor intel
target remote | qemu-system-x86_64 -m 1G -S -gdb stdio -boot c -drive format=raw,if=ide,index=0,media=disk,file=/trainos/build/trainos.bin
