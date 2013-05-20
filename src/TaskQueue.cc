#include "TaskQueue.h"
#include "Task.h"

#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

TaskQueue::TaskQueue() : m_Current(0)
{}

void
TaskQueue::operator()()
{
	m_ThreadId = pthread_self();

	while (true)
	{
		//std::cout << "running" << std::endl << std::flush;
		{
			boost::mutex::scoped_lock lock(locker);
			if (!m_Contexts.empty())
			{
				m_Current = *m_Contexts.begin();
				m_Contexts.pop_front();
			}
			else
			{
				m_Current = 0;
			}
		}

		if (m_Current)
		{
			if (swapcontext(&m_BaseContext, m_Current) == -1)
				handle_error("swapcontext");
		}
		else
		{
			sleep(1);
		}

		//sleep(1);
	}
}

void
TaskQueue::execute(Task *instance)
{
	if (instance)
	{
		instance->run();
		instance->setDone();
	}
}

void
TaskQueue::add(Task *instance)
{
	instance->m_Instance = this;
	instance->m_Context  = new ucontext_t;
	if (getcontext(instance->m_Context) == -1)
		handle_error("getcontext");
	instance->m_Context->uc_stack.ss_sp = new char[512000 * 10];
	instance->m_Context->uc_stack.ss_size = 512000 * 10;
	instance->m_Context->uc_link = &m_BaseContext;
	makecontext(instance->m_Context, (void (*)())execute, 1, instance);

	{
		boost::mutex::scoped_lock lock(locker);
		m_Contexts.push_back(instance->m_Context);
	}
}

void
TaskQueue::yield(Task &t)
{
	if (t.m_Instance)
	{
		if (!t.getCancelled())
		{
			boost::mutex::scoped_lock lock(t.m_Instance->locker);
			if (t.m_Context)
				t.m_Instance->m_Contexts.push_back(t.m_Context);
		}

		if (swapcontext(t.m_Instance->m_Current, &t.m_Instance->m_BaseContext) == -1)
			handle_error("swapcontext");
	}
}

void
TaskQueue::yield()
{
	{
		boost::mutex::scoped_lock lock(locker);
		if (m_Current)
			m_Contexts.push_back(m_Current);
	}

	if (swapcontext(m_Current, &m_BaseContext) == -1)
		handle_error("swapcontext");
}

void
TaskQueue::stop(Task &t)
{
	if (t.m_Instance)
	{
		{
			boost::mutex::scoped_lock lock(t.m_Instance->locker);
			//t.m_Instance = 0;

			t.m_Instance->m_Contexts.begin();
			t.m_Instance->m_Contexts.end();

			std::list<ucontext_t*>::iterator it = std::find(	t.m_Instance->m_Contexts.begin(),
																	t.m_Instance->m_Contexts.end(),
																	t.m_Context);

			if (t.m_Instance->m_Contexts.end() != it)
			{
				t.m_Instance->m_Contexts.erase(it);
			}

			t.m_Context = 0;
		}

//			if (swapcontext(t.m_Instance->m_Current, &t.m_Instance->m_BaseContext) == -1)
//				handle_error("swapcontext");
	}
}

pthread_t
TaskQueue::getThreadId() const
{
	return m_ThreadId;
}
