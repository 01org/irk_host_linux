#ifndef HEARTBEAT_INPUT_MESSAGE_H_
#define HEARTBEAT_INPUT_MESSAGE_H_

#include "InputMessage.h"

class HeartbeatInputMessage : public InputMessage
{    
public:
    HeartbeatInputMessage();
    virtual ~HeartbeatInputMessage();

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(HeartbeatInputMessage);
};

#endif // HEARTBEAT_INPUT_MESSAGE_H_
