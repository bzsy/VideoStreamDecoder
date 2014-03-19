#ifndef _INTERFACE_H_H_
#define _INTERFACE_H_H_

#define MAX_STREAM_BUF_SIZE 655360
#define MAX_YUV_BUF_SIZE 1920*1080*3/2

#pragma comment(lib,"../bin/avcodec-52.lib")
#pragma comment(lib,"../bin/avutil-50.lib")

#ifdef __cplusplus
extern "C" {
#endif

	//decode
	long createDecH264(void);
	long createDecAVS(void);
	long createDecH263(void);
	long createDecMPEG4(void);
	int doDecFrame(long handle, unsigned char * in, int dec_stream_size, int *dec_yuv_width, int *dec_yuv_height);
	void getDataFrame(long handle, unsigned char * out);
	void exitDecoder(long handle);

#ifdef __cplusplus
}
#endif


#endif
