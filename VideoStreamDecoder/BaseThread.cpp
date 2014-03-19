#include "StdAfx.h"
#include "BaseThread.h"

CBaseThread::CBaseThread() :
	_thread_handle(NULL),
	_parent_ptr(NULL),
	_joined(false)
{
	//...
}

CBaseThread::~CBaseThread()
{
	//...
}

bool CBaseThread::start(void* parent)
{
	unsigned int id(0);

	_parent_ptr = parent;

	bool flg = on_start();  //on_startҪ���ʼ����
	if(!flg) return false;

	_thread_handle = (HANDLE)_beginthreadex( 
										NULL,  //��ȫ���ԣ�NULLΪĬ�ϰ�ȫ����
										   0,  //ָ���̶߳�ջ�Ĵ�С�����Ϊ0�����̶߳�ջ��С�ʹ��������̵߳���ͬ��һ����0
									  _entry,  //��3��������ָ���̺߳����ĵ�ַ��Ҳ�����̵߳���ִ�еĺ�����ַ(�ú������Ƽ��ɣ��������ƾͱ�ʾ��ַ)
								 (void*)this,  //���ݸ��̵߳Ĳ�����ָ�룬����ͨ����������ָ�룬���̺߳�������ת��Ϊ��Ӧ���ָ��
										   0,  //�̳߳�ʼ״̬��0:�������У�CREATE_SUSPEND��suspended�����ң�
										 &id   //���ڼ�¼�߳�ID�ĵ�ַ
										 );

	SuspendThread(_thread_handle);//��ͣ(����)�߳� 
	SetThreadPriority(_thread_handle, THREAD_PRIORITY_HIGHEST);//�����߳����ȼ�
	ResumeThread(_thread_handle);//�ָ��߳�����

	_joined = false;

	return true;
}

void CBaseThread::shutdown()
{
	_joined = true;

	if(NULL!=_thread_handle) join();

	on_close();  //on_closeҪ�����յ���
}

void CBaseThread::join()
{
    DWORD theErr = ::WaitForSingleObject(_thread_handle, INFINITE);
}

unsigned int WINAPI CBaseThread::_entry(LPVOID inThread)
{
    CBaseThread* theThread = (CBaseThread*)inThread;

	if(NULL == theThread)
		return -1;

	while(false == theThread->_joined)
	{
		theThread->entry();
	}

    return 0;
}