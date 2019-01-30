#ifndef _KLIBA_H_
#define _KLIBA_H_ 1

#include "type.h"
#include "const.h"

PUBLIC void*	memcpy(void* pDst,const void* pSrc,unsigned int iSize);
PUBLIC void		disp_str(const char* info);
PUBLIC void		gotoxy(int x,int y);
PUBLIC void		out_byte(u16 port,u8 value);
PUBLIC u8  		in_byte(u16 port);
PUBLIC void		cls(u8 x,u8 y,u8 width,u8 height);
PUBLIC void		disp_color_str(const char* info,u8 color);
PUBLIC char*	itoa(char *str,u32 num);
PUBLIC void		disp_int(u32 num);

#endif // _KLIBA_H_
