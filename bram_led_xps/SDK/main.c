/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include "xparameters.h"
#include "xil_types.h"
#include "sleep.h"
#include "xil_io.h"
#include "platform.h"


void light_led(u8 value)
{
    Xil_Out32(XPAR_BRAM_LED_0_BASEADDR, value & 0x0F);
    Xil_Out32(XPAR_BRAM_0_BASEADDR, value >> 4);
    printf("%x\n\r", value);
}

int main()
{
    int i;

    init_platform();

    printf("Hello World\n\r");
    i = 0;
    while(1)
    {
        printf("show %d\n\r", i++);
        light_led(0x00);
        sleep(1);
        printf("show %d\n\r", i++);
        light_led(0x55);
        sleep(1);
        printf("show %d\n\r", i++);
        light_led(0xaa);
        sleep(1);
        printf("show %d\n\r", i++);
        light_led(0xff);
        sleep(1);
    }

    return 0;
}
