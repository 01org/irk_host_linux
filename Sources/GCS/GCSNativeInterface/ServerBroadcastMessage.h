#ifndef SERVER_BROADCAST_MESSAGE_H_
#define SERVER_BROADCAST_MESSAGE_H_

#include "OutputMessage.h"

class CommunicationBuffer;
class Server;

class ServerBroadcastMessage : public OutputMessage
{
public:
    ServerBroadcastMessage(const IPAddress *ipAddress, Server *server);
    virtual ~ServerBroadcastMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ServerBroadcastMessage); 

    IPAddress *ip_address_;
    Server *server_;
};

#endif // SERVER_BROADCAST_MESSAGE_H_