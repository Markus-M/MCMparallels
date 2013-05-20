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
