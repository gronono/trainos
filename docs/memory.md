The CPU starts in real mode, which means it operates in 16 bits with a RAM address space of 1MB.

To address memory, it uses a segment register and an offset. Addresses are written in the form <segment>:<offset>. The physical address is determined by the formula: ```PhysicalAddress = Segment * 0x10 + Offset```. This is what we call segmentation.

The segment registers are determined by the context:
* CS: Code Segment
* DS: Data Segment
* SS: Stack Segment
* ES: Extra Segment

## Before MBR


## Before Stage 1

## Before Stage 2

## Before Kernel
