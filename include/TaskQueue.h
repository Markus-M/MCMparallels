
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__TASK_QUEUE_H__)
#define __TASK_QUEUE_H__

#include <ucontext.h>
#include <list>
#include <boost/thread/mutex.hpp>

class Task;

class TaskQueue
{
public:

	TaskQueue();

	void 			operator()();

	static void 	execute(Task *instance);

	void 			add(Task *instance);

	        void   yield();
	static void 	yield(Task &t);
	static void 	stop(Task &t);

	pthread_t		getThreadId() const;
private:

	ucontext_t 				m_BaseContext;
	ucontext_t 				*m_Current;
	std::list<ucontext_t*>	m_Contexts;

	boost::mutex 			locker;
	pthread_t				m_ThreadId;
};


#endif //#if !defined(__TASK_QUEUE_H__)
