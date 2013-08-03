#include <stdio.h>
#include "platform.h"
#include "xil_io.h"
#include "sleep.h"
#include "xparameters.h"
#include "xil_cache.h"

#define SRC_ADDR           0x10000000
#define DST_ADDR           0x10100000
#define NUM_WORDS          8
#define SRC_NUM_WORDS      NUM_WORDS
#define DST_NUM_WORDS      NUM_WORDS
#define BUFFER_SIZE    (NUM_WORDS*4)

#if XPAR_AXI_DMA_0_INCLUDE_SG == 0

/* AXI DMA Simple DMA Mode Register Address (C_INCLUDE_SG = 0) */
#define MM2S_DMACR  0  // control reg, 0x0
#define MM2S_DMASR  1  // status reg, 0x04

#define MM2S_SA      6 // source addr 0x18
#define MM2S_LENGTH 10 // source length 0x28

#define S2MM_DMACR 12  // control reg 0x30
#define S2MM_DMASR 13  // status reg 0x34

#define S2MM_DA     18 // destination addr 0x48
#define S2MM_LENGTH 22 // destination length 0x58

#else
#error "Just for axi simple dma mode"
#endif

inline void out32_and_test(u32 Addr, u32 data)
{
	Xil_Out32(Addr, data);
	while(Xil_In32(Addr) & data);
}

void do_axi_dma_simple(u32 Addr)
{
    unsigned int * p = (unsigned int *)Addr;
    *(p+MM2S_DMACR) = 0x04;  //reset send axi dma
    while(*(p+MM2S_DMACR) & 0x04);
    *(p+S2MM_DMACR) = 0x04;  //reset recv axi dma
    while(*(p+S2MM_DMACR) & 0x04);
    printf(" reset done\n\r");

    *(p+MM2S_DMACR) = 0x1;   // run send dma
    while((*(p+MM2S_DMASR) & 0x01));
    *(p+S2MM_DMACR) = 0x1;   // run recv dma
    while((*(p+S2MM_DMASR) & 0x01));
    printf(" start dma done\n\r");

    *(p+MM2S_SA) = (unsigned int )SRC_ADDR;  // set src addr
    *(p+S2MM_DA) = (unsigned int )DST_ADDR;  // set dst addr

    Xil_DCacheFlushRange(SRC_ADDR, BUFFER_SIZE);

    *(p+S2MM_LENGTH) = BUFFER_SIZE;   // set src length
    *(p+MM2S_LENGTH) = BUFFER_SIZE;   // set src length

    while(!(*(p+MM2S_DMASR) & 0x2)); //wait for send ok ???
}

void print_u32s(u32 Addr, u32 num)
{
    int i;
    unsigned int * p;
    p = (unsigned int*)Addr;
    for(i = 0; i < num; ++i)
        printf("%u  ", p[i]);
    printf("\n\r\n\r");
}

int main()
{
    int i;
    init_platform();

    unsigned int * p;

    p = (unsigned int*)SRC_ADDR;
    printf("init src buffer...\n\r");
    for(i = 0; i < SRC_NUM_WORDS; ++i)
        p[i] = i;

    printf("do dma...\n\r");
    do_axi_dma_simple(XPAR_AXI_DMA_0_BASEADDR);

    printf("show results...\n\r");
    print_u32s(SRC_ADDR, SRC_NUM_WORDS);
    print_u32s(DST_ADDR, DST_NUM_WORDS);
    sleep(1);
    print_u32s(DST_ADDR, DST_NUM_WORDS);

    printf("--------------\n\r");
    cleanup_platform();
    return 0;
}

