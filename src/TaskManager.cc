#include "TaskManager.h"
#include "TaskQueue.h"
#include "TaskScheduler.h"

#include <stdio.h>

TaskManager* TaskManager::instance = 0;


class QueueThread
{
public:

	QueueThread(TaskQueue &tq) : m_Queue(tq)
	{}

	void operator()()
	{
		m_Queue();
	}

private:

	TaskQueue &m_Queue;
};

void
TaskManager::initTaskManager(size_t threadCount, TaskScheduler &scheduler)
{
	instance = new TaskManager(scheduler);

	instance->m_ThreadCount = threadCount;
	instance->m_Queues = new TaskQueue[threadCount];

//	new boost::thread(QueueThread(instance->m_Queues[0]));

	for (size_t loop = 0; loop < threadCount; ++loop)
	{
		instance->m_Threads.push_back(boost::shared_ptr<boost::thread>(
				new boost::thread(QueueThread(instance->m_Queues[loop]))));
	}
}


TaskManager&
TaskManager::getInstance()
{
	return *instance;
}

void
TaskManager::addTask(Task *instance)
{
	size_t index = m_Scheduler.scheduleNextTask(instance);

	if (index < m_ThreadCount)
	{
		m_Queues[index].add(instance);
	}
}

void
TaskManager::yield()
{
	pthread_t currentThreadId = pthread_self();

	for (size_t loop = 0; loop < m_ThreadCount; ++loop)
	{
		if (m_Queues[loop].getThreadId() == currentThreadId)
		{
			m_Queues[loop].yield();
		}
	}
}


TaskManager::TaskManager(TaskScheduler &scheduler) : m_Scheduler(scheduler)
{
	m_Scheduler.setTaskManager(this);
}
