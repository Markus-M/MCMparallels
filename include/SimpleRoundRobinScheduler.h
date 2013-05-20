
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__SIMPLE_ROUND_ROBIN_SCHEDULER_H__)
#define __SIMPLE_ROUND_ROBIN_SCHEDULER_H__

#include "TaskScheduler.h"
#include "TaskManager.h"

class SimpleRoundRobinScheduler : public TaskScheduler
{
public:

	SimpleRoundRobinScheduler() : m_NextThread(0)
	{}

	virtual size_t	scheduleNextTask(Task *instance)
	{
		size_t ret = m_NextThread;

		m_NextThread = (m_NextThread + 1) % m_TaskManager->getThreadCount();

		return ret;
	}


private:

	size_t 	m_NextThread;
};


#endif // #if !defined(__SIMPLE_ROUND_ROBIN_SCHEDULER_H__)
