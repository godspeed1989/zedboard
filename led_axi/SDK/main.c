

#include <stdio.h>
#include "platform.h"
#include "sleep.h"
#include "xil_io.h"

#define led_base_addr 0x76a00000

int main()
{
    init_platform();
    int i;
    print("Hello World\n\r");
    while(1)
    {
        i = 0;
        printf("show %d\n\r", i++);
        Xil_Out32(led_base_addr,0x00);
        sleep(1);
        printf("show %d\n\r", i++);
        Xil_Out32(led_base_addr,0x55);
        sleep(1);
        printf("show %d\n\r", i++);
        Xil_Out32(led_base_addr,0xaa);
        sleep(1);
    }

    return 0;
}
