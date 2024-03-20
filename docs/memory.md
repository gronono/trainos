The CPU starts in real mode, which means it operates in 16 bits with a RAM address space of 1MB.

To address memory, it uses a segment register and an offset.
Addresses are written in the form <segment>:<offset>.
The physical address is determined by the formula: ```PhysicalAddress = Segment * 0x10 + Offset```.
This is what we call segmentation.

The segment registers are determined by the context:
* CS: Code Segment
* DS: Data Segment
* SS: Stack Segment
* ES: Extra Segment

The layout of the first 1MB memory is well known:

| Offset | Size   | Status   | Description        |
|--------|--------|----------|--------------------|
| 0x0000 | 1280   | unusable | BIOS               |
| 0x0500 | 30464  | usable   | Free space         |
| 0x7C00 | 512    | MBR      | Master Boot Record |
| 0x7E00 | 492032 | usable   | Free space         |
| 0x8000 | 524287 | unusable | VGA + BIOS         |

## After MBR

The MBR is in charge to lookup into partition table, ask the user to select the OS boot partition.
Finally, the MBR jump to the specific Volume Boot Root (VBR).

Before the VBR, the memory layout is:

| Offset | Size   | Status | Description        |
|--------|--------|--------|--------------------|
| 0x0500 | 512    | usable | MBR code           |
| 0x0700 | 31232  | usable | Free space         |
| 0x7C00 | 512    | VBR    | Master Boot Record |
| 0x7E00 | 492032 | usable | Free + stack       |

## VBR

First the VBR copy it self at 0x0500 and erase the MBR code.

## Before Stage 2

## Before Kernel
