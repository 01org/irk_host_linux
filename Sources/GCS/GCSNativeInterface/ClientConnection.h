#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include "ClientDockInfo.h"

class ClientConnectionMessageThread;
class InputMessage;
class NetworkSocket;
class OutputMessage;
class Server;

class ClientConnection
{
public:
    ClientConnection(Server *server, NetworkSocket *socket);
    virtual ~ClientConnection();

    bool SendMessage(OutputMessage *message);
    void ProcessMessage(InputMessage *message, bool &exitThread);

    void OnAuthorizationRequired();
    void OnConnectionComplete();
    void OnConnectionError(int error);

    void Disconnect(bool send_message);
    bool IsConnecting() const;
    bool IsConnected() const;
    bool IsDisconnected() const;
    void Suspend();
    bool IsSuspended() const;
    bool IsActive() const;

    void Update(float seconds);

    const ClientDockInfo &GetDockInfo() const;
    void SetDockInfo(const ClientDockInfo &info);

    time_t GetConnectionTime() const;

private:
    DISALLOW_COPY_AND_ASSIGN(ClientConnection);
    
    enum State
    {
        PendingConnection,
        ConnectionComplete,
        PendingMessageThreadDestroy,
        CloseComplete
    };
    
    void SendHeartbeat();
    void ReceiveHeartbeat();
    void DestroyConnection();
    void UpdateHeartbeat(float seconds);
    int DataStreamWrite(const BYTE *buffer, int offset, int length);
    
    Server *server_;
    ClientConnectionMessageThread *current_message_thread_;
    State state_;
    bool suspended_;
    float time_since_last_heartbeat_send_;
    float time_since_last_heartbeat_receive_;
    NetworkSocket *socket_;
    ClientDockInfo current_dock_info;
    time_t connection_time_;
};

inline const ClientDockInfo &ClientConnection::GetDockInfo() const
{
    return current_dock_info;
}

#endif // CLIENT_CONNECTION_H_
