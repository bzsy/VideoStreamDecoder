#include "StdAfx.h"
#include "ReadStream.h"

CReadStream::CReadStream(void)
{
}

CReadStream::~CReadStream(void)
{
}

int CReadStream::getOneFrame(FILE *fp, unsigned char * in, int stream_type)
{
	int size = 0;
	switch (stream_type)
	{
	case 1:
		size = getOne263Frame(fp, in);
		break;
	case 2:
		size = getOneMPEG4Frame(fp, in);
		break;
	case 3:
		size = getOne264Frame(fp, in);
		break;
	case 4:
		size = getOneAVSFrame(fp, in);
		break;
	default:
		break;
	}
	return size;
}

int CReadStream::getOneMPEG4Frame(FILE *fp, unsigned char *in)
{
	int size = 0;
	int num = 0;
	int flag = 0;

	num = (int)fread(in,1,4,fp);
	if(num<4) return -1;

	if(!(0x00==in[0] && 0x00==in[1] && 0x01==in[2] && 0xB0==in[3]) && !(0x00==in[0] && 0x00==in[1] && 0x01==in[2] && 0xB6==in[3]))
	{
		return -1;
	}

	if(0x00==in[0] && 0x00==in[1] && 0x01==in[2] && 0xB6==in[3])
	{
		flag = 1;
	}

	size+=4;

	for(;;)
	{
		num = (int)fread(in + size, 1, 1, fp);
		if(num<1)
		{
			break;
		}

		size += 1;

		if(size>=8 && (0x00==in[size-4] && 0x00==in[size-3] && 0x01==in[size-2] && 0xB6==in[size-1]) && flag)
		{
			fseek(fp, -4, SEEK_CUR);
			size -= 4;
			break;
		}
		else if(size>=8 && (0x00==in[size-4] && 0x00==in[size-3] && 0x01==in[size-2] && 0xB6==in[size-1]) && !flag)
		{
			flag = 1;
		}
	}
	return size;
}

int CReadStream::getOne264Frame(FILE *fp, unsigned char * in)
{
	int size = 0;
	int num = 0;

	num = (int)fread(in,1,4,fp);
	if(num<4) return -1;
	if(!(0x00==in[0] && 0x00==in[1] && 0x00==in[2] && 0x01==in[3]))
	{
		return -1;
	}

	size+=4;

	for(;;)
	{
		num = (int)fread(in + size, 1, 1, fp);
		if(num<1)
		{
			break;
		}

		size += 1;

		if(size>=9 && ( (0x00==in[size-5] && 0x00==in[size-4] && 0x00==in[size-3] && 0x01==in[size-2] && 0x67==in[size-1]) ||
					    (0x00==in[size-5] && 0x00==in[size-4] && 0x00==in[size-3] && 0x01==in[size-2] && 0x41==in[size-1])
				      )    )
		{
			fseek(fp, -5, SEEK_CUR);
			size -= 5;
			break;
		}
	}
	return size;
}

int CReadStream::getOneAVSFrame(FILE *fp, unsigned char * in_buf)
{
	int size = 0;
	int num = 0;
	int frame_type;

	num = (int)fread(in_buf,1,4,fp);
	if(num<4)
	{
		return -1;
	}
	if(in_buf[0]==0x00 && in_buf[1]==0x00 && in_buf[2]==0x01 && in_buf[3]==0xB0)  //IDR֡
	{
		frame_type = 1;
	}
	else if(in_buf[0]==0x00 && in_buf[1]==0x00 && in_buf[2]==0x01 && in_buf[3]==0xB6)  //P֡
	{
		frame_type = 0;
	}
	else 
	{
		return -1;
	}

	size+=4;

	for(;;)
	{
		num = (int)fread(in_buf + size, 1, 1, fp);
		if(num<1)
		{
			break;
		}

		size += 1;

		if(size>=8 && (in_buf[size-4]==0x00 && in_buf[size-3]==0x00 && in_buf[size-2]==0x01 && (in_buf[size-1]==0xB0 || in_buf[size-1]==0xB6)))
		{
			fseek(fp, -4, SEEK_CUR);
			size -= 4;
			break;
		}
	}

	return size;
}

int CReadStream::getOne263Frame(FILE *fp, unsigned char * in)
{
	int size = 0;
	int num = 0;

	num = (int)fread(in,1,2,fp);
	if(num<2) return -1;
	if(!(0x00==in[0] && 0x00==in[1]))
	{
		return -1;
	}

	size+=2;

	for(;;)
	{
		num = (int)fread(in + size, 1, 1, fp);
		if(num<1)
		{
			break;
		}

		size += 1;

		if(size>=4 && 0x00==in[size-2] && 0x00==in[size-1])
		{
			fseek(fp, -2, SEEK_CUR);
			size -= 2;
			break;
		}
	}
	return size;
}