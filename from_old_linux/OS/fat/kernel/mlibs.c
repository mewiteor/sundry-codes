#include"mlibs.h"

void disp_str(const char* info)
{
	disp_color_str(info,0xf);
}

char* itoa(char* str,u32 num)
{
	*str++='0';
	*str++='x';
	if(!num)
		*str++='0';
	else
	{
		int i;
		u8 flag=0;
		for(i=28;i>=0;i-=4)
		{
			char c = num>>i&0xf;
			if(flag||c)
			{
				flag=1;
				if(c<10)c+='0';
				else c+='A'-10;
				*str++=c;
			}
		}
	}
	*str=0;
	return str;
}

void disp_int(u32 num)
{
	char output[16];
	itoa(output,num);
	disp_str(output);
}
