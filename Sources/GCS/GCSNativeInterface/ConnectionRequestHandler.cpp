#include "StdAfx.h"
#include "ConnectionRequestHandler.h"
#include "InputMessage.h"
#include "InputMessageFactory.h"
#include "IPAddress.h"
#include "NetworkSocket.h"
#include "Server.h"
#include "ServerBroadcastThread.h"
#include "SimpleNetworkBuffer.h"

namespace
{
    static const int kMaxMessageSize = 1024;
    static const int kConnectionQueueSize = 16;
    static const unsigned short kServerBroadcastPort = 54545;
}

ConnectionRequestHandler::ConnectionRequestHandler(Server *server, const IPAddress *local_address)
    : server_(server)
    , local_address_(NULL)
    , tcp_accept_socket_(NULL)
    , broadcast_thread_(NULL)
    , buffer_(NULL)
    , buffer_size_(kMaxMessageSize)
{
    ASSERT(server != NULL);
    ASSERT(local_address != NULL);

    tcp_accept_socket_ = new NetworkSocket(NetworkSocket::TCP);
    tcp_accept_socket_->Bind(local_address);

    local_address_ = new IPAddress();
    tcp_accept_socket_->GetAddress(local_address_);
    tcp_accept_socket_->Listen(kConnectionQueueSize);
    
    BYTE broadcast_ip[4];
    local_address->GetBroadcastAddress(broadcast_ip, 4);
    IPAddress broadcast_address(broadcast_ip, kServerBroadcastPort);

    broadcast_thread_ = new ServerBroadcastThread(&broadcast_address, local_address_, server_);
    broadcast_thread_->Run();

    buffer_ = new BYTE[buffer_size_];
}

// virtual
ConnectionRequestHandler::~ConnectionRequestHandler()
{
    if (broadcast_thread_ != NULL)
    {
        broadcast_thread_->RequestExit();
    }

    SAFE_DELETE_ARRAY(buffer_);
    SAFE_DELETE(tcp_accept_socket_);
    SAFE_DELETE(local_address_);

    if (broadcast_thread_ != NULL)
    {
        broadcast_thread_->Join();
        SAFE_DELETE(broadcast_thread_);
    }
}

void ConnectionRequestHandler::Update()
{
    if (tcp_accept_socket_ != NULL)
    {
        // Accept any incoming connections
        NetworkSocket *new_socket = tcp_accept_socket_->Accept();
        if (new_socket != NULL)
        {
            server_->SocketClientAdded(new_socket);
        }
    }
}

void ConnectionRequestHandler::Suspend()
{
    if (broadcast_thread_ != NULL)
    {
        broadcast_thread_->Suspend();
    }
}

void ConnectionRequestHandler::Resume()
{        
    if (broadcast_thread_ != NULL)
    {
        broadcast_thread_->Resume();
    }
}

void ConnectionRequestHandler::NotifyServerInfoChanged()
{
    broadcast_thread_->NotifyServerInfoChanged();
}

bool ConnectionRequestHandler::HasAddress(const IPAddress *address) const
{       
    if(address == NULL || local_address_ == NULL)
    {
        return false;
    }

    //ignore the port
    return address->AsString().substr(0, address->AsString().find(L':')) == local_address_->AsString().substr(0, local_address_->AsString().find(L':'));
}
