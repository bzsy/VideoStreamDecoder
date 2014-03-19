// **********************************************************************
// ����: hzq
// �汾: 1.0
// ����: 2011-03-10
// �޸���ʷ��¼: 
// ����, ����, �������
// **********************************************************************

#ifndef _BASE_THREAD_H_     
#define _BASE_THREAD_H_

#include <process.h>

//�̻߳���
class CBaseThread
{
public:
	CBaseThread();
	~CBaseThread();

	bool start(void* parent);
	void shutdown();

protected:
	void join();
	
	virtual bool on_start() = 0;
	virtual void on_close() = 0;

	//�߳���ں���
	static unsigned int WINAPI _entry(LPVOID inThread);
	
	//�߳�ʵ����ں�����������
	virtual void entry() = 0;

public:
	bool _joined;//�߳��Ƿ񼴽��ر�

protected:
	void* _parent_ptr;//�߳�������ָ��
	HANDLE _thread_handle;
};

#endif  //_BASE_THREAD_H_ 