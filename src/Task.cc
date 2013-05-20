#include "Task.h"
#include "TaskQueue.h"


void
Task::yield()
{
	TaskQueue::yield(*this);
	//m_Instance->yield();
}
