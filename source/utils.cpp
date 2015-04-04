#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

//---------------------------------------------------------------------------
char *trim(char *s)
{
	if(!s || !*s)
		return s;
	while(*s == 32)
		s++;
	char *p=s;
	while(1){
		if(*p == 0){
			p--;
			break;
		}
		p++;
	}
	while(p!=s && *p==20)
		*p--=0;
	return s;
}
//---------------------------------------------------------------------------
char *strtolower(char *s)
{
	if(!s || !s[0])
		return s;
	for(char *p=s;*p != 0;p++)
		*p = tolower(*p);
	return s;
}
//---------------------------------------------------------------------------	
char *strtoupper(char *s)
{
	if(!s || !s[0])
		return s;
	for(char *p=s;*p != 0;p++)
		*p = toupper(*p);
	return s;
}	
//---------------------------------------------------------------------------
char *ucwords(char *s)
{
	int mode;
	char *p;
	
	if(!s || !s[0])
		return s;
	mode = 1;
	for(p=s;*p != 0;p++){
		char c = *p;
		if(mode){
			*p=toupper(c);
			mode=0;
		}
		mode = (c==' ' || c=='-');
	}
	return s;
}
//---------------------------------------------------------------------------------
int urlencode(char *src,char *dst)
{
	int i;
	char c1[5];
	
	if(!src || !*src || !dst)
		return -1;
	*dst = 0;
	for(i=0;*src != 0;i++){
		unsigned char c = *src++;
		if(c < 128 && (isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~')){
			c1[0] = c;
			c1[1] = 0;
		}
		else{
			sprintf(c1,"%%%02X",(int)c);
			i+=2;
		}
		strcat(dst,c1);
	}	
	return i;
}
//---------------------------------------------------------------------------
char *stripslashes(char *s)
{
   int i,ii;

   if(!s || !*s)
		return s;
   for(i=ii=0;s[i] != 0;i++){
       if(s[i] == '\\'){
           int i0 = i+1;

           if(s[i0] == '/' || s[i0] == '\'' || s[i0] == '"' || s[i0] == '\\')
               continue;
       }
       s[ii++] = s[i];
   }
   s[ii] = 0;
   return s;
}
//---------------------------------------------------------------------------
int printd(char *fmt,...)
{
#ifdef _DEBUG
	va_list argptr;
	int cnt;
	char s[512];

	if(!fmt)
		return -1;
	va_start(argptr, fmt);
	vsprintf(s,fmt, argptr);
	va_end(argptr);
	svcOutputDebugString(s,strlen(s));
#endif
	return 0;
}
//---------------------------------------------------------------------------
u32 write_to_sdmc(const char *filename,u8 *_buf,u32 size)
{
	u32 byteswritten;
	
	/*Result res = FSUSER_OpenFile(NULL,&sram,sdmcArchive,FS_makePath(PATH_CHAR,filename),FS_OPEN_CREATE|FS_OPEN_WRITE,FS_ATTRIBUTE_NONE);
	if (res == 0){
		
		FSFILE_Write(sram, &byteswritten, 0, _buf, size, FS_WRITE_FLUSH);
		FSFILE_Close(sram);
	}*/		
	FILE *fp;
	char *p;
	
	if(!filename || !_buf || !size)
		return 0;
	p = (char *)malloc(strlen(filename)+6);
	if(p == NULL)
		return 0;
	strcpy(p,"sdmc:");
	strcat(p,filename);
	fp = fopen(p,"wb");
	byteswritten = 0;		
	if(fp != NULL){
		byteswritten = fwrite(_buf,1,size,fp);
		fclose(fp);
	}
	return byteswritten;
}
//---------------------------------------------------------------------------
buffer::buffer()
{
	_size = 0;
	_bytes = 0;
	_buf = NULL;
}
//---------------------------------------------------------------------------
buffer::~buffer()
{
	destroy();
}
//---------------------------------------------------------------------------
int buffer::alloc(u32 sz)
{
	char *p;
	
	if(_size > sz)
		return 0;
	sz = (sz >> 10) << 10;
	p = (char *)malloc(sz);
	if(p == NULL)
		return -1;
	if(_bytes)
		memcpy(p,_buf,_bytes);
	_size = sz;
	::free(_buf);
	_buf = p;
	return 0;
}
//---------------------------------------------------------------------------
int buffer::destroy()
{
	if(_buf != NULL){
		::free(_buf);
		_buf = NULL;
		_bytes = _size = 0;
	}
	return 0;
}
//---------------------------------------------------------------------------
int buffer::copy(char *data,u32 size,u32 flags)
{
	if(flags&1)
		_bytes=0;
	if(free() < size)
		alloc(_size + size);
	if(_buf == NULL || free() < size)
		return -1;
	memcpy(&_buf[_bytes],data,size);
	_bytes += size;
	return 0;
}
