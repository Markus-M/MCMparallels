
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__TASK_H__)
#define __TASK_H__

#include <ucontext.h>

class TaskQueue;

class Task
{

public:

	TaskQueue 	*m_Instance;
	ucontext_t	*m_Context;
	bool		m_Cancelled;
	bool		m_Done;

	Task() : m_Instance(0),
			  m_Cancelled(false),
			  m_Done(false)
	{}

	virtual ~Task()
	{}

	virtual void run() = 0;

	inline bool getCancelled() const
	{
		return m_Cancelled;
	}

	inline void cancel()
	{
		m_Cancelled = true;
	}

	bool isDone() const
	{
		return m_Done;
	}

	void setDone()
	{
		m_Done = true;
	}

	void yield();
};


#endif // #if !defined(__TASK_H__)
