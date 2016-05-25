#ifndef CLIENT_CONNECTION_MESSAGE_THREAD_H_
#define CLIENT_CONNECTION_MESSAGE_THREAD_H_

#include "SyncEvent.h"

typedef pthread_t HANDLE;

class ClientConnection;
class InputMessage;
class InputMessageFactory;
class NetworkSocket;

class ClientConnectionMessageThread
{
public:
    ClientConnectionMessageThread(ClientConnection *client_connection, InputMessageFactory *factory, NetworkSocket *socket);
    virtual ~ClientConnectionMessageThread();
    void SignalDestroy();
    bool IsDestroyed();
    
private:
    DISALLOW_COPY_AND_ASSIGN(ClientConnectionMessageThread);
    
    static void *ReceiveMessagesEntryPoint(void *context);
    void ReceiveMessages();
    
    ClientConnection * const client_connection_;
    HANDLE thread_handle_;
    InputMessageFactory * const input_message_factory_;

    volatile bool thread_should_exit_;
    NetworkSocket *socket_;
    
    SyncEvent thread_exit_signal_;
};

#endif // CLIENT_CONNECTION_MESSAGE_THREAD_H_
