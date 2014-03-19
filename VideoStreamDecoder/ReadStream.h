#pragma once

class CReadStream
{
public:
	CReadStream(void);
	~CReadStream(void);

private:
	static int getOneMPEG4Frame(FILE *fp, unsigned char *in);
	static int getOne264Frame(FILE *fp, unsigned char * in);
	static int getOneAVSFrame(FILE *fp, unsigned char * in_buf);
	static int getOne263Frame(FILE *fp, unsigned char * in);

public:
	static int getOneFrame(FILE *fp, unsigned char * in, int stream_type);

};
