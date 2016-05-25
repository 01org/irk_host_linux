#ifndef SUSPEND_GCC_MESSAGE_H_
#define SUSPEND_GCC_MESSAGE_H_

#include "InputMessage.h"

class SuspendGCCMessage : public InputMessage
{    
public:
    SuspendGCCMessage();
    virtual ~SuspendGCCMessage();

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(SuspendGCCMessage);
};

#endif // SUSPEND_GCC_MESSAGE_H_
