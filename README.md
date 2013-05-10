zedboard
========

xilinx-avnet zedboard designs.

###generate `boot.bin`

	bootgen -image boot.bif -o i boot.bin

###compile kernel

	export ARCH=arm
	export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
	make digilent_zed_defconfig
	make -j3 ARCH=arm

###Device Tree

	cd <linux-src-dir>
	scripts/dtc/dtc -I dts -O dtb -o arch/arm/boot/devicetree.dtb arch/arm/boot/dts/digilent-zed.dts
	cp arch/arm/boot/devicetree.dtb </path/to/sdcard>

