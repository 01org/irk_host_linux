#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "IPAddress.h"
#include "NetworkSocket.h"
#include "ServerBroadcastMessage.h"
#include "ServerBroadcastThread.h"
#include <unistd.h>

namespace
{
    static const int kBroadcastInterval = 1; // In seconds
    static const struct in_addr kMulticastAddress = {0x010101e1}; //Broadcast on 225.1.1.1, but in_addr is just an unsigned long on Linux, and is stored in reverse byte order
    static const unsigned short kMulticastPort = 54545;

}

ServerBroadcastThread::ServerBroadcastThread(const IPAddress *broadcast_address, const IPAddress *connection_address, Server *server)
    : Thread(DEFAULT_STACK_SIZE)
    , broadcast_address_(NULL)
    , connection_address_(NULL)
    , broadcast_socket_(NULL)
    , multicast_socket_(NULL)
    , suspended_(true)
    , server_(server)
    , update_broadcast_message_(false)
    , update_broadcast_message_cs_()
{
    broadcast_socket_ = new NetworkSocket(NetworkSocket::UDP);
    broadcast_socket_->EnableBroadcast();

    multicast_socket_ = new NetworkSocket(NetworkSocket::MULTICAST);
    multicast_socket_->EnableMulticast(*broadcast_address);

    if (broadcast_address != NULL)
    {
        broadcast_address_ = new IPAddress(*broadcast_address);
    }

    if(connection_address != NULL)
    {
        connection_address_ = new IPAddress(*connection_address);
        struct sockaddr_in multicast = {AF_INET, htons(kMulticastPort), kMulticastAddress, {0, 0, 0, 0, 0, 0, 0, 0}};
        multicast_address_ = new IPAddress(&multicast);
    }
}

// virtual
ServerBroadcastThread::~ServerBroadcastThread()
{
    SAFE_DELETE(multicast_socket_);
    SAFE_DELETE(broadcast_socket_);
    SAFE_DELETE(multicast_address_);
    SAFE_DELETE(broadcast_address_);
    SAFE_DELETE(connection_address_);
}

void ServerBroadcastThread::Suspend()
{
    suspended_ = true;
}

void ServerBroadcastThread::Resume()
{
    suspended_ = false;
}

// virtual
int ServerBroadcastThread::Execute()
{
    if (broadcast_address_ == NULL || connection_address_ == NULL)
    {
        return -1;
    }

    ServerBroadcastMessage *message = new ServerBroadcastMessage(connection_address_, server_);
    const CommunicationBuffer *message_buffer = message->GetCommunicationBuffer();
    if (message_buffer == NULL)
    {
        SAFE_DELETE(message);
        return -2;
    }

    while (!IsExitRequested())
    {
        sleep(kBroadcastInterval);

        update_broadcast_message_cs_.Enter();
        bool need_new_message = update_broadcast_message_;
        update_broadcast_message_ = false;
        update_broadcast_message_cs_.Leave();

        if(need_new_message)
        {
            message_buffer = NULL;
            SAFE_DELETE(message);
            message = new ServerBroadcastMessage(connection_address_, server_);
            message_buffer = message->GetCommunicationBuffer();
        }

        if (!suspended_)
        {
            broadcast_socket_->Send(broadcast_address_, message_buffer->GetBuffer(), message_buffer->GetSize());
            multicast_socket_->Send(multicast_address_, message_buffer->GetBuffer(), message_buffer->GetSize());
        }
    }

    SAFE_DELETE(message);

    return 0;
}
