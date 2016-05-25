#include "StdAfx.h"
#include "Thread.h"
#include <limits.h>

Thread::Thread(int stackSize)
    : m_ThreadAttribute()
    , m_ThreadHandle()
    , m_ExitEvent(false)
    , m_ExitCode(0)
{
    int result = pthread_attr_init(&m_ThreadAttribute);
    ASSERT(result == 0);

    if (stackSize != DEFAULT_STACK_SIZE)
    {
        stackSize = std::max<int>(stackSize, PTHREAD_STACK_MIN);
        result = pthread_attr_setstacksize(&m_ThreadAttribute, stackSize);
        ASSERT(result == 0);
    }
}

// virtual
Thread::~Thread()
{
    int result = pthread_attr_destroy(&m_ThreadAttribute);
    ASSERT(result == 0);
}

void Thread::Run()
{
    int result = pthread_create(&m_ThreadHandle, &m_ThreadAttribute, ThreadEntryFunction, this);
    ASSERT(result == 0);
}

void Thread::Join()
{
    RequestExit();
    int result = pthread_join(m_ThreadHandle, NULL);
    ASSERT(result == 0);
}

// static
void * Thread::ThreadEntryFunction(void *userData)
{
    Thread *thread = reinterpret_cast<Thread *>(userData);
    ASSERT(thread != NULL);
    
    if (thread != NULL)
    {
        int exitCode = thread->Execute();
        thread->m_ExitCode = exitCode;
        thread->m_ExitEvent.WaitInfinite();
    }

    return NULL;
}

void Thread::RequestExit()
{
    m_ExitEvent.Set();
}

bool Thread::IsExitRequested() const
{
    return m_ExitEvent.IsSet();
}

int Thread::GetExitCode() const
{
    ASSERT(m_ExitEvent.IsSet());
    return m_ExitCode;
}
