#ifndef THREAD_H_
#define THREAD_H_

#include "SyncEvent.h"

class Thread
{
public:
    static const int DEFAULT_STACK_SIZE = -1;

    Thread(int stackSize);
    virtual ~Thread();

    void                    Run();
    void                    Join();
    void                    RequestExit();

protected:
    virtual int             Execute() = 0;

    bool                    IsExitRequested() const;
    int                     GetExitCode() const;

private:
    DISALLOW_COPY_AND_ASSIGN(Thread);

    static void *           ThreadEntryFunction(void *userData);
    pthread_attr_t          m_ThreadAttribute;
    pthread_t               m_ThreadHandle;
    SyncEvent               m_ExitEvent;
    int                     m_ExitCode;
};

#endif // THREAD_H_
