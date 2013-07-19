
#include <stdio.h>
#include "xil_types.h"
#include "sleep.h"
#include "xil_io.h"
#include "platform.h"


void write_bram(u16 offset, u8 value)
{
    Xil_Out8(XPAR_BRAM_0_BASEADDR + offset, value);
}

struct Color
{
    u8 R:4;
    u8 G:4;
    u8 B:4;
};

static struct Color color;

void write_color()
{
    write_bram(0, color.R);
    write_bram(1, color.G);
    write_bram(2, color.B);
}

int main()
{
    int i;
    init_platform();
    printf("Hello World\n\r");
    i = 0;
    for(;;)
    {
        printf("show %d\n\r", i);
        write_color();
        sleep(2);
        ++i;
        switch (i % 3)
        {
            case 0: ++color.B; break;
            case 1: ++color.G; break;
            case 2: ++color.R; break;
            default: break;
        }
    }

    return 0;
}

