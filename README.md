zedboard
========

xilinx-avnet zedboard designs.

###generate `boot.bin`

	bootgen -image boot.bif -o i boot.bin

###compile Linux kernel

	export ARCH=arm
	export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
	make digilent_zed_defconfig
	make all -j3 ARCH=arm

###make device tree blob

	cd <linux-src-dir>
	scripts/dtc/dtc -I dts -O dtb -o arch/arm/boot/devicetree.dtb arch/arm/boot/dts/digilent-zed.dts
	cp arch/arm/boot/devicetree.dtb </path/to/sdcard>

###install modules

	make modules_install INSTALL_MOD_PATH=/home/yl/usb

###xmd commands

	connect arm hw
	fpga -f system.bit
	dow your-program.elf
	run

###boot mode config

![boot config][boot]
[boot]: https://raw.github.com/godspeed1989/zedboard/master/config.png
