#include "StdAfx.h"
#include "IPAddress.h"
#include "NetworkSocket.h"
#include <errno.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCKET_ERROR -1

NetworkSocket::NetworkSocket(Type type)
    : type_(type)
    , socket_(INVALID_SOCKET)
    , is_connected_(false)
{
    switch (type)
    {
        case UDP:
        {
            socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            ASSERT(socket_ != INVALID_SOCKET);
            break;
        }

        case TCP:
        {
            socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            ASSERT(socket_ != INVALID_SOCKET);
            break;
        }

        case MULTICAST:
        {
            socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            ASSERT(socket_ != INVALID_SOCKET);
            break;
        }
    }

    Init();
}

NetworkSocket::NetworkSocket(Type type, SOCKET socket_)
    : type_(type)
    , socket_(socket_)
{
    Init();
}

// virtual
NetworkSocket::~NetworkSocket()
{
    if (socket_ != INVALID_SOCKET)
    {
        close(socket_);
        socket_ = INVALID_SOCKET;
    }
}

bool NetworkSocket::EnableBroadcast()
{
    if(socket_ == INVALID_SOCKET)
    {
        return false;
    }

    BOOL on = TRUE;
    int result = setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&on), sizeof(on));
    if(result != 0)
    {
        int error = errno;
        GCSOutputDebugString(L"Could not set the socket option SO_BROADCAST (%d)\n", error);
        ASSERT(result == 0);
    }

    return (result == 0);
}

bool NetworkSocket::EnableMulticast(const IPAddress &address)
{
    if(socket_ == INVALID_SOCKET)
    {
        return false;
    }

    struct in_addr interface = address.GetIPV4Address()->sin_addr;
    int result = setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_IF, reinterpret_cast<char *>(&interface), sizeof(interface));
    if(result != 0)
    {
        int error = errno;
        GCSOutputDebugString(L"Could not join multicast group (%d)\n", error);
        ASSERT(result == 0);
    }

    return (result == 0);
}

bool NetworkSocket::Bind(const IPAddress *ip_address)
{
    if (ip_address == NULL || socket_ == INVALID_SOCKET)
    {
        return false;
    }

    const sockaddr_in *sock_addr_in = ip_address->GetIPV4Address();
    if (sock_addr_in == NULL)
    {
        return false;
    }

    int result = bind(socket_, reinterpret_cast<const struct sockaddr *>(sock_addr_in), sizeof(sockaddr_in));
    if (result != 0)
    {
        int error = errno;
        GCSOutputDebugString(L"Could not bind socket to address '%s' (error %d)\n", ip_address->AsString().c_str(), error);
        ASSERT(result == 0);
    }

    return (result == 0);
}

