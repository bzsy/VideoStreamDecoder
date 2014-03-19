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

	bool flg = on_start();  //on_start要在最开始调用
	if(!flg) return false;

	_thread_handle = (HANDLE)_beginthreadex( 
										NULL,  //安全属性，NULL为默认安全属性
										   0,  //指定线程堆栈的大小。如果为0，则线程堆栈大小和创建它的线程的相同。一般用0
									  _entry,  //第3个参数：指定线程函数的地址，也就是线程调用执行的函数地址(用函数名称即可，函数名称就表示地址)
								 (void*)this,  //传递给线程的参数的指针，可以通过传入对象的指针，在线程函数中再转化为对应类的指针
										   0,  //线程初始状态，0:立即运行；CREATE_SUSPEND：suspended（悬挂）
										 &id   //用于记录线程ID的地址
										 );

	SuspendThread(_thread_handle);//暂停(挂起)线程 
	SetThreadPriority(_thread_handle, THREAD_PRIORITY_HIGHEST);//设置线程优先级
	ResumeThread(_thread_handle);//恢复线程运行

	_joined = false;

	return true;
}

void CBaseThread::shutdown()
{
	_joined = true;

	if(NULL!=_thread_handle) join();

	on_close();  //on_close要在最终调用
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