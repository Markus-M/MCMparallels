
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__TASK_MANAGER_H__)
#define __TASK_MANAGER_H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

class Task;
class TaskQueue;
class TaskScheduler;

class TaskManager
{

public:

	static void			initTaskManager(size_t threadCount, TaskScheduler &scheduler);
	static TaskManager& 	getInstance();

	void					addTask(Task *instance);

	inline size_t			getThreadCount() const
	{
		return m_ThreadCount;
	}

	void					yield();

private:

	TaskManager(TaskScheduler &scheduler);
	static TaskManager *instance;

	size_t												m_ThreadCount;
	TaskQueue											*m_Queues;
	std::vector<boost::shared_ptr<boost::thread> >		m_Threads;
	TaskScheduler										&m_Scheduler;
};


#endif //  #if !defined(__TASK_MANAGER_H__)
