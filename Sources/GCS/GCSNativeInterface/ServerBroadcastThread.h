#ifndef SERVER_BROADCAST_THREAD_H_
#define SERVER_BROADCAST_THREAD_H_

#include "CriticalSection.h"
#include "Thread.h"

class IPAddress;
class NetworkSocket;
class Server;

class ServerBroadcastThread : public Thread
{
public:
    ServerBroadcastThread(const IPAddress *broadcast_address, const IPAddress *connection_address, Server *server);
    virtual ~ServerBroadcastThread();

    void Suspend();
    void Resume();

    void NotifyServerInfoChanged();

protected:
    virtual int Execute();

private:
    DISALLOW_COPY_AND_ASSIGN(ServerBroadcastThread);

    const IPAddress *broadcast_address_;
    const IPAddress *multicast_address_;
    const IPAddress *connection_address_;

    NetworkSocket *broadcast_socket_;
    NetworkSocket *multicast_socket_;
    bool suspended_;

    Server *server_;

    bool update_broadcast_message_;
    CriticalSection update_broadcast_message_cs_;

};

inline void ServerBroadcastThread::NotifyServerInfoChanged()
{
    update_broadcast_message_cs_.Enter();
    update_broadcast_message_ = true;
    update_broadcast_message_cs_.Leave();
}

#endif // SERVER_BROADCAST_THREAD_H_
