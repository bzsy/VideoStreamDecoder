#include "StdAfx.h"
#include "DecodeThread.h"
#include "Resource.h"
#include "Interface.h"
#include "ReadStream.h"
#include "VideoStreamDecoderDlg.h"

CDecodeThread::CDecodeThread(int type, char *filename)
{
	this->type = type;
	this->filename = filename;
	fp_in = NULL;
	fp_out = NULL;
	yuv_buf = NULL;
	stream_buf = NULL;
	dec_handle = -1;
	stream_size = 0;
	dec_w = dec_h = 0;
	processed_frame_num = 0;
}

CDecodeThread::~CDecodeThread()
{
}

long (*createDec[5])(void) = {NULL, createDecH263, createDecMPEG4, createDecH264, createDecAVS};

bool CDecodeThread::on_start()
{
	fopen_s(&fp_in, filename, "rb");
	CString temp;
	temp.Format("%s.yuv",filename);
	fopen_s(&fp_out, temp, "wb");
    dec_handle = createDec[type]();
	if (dec_handle < 0)
	{
		return false;
	}
	yuv_buf = (unsigned char *)malloc(MAX_YUV_BUF_SIZE*sizeof(char));
	if (NULL == yuv_buf)
	{
		return false;
	}
	stream_buf = (unsigned char *)malloc(MAX_STREAM_BUF_SIZE*sizeof(char));
	if (NULL == stream_buf)
	{
		return false;
	}
	
	return true;
}

void CDecodeThread::entry()
{
	stream_size = CReadStream::getOneFrame(fp_in, stream_buf, type);
	if (stream_size > 0)
	{
		if (doDecFrame(dec_handle, stream_buf, stream_size, &dec_w, &dec_h))
		{
			getDataFrame(dec_handle, yuv_buf);
			fwrite(yuv_buf, dec_w * dec_h * 3 / 2, 1, fp_out);

			++processed_frame_num;
			temp.Format("Processing...%d", processed_frame_num);
			((CVideoStreamDecoderDlg *)_parent_ptr)->edit_info.SetWindowTextA(temp);
		}
	}
	else
	{
		_joined = true;
		fclose(fp_out);
		fp_out = NULL;
		((CVideoStreamDecoderDlg *)_parent_ptr)->edit_info.SetWindowTextA("Finished.");
	}
}

void CDecodeThread::on_close()
{
	if (dec_handle > 0)
	{
		exitDecoder(dec_handle);
		dec_handle = -1;
	}
	if (NULL != fp_in)
	{
		fclose(fp_in);
		fp_in = NULL;
	}
	if (NULL != fp_out)
	{
		fclose(fp_out);
		fp_out = NULL;
	}
	if (NULL != yuv_buf)
	{
		free(yuv_buf);
		yuv_buf = NULL;
	}
	if (NULL != stream_buf)
	{
		free(stream_buf);
		stream_buf = NULL;
	}
}
