CC = arm-xilinx-linux-gnueabi-gcc
CFLAGS = -g

all : led_blink

led_blink : led_blink.o
	${CC} ${CFLAGS} -o $@ $^

clean: 
	rm -rfv *.o

.PHONY: clean

