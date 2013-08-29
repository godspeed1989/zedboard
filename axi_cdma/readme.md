###CDMA transfers from BRAM to OCM

This BRAM attached to the CPU via M_AXI_GP0.
The same block RAM is also accessible by the CDMA.
The CPU initializes the block RAM.
The CDMA in is transferring data from the block RAM to the OCM via ACP port.
The transfer is cache coherent and when the transfer is complete, the CPU sees the updated OCM without invalidating or flushing the cache.

*  The HIGH OCM must be accessible through the ACP port.
   In the .MHS, the processing_system7 has PARAMETER C_S_AXI_ACP_ENABLE_HIGHOCM = 1.

*   The MMU table has the HIGH OCM shareable.
    /* S=b1 TEX=b100 AP=b11, Domain=b0, C=b1, B=b1 */
    Xil_SetTlbAttributes(0xFFF00000, 0x10C06);

![bus interface][arch]
[arch]: https://raw.github.com/godspeed1989/zedboard/master/axi_cdma/arch.jpg
![addr space][addr]
[addr]: https://raw.github.com/godspeed1989/zedboard/master/axi_cdma/addr.jpg

