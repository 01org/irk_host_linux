#ifndef DISCONNECT_MESSAGE_H_
#define DISCONNECT_MESSAGE_H_
#include "InputMessage.h"

class DisconnectMessage : public InputMessage
{    
public:
    DisconnectMessage();
    virtual ~DisconnectMessage();

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(DisconnectMessage);
};

#endif // DISCONNECT_UOIP_MESSAGE_H_
