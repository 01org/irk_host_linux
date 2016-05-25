#ifndef SERVER_DISCONNECT_MESSAGE_H_
#define SERVER_DISCONNECT_MESSAGE_H_
#include "OutputMessage.h"

class CommunicationBuffer;

class ServerDisconnectMessage : public OutputMessage
{
public:
    ServerDisconnectMessage();
    virtual ~ServerDisconnectMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ServerDisconnectMessage); 
};

#endif // SERVER_DISCONNECT_MESSAGE_H_
