// **********************************************************************
// 作者: hzq
// 版本: 1.0
// 日期: 2011-03-10
// 修改历史记录: 
// 日期, 作者, 变更内容
// **********************************************************************

#ifndef _BASE_THREAD_H_     
#define _BASE_THREAD_H_

#include <process.h>

//线程基类
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

	//线程入口函数
	static unsigned int WINAPI _entry(LPVOID inThread);
	
	//线程实际入口函数，重载用
	virtual void entry() = 0;

public:
	bool _joined;//线程是否即将关闭

protected:
	void* _parent_ptr;//线程启动者指针
	HANDLE _thread_handle;
};

#endif  //_BASE_THREAD_H_ 