bool NetworkSocket::GetAddress(IPAddress *destination) const
{
    if (destination == NULL || socket_ == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in sock_addr_in;
    socklen_t size = sizeof(sock_addr_in);

    int result = getsockname(socket_, reinterpret_cast<sockaddr *>(&sock_addr_in), &size);
    if (result != 0 || size != sizeof(sock_addr_in))
    {
        GCSOutputDebugString(L"Could not read socket address\n");
        return false;
    }

    *destination = IPAddress(&sock_addr_in);
    return true;
}

bool NetworkSocket::Listen(int connection_queue_size)
{
    if (socket_ == INVALID_SOCKET || type_ != TCP)
    {
        return false;
    }

    int result = listen(socket_, connection_queue_size);
    if (result != 0)
    {
        int error = errno;
        GCSOutputDebugString(L"Could not listen on socket\n", error);
        return false;
    }

    return true;
}

bool NetworkSocket::Send(const IPAddress *to_address, const BYTE *bytes, int num_bytes)
{
    if(type_ == TCP)
    {
        return Send(bytes, num_bytes);
    }

    if (to_address == NULL || bytes == NULL || num_bytes == 0 || socket_ == INVALID_SOCKET)
    {
        return false;
    }

    const sockaddr_in *sock_addr_in = to_address->GetIPV4Address();
    if (sock_addr_in == NULL)
    {
        return false;
    }

    int result = sendto(socket_, reinterpret_cast<const char *>(bytes), num_bytes, 0, reinterpret_cast<const struct sockaddr *>(sock_addr_in), sizeof(sockaddr_in));
    if (result != static_cast<int>(num_bytes))
    {
        int error = errno;
        GCSOutputDebugString(L"Could not send data to address '%s' (error %d)\n", to_address->AsString().c_str(), error);
    }

    return (result == static_cast<int>(num_bytes));
}

bool NetworkSocket::Send(const BYTE *bytes, int num_bytes)
{
    if (bytes == NULL || num_bytes == 0 || socket_ == INVALID_SOCKET)
    {
        return false;
    }
    
    int flags = MSG_NOSIGNAL;
    int result = send(socket_, reinterpret_cast<const char *>(bytes), num_bytes, flags);
    if (result != static_cast<int>(num_bytes))
    {
        int error = errno;
        switch(error)
        {
            case EPIPE:
            case ECONNRESET: 
            {
                //Socket has been closed, so clean up the connection
                GCSOutputDebugString(L"Could not send data (socket not connected)\n");
                is_connected_ = false;
                return false;
            }
            default:
            {
                GCSOutputDebugString(L"Could not send data (error %d)\n", error);
            }
        }
    }

    return (result == static_cast<int>(num_bytes));
}

int NetworkSocket::Receive(IPAddress *from_address, BYTE *bytes, int num_bytes)
{
    if (bytes == NULL || num_bytes == 0 || socket_ == INVALID_SOCKET)
    {
        return 0;
    }

    struct sockaddr_in from_sock_addr;
    socklen_t from_sock_addr_size = sizeof(from_sock_addr);

    fd_set read_sockets;
    FD_ZERO(&read_sockets);
    FD_SET(socket_, &read_sockets);

    select(static_cast<int>(socket_)+ 1, &read_sockets, NULL, NULL, NULL); //wait indefinitely for there to be bytes to receive

    int result = recvfrom(socket_, reinterpret_cast<char *>(bytes), num_bytes, 0, reinterpret_cast<struct sockaddr *>(&from_sock_addr), &from_sock_addr_size);
    
    if(result == 0)
    {
        //Socket was ready to receive, but received no bytes. This happens when the socket gets closed, so return an error (-1)
        GCSOutputDebugString(L"Received 0 bytes, so socket was closed\n");
        is_connected_ = false;
        return -1;
    }

    if (result == SOCKET_ERROR)
    {
        int last_error = errno;
        if (last_error != EWOULDBLOCK)
        {
            GCSOutputDebugString(L"Could not receive data (error %d)", last_error);
            return -1;
        }
        
        GCSOutputDebugString(L"NetworkSocker::Receive would block");
        return 0;
    }

    if (from_address != NULL && from_sock_addr_size == sizeof(from_sock_addr))
    {
        *from_address = IPAddress(&from_sock_addr);
    }

    return result;
}

bool NetworkSocket::IsConnected()
{
    return is_connected_;
}

NetworkSocket * NetworkSocket::Accept()
{
    if (socket_ == INVALID_SOCKET || type_ != TCP)
    {
        return NULL;
    }

    struct sockaddr_in from_sock_addr;
    socklen_t from_sock_addr_size = sizeof(from_sock_addr);
    SOCKET result = accept(socket_, reinterpret_cast<struct sockaddr *>(&from_sock_addr), &from_sock_addr_size);
    if (result == INVALID_SOCKET)
    {
        int last_error = errno;
        if (last_error != EWOULDBLOCK)
        {
            GCSOutputDebugString(L"Socket accept call failed (error %d)\n", last_error);
        }

        return NULL;
    }

    NetworkSocket *new_socket = new NetworkSocket(TCP, result);
    return new_socket;
}

void NetworkSocket::Init()
{
    if (socket_ != INVALID_SOCKET)
    {
        // Allow any bound address to be re-used
        BOOL on = TRUE;
        int result = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&on), sizeof(on));
        if (result != 0)
        {
            int error = errno;
            GCSOutputDebugString(L"Could not set the socket option SO_REUSEADDR (%d)\n", error);
            ASSERT(result == 0);
        }

        // Make the socket non-blocking
        u_long nonBlocking = 1;
        result = ioctl(socket_, FIONBIO, &nonBlocking);
        if (result != 0)
        {
            int error = errno;
            GCSOutputDebugString(L"Could not set the socket to non-blocking (%d)\n", error);
            ASSERT(result == 0);
        }

        is_connected_ = true;
    }
}
