#ifndef __MYLED_H__
#define __MYLED_H__

#define DEVICE_NAME "myled-1.00.a"
// wether use configuration from .dtb file
#define USE_DTD

#ifdef USE_DTD
	// from .dtb by platform_get_resource()
#else
	#define RS_START   0x76A00000
	#define RS_END     0x76A0FFFF
#endif

#endif

