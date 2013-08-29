/* 
 * off_t   name          
 *  00h   CDMACR   CDMA Control Register
 *  04h   CDMASR   CDMA Status Register
 *  18h     SA     Source Address Register
 *  20h     DA     Destination Address Register
 *  28h     BTT    Bytes to Transfer Register
 */

#include <xil_printf.h>
#include <xil_cache.h>
#include <stdlib.h>
#include <stdio.h>
#include <xuartps.h>
#include "xil_mmu.h"


#define CDMA_BASE_ADDR 	    0x60000000
#define MY_SIZE_BYTE        64

/* Write to memory location or register */
#define X_mWriteReg(BASE_ADDRESS, RegOffset, data) \
           *(unsigned int *)(BASE_ADDRESS + RegOffset) = ((unsigned int) data);
/* Read from memory location or register */
#define X_mReadReg(BASE_ADDRESS, RegOffset) \
           *(unsigned int *)(BASE_ADDRESS + RegOffset);

int main()
{
    // src: same BRAM different port
    char * srcDMA = (char *) 0x80000000; // from BRAM
    char * srcCPU = (char *) 0x70000000; // from BRAM
    // dst: On Chip Memory
    char * dstDMA = (char *) 0xFFFF8000; // to OCM
    char * dstCPU = (char *) 0xFFFF8000; // to OCM

    volatile unsigned int i;
    volatile int value;
    unsigned int addresList[] = {0x0,0x18,0x20,0x28,0x4};

    /* set HIGH OCM shareable
     * S=b1 TEX=b100 AP=b11, Domain=b0, C=b1, B=b1 */
    Xil_SetTlbAttributes(0xFFF00000, 0x14c0e);

    xil_printf("\n\rHello World");
    xil_printf("\n\rsrcCPU addr = 0x%x",(unsigned int)srcCPU);
    xil_printf("\n\rsrcDMA addr = 0x%x",(unsigned int)srcDMA);
    xil_printf("\n\rdst addr = 0x%x",(unsigned int)dstCPU);

    //????//
    value = X_mReadReg(0xF8000910,0x0);
    xil_printf("\n\r0x%x = 0x%x",0xF8000910, value);
    value = X_mReadReg(0xF8F00000,0x0);
    xil_printf("\n\r0x%x = 0x%x",0xF8000000, value);
    value = X_mReadReg(0xF8F00040,0x0);
    xil_printf("\n\r0x%x = 0x%x",0xF8000040, value);
    value = X_mReadReg(0xF8F00044,0x0);
    xil_printf("\n\r0x%x = 0x%x",0xF8000044, value);

    // init src and dst CPU
    for (i = 0; i<MY_SIZE_BYTE; i++)
    {
        srcCPU[i] = i+1;
        dstCPU[i] = 0xCD;
    }
    // output src and dst CPU after init
	xil_printf("\n\r ... src CPU memory");
    for (i = 0; i<MY_SIZE_BYTE; i++)
        xil_printf("\n\r0x%02x", srcCPU[i]);
    xil_printf("\n\r ... dst CPU memory");
    for (i = 0; i<MY_SIZE_BYTE; i++)
        xil_printf("\n\r0x%02x", dstCPU[i]);

    xil_printf("\n\r ... cdma default status");
    value = X_mReadReg( CDMA_BASE_ADDR, 0x4);
    xil_printf("\n\r0x%08x = 0x%08x",CDMA_BASE_ADDR+0x4,value);

    xil_printf("\n\r ... programming");

    X_mWriteReg(CDMA_BASE_ADDR, 0x0, 0x00005000); // set control reg
    value = X_mReadReg( CDMA_BASE_ADDR, 0x0);
    xil_printf("\n\r0x%08x = 0x%08x",CDMA_BASE_ADDR+0x0, value);

    X_mWriteReg(CDMA_BASE_ADDR, 0x18, (unsigned int)srcDMA); // set src addr reg
    value = X_mReadReg( CDMA_BASE_ADDR, 0x18);
    xil_printf("\n\r0x%08x = 0x%08x", CDMA_BASE_ADDR+0x18, value);

    X_mWriteReg(CDMA_BASE_ADDR, 0x20, (unsigned int)dstDMA); // set dst addr reg
    value = X_mReadReg( CDMA_BASE_ADDR, 0x20);
    xil_printf("\n\r0x%08x = 0x%08x", CDMA_BASE_ADDR+0x20, value);

    X_mWriteReg(CDMA_BASE_ADDR, 0x28, 0x00000040); // set number of Byte trans reg
    value = X_mReadReg( CDMA_BASE_ADDR, 0x28);
    xil_printf("\n\r0x%08x = 0x%08x",CDMA_BASE_ADDR+0x28,value);

    xil_printf("\n\r ... polling DMA status");
    for (i=0; i<5; i++)
    {
        value = X_mReadReg( CDMA_BASE_ADDR, 0x4); // get DMA status
        xil_printf("\n\r0x%08x = 0x%08x",CDMA_BASE_ADDR+0x4,value);
    }

    xil_printf("\n\r ... show DMA registers");
    for (i=0; i<4; i++)
    {
        value = X_mReadReg( CDMA_BASE_ADDR, addresList[i]);
        xil_printf("\n\r0x%08x = 0x%08x",CDMA_BASE_ADDR+addresList[i],value);
    }

    // output src and dst CPU after DMA
    xil_printf("\n\r ... src memory");
	for (i=0; i<MY_SIZE_BYTE; i++)
	{
        xil_printf("\n\r0x%08x = 0x%02x",(unsigned int)(srcCPU+i),srcCPU[i]);
    }
    xil_printf("\n\r ... dst memory");
	for (i=0; i<MY_SIZE_BYTE; i++)
	{
        xil_printf("\n\r0x%08x = 0x%02x",(unsigned int)(dstCPU+i),dstCPU[i]);
	}

    return 0;
}

