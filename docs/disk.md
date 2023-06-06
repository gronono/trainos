# Disk structure (FAT32)

| Offset | Size | Total | Value      | Description                        |
|--------|------|-------|------------|------------------------------------|
| 0x00   |    3 |     3 | EB 3C 90   | jump to skip FAT32 headers         |
| 0x03   |    8 |    11 | 'MSWIN4.1' | OEM identifier. Recommended value. |
| 0x0B   |    2 |    13 | 00 02      | Bytes per sector                   |
| 0x0D   |    1 |    14 | 04         | Number of sector per cluster       |
| 0x0E   |    2 |    16 | 01 00      | Number of reserved sectors         |
| 0x10   |    1 |    17 | 02         | Number of File Allocation Tables   |
| 0x11   |    2 |    19 | 00 02      | Number of root directory entries   | 
| 0x13   |    2 |    21 | | The total sectors in the logical volume
| 0x15   |    1 |    22 | | Media type descriptor |
| 0x16   |    2 |    24 | | Number of sectors per FAT. FAT12/FAT16 only |
| 0x18   |    2 |    26 | | Number of sectors per track.
| 0x1A   |    2 |    28 | | Number of heads or sides on the storage media.
| 0x1C   |    4 |    32 | | Number of hidden sectors. (i.e. the LBA of the beginning of the partition.)
| 0x20   |    4 |    36 | | Large sector count. This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13.

eb 58 90
6d 6b 66 73 2e 66 61 74
00 02
01
20 00
02
00 00
00 00
f8
00 00
20 00
08 00
00 00 00 00
00 00 04 00
e1 07 00 00
00 00
00 00
02 00 00 00
01 00
06 00
00 00 00 00 00 00 00 00 00 00 00 00
80
00
29
e2 03 66 0f
4e 4f 20 4e 41 4d 45 20 20 20 20
46 41 54 33 32 20 20 20

52 52 61 41
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
72 72 41 61
1d f0 03 00
02 00 00 00

00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 aa  |..............U.|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........

eb 58 90 6d 6b 66 73 2e 66 61 74 00 02 01 20 00  |.X.mkfs.fat... .|
02 00 00 00 00 f8 00 00 20 00 08 00 00 00 00 00  |........ .......|
00 00 04 00 e1 07 00 00 00 00 00 00 02 00 00 00  |................|
01 00 06 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|
80 00 29 e2 03 66 0f 4e 4f 20 4e 41 4d 45 20 20  |..)..f.NO NAME  |
20 20 46 41 54 33 32 20 20 20 0e 1f be 77 7c ac  |  FAT32   ...w|.|
22 c0 74 0b 56 b4 0e bb 07 00 cd 10 5e eb f0 32  |".t.V.......^..2|
e4 cd 16 cd 19 eb fe 54 68 69 73 20 69 73 20 6e  |.......This is n|
6f 74 20 61 20 62 6f 6f 74 61 62 6c 65 20 64 69  |ot a bootable di|
73 6b 2e 20 20 50 6c 65 61 73 65 20 69 6e 73 65  |sk.  Please inse|
72 74 20 61 20 62 6f 6f 74 61 62 6c 65 20 66 6c  |rt a bootable fl|
6f 70 70 79 20 61 6e 64 0d 0a 70 72 65 73 73 20  |oppy and..press |
61 6e 79 20 6b 65 79 20 74 6f 20 74 72 79 20 61  |any key to try a|
67 61 69 6e 20 2e 2e 2e 20 0d 0a 00 00 00 00 00  |gain ... .......|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........
00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 aa  |..............U.|

52 52 61 41 00 00 00 00 00 00 00 00 00 00 00 00  |RRaA............|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........
00 00 00 00 72 72 41 61 1d f0 03 00 02 00 00 00  |....rrAa........|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 55 aa  |..............U.|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........
f8 ff ff 0f ff ff ff 0f f8 ff ff 0f 00 00 00 00  |................|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........
f8 ff ff 0f ff ff ff 0f f8 ff ff 0f 00 00 00 00  |................|
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |.........