#ifndef __MYLED_H__
#define __MYLED_H__

// wether use configuration from .dtb file
//#define USE_DTD

#ifdef USE_DTD

#else
	#define RS_START   0x7E400000
	#define RS_END     0x7E40FFFF
#endif

#endif

