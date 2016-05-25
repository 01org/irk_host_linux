#include "StdAfx.h"
#include "ClientConnectionMessageThread.h"
#include "ClientConnection.h"
#include "InputMessageFactory.h"
#include "StreamNetworkBuffer.h"
#include "NetworkSocket.h"
#include <unistd.h>
#include "pevents.h"

namespace
{
    static const int kMaxMessageSize = 1024;
    static const unsigned int THREAD_EXIT_FAILED = 0xFFFF;
    static const unsigned int THREAD_EXIT_SUCCESS = 0;
}

ClientConnectionMessageThread::ClientConnectionMessageThread(ClientConnection *client_connection, InputMessageFactory *factory, NetworkSocket *network_socket)
    : client_connection_(client_connection)
    , input_message_factory_(factory)
    , thread_handle_()
    , thread_should_exit_(false)
    , socket_(network_socket)
    , thread_exit_signal_(false)
{
    pthread_create(&thread_handle_, NULL, &ClientConnectionMessageThread::ReceiveMessagesEntryPoint, this);
}

// virtual
ClientConnectionMessageThread::~ClientConnectionMessageThread()
{
    //TODO: LINUX
    SignalDestroy();
    sleep(1);
    void *retval;
    pthread_join(thread_handle_, &retval);
    unsigned int *status = static_cast<unsigned int *>(retval);
    SAFE_DELETE(status);
}

void ClientConnectionMessageThread::SignalDestroy()
{
    thread_should_exit_ = true;
}

bool ClientConnectionMessageThread::IsDestroyed()
{
    return thread_exit_signal_.Wait(0);
}

// static
void *ClientConnectionMessageThread::ReceiveMessagesEntryPoint(void *context)
{
    ClientConnectionMessageThread *client_connection_message_thread = reinterpret_cast<ClientConnectionMessageThread *>(context);
    if (client_connection_message_thread == NULL)
    {
        GCSOutputDebugString(L"Invalid client connection message thread received in thread entry point");
        unsigned int *failed = new unsigned int;
        *failed = THREAD_EXIT_FAILED;
        pthread_exit(failed);
        return failed;
    }

    client_connection_message_thread->ReceiveMessages();
    unsigned int *success = new unsigned int;
    *success = THREAD_EXIT_SUCCESS;
    client_connection_message_thread->thread_exit_signal_.Set();
    pthread_exit(success);
    return success;
}

void ClientConnectionMessageThread::ReceiveMessages()
{
    if (client_connection_ == NULL)
    {
        GCSOutputDebugString(L"ClientConnection object does not exist in receive thread.  No messages can be processed.\n");
        return;
    }

    StreamNetworkBuffer network_buffer(socket_);

    while (!thread_should_exit_ && client_connection_->IsConnected())
    {
        BYTE type = 0;
        if (network_buffer.ReadByte(type, 1000))
        {
            InputMessage *message = input_message_factory_->CreateInputMessage(static_cast<InputMessage::InputMessageType>(type), &network_buffer);
            if (message != NULL)
            {
                bool exitThread = false;
                client_connection_->ProcessMessage(message, exitThread);
                SAFE_DELETE(message);
            
                if (exitThread)
                {
                    return;
                }
            }
        }
    }
}
