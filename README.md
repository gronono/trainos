# Building

## Toolchain

```
apt-get install -y \
        build-essential \
        nasm \
        qemu-system-x86
```

## Build

```
make
```

## Booting

```
qemu-system-i386 -drive format=raw,file=build/boot.bin,if=ide,index=0,media=disk
```