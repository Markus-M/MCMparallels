
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__TASK_SCHEDULER_H__)
#define __TASK_SCHEDULER_H__

class TaskManager;

class TaskScheduler
{

	virtual size_t	scheduleNextTask(Task *instance) = 0;


protected:

	friend class TaskManager;
	void	setTaskManager(TaskManager *manager)
	{
		m_TaskManager = manager;
	}



	TaskManager			*m_TaskManager;
};



#endif // #if !defined(__TASK_SCHEDULER_H__)
