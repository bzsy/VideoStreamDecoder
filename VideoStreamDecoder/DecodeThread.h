// **********************************************************************
//½âÂëÏß³Ì
// **********************************************************************

#ifndef _TASK_THREAD_H_     
#define _TASK_THREAD_H_

#include <stdio.h>
#include "BaseThread.h"

class CDecodeThread :public CBaseThread
{
public:
	CDecodeThread(int type, char *filename);
	~CDecodeThread();

	bool on_start();
	void on_close();
	void entry();

public:
	int type;
	char *filename;
	FILE *fp_in;
	FILE *fp_out;
	unsigned char *yuv_buf;
	unsigned char *stream_buf;
	long dec_handle;
	int stream_size;
	int dec_w, dec_h;
	int processed_frame_num;
	CString temp;

};


#endif  //_TASK_THREAD1_H_ 