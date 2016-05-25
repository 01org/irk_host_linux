#ifndef CONNECTION_REQUEST_HANDLER_H_
#define CONNECTION_REQUEST_HANDLER_H_

#include "CriticalSection.h"

class IPAddress;
class NetworkSocket;
class Server;
class ServerBroadcastThread;

class ConnectionRequestHandler
{
public:
    ConnectionRequestHandler(Server *server, const IPAddress *local_address);
    virtual ~ConnectionRequestHandler();

    void Update();
    void Suspend();
    void Resume();

    void NotifyServerInfoChanged();
    bool HasAddress(const IPAddress *address) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ConnectionRequestHandler);

    Server *server_;
    IPAddress *local_address_;
    NetworkSocket *tcp_accept_socket_;
    ServerBroadcastThread *broadcast_thread_;
    BYTE *buffer_;
    int buffer_size_;
};

#endif // CONNECTION_REQUEST_HANDLER_H_
