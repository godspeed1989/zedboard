#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_write(unsigned char c)
{
	FILE* fp;
	fp = fopen("/proc/myled", "w");
	if(fp == NULL)
	{
		printf("Cannot open /proc/myled for write\n");
		return;
	}
	printf("0x%x\n", c);
	fprintf(fp, "0x%x\n", c);
	sleep(1);
	fclose(fp);
}

int main()
{
	int i;
	unsigned char c;
	c = 0x55;
	for(i=0; i<10; i++)
	{
		do_write(c);
		c = ~c;
	}
    return 0;
}

