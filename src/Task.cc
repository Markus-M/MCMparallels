
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "Task.h"
#include "TaskQueue.h"


void
Task::yield()
{
	TaskQueue::yield(*this);
	//m_Instance->yield();
}
