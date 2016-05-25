#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "IPAddress.h"
#include "Server.h"
#include "ServerBroadcastMessage.h"
#include "Version.h"

#include <string.h>

ServerBroadcastMessage::ServerBroadcastMessage(const IPAddress *ipAddress, Server *server) 
    : OutputMessage(OutputMessage::kServerBroadcast)
    , ip_address_(NULL)
    , server_(server)
{
    if (ipAddress != NULL)
    {
        ip_address_ = new IPAddress(*ipAddress);
    }
}

//virtual 
ServerBroadcastMessage::~ServerBroadcastMessage()
{
    SAFE_DELETE(ip_address_);
}

//virtual
int ServerBroadcastMessage::GetDataSize() const
{
    //wchar_t is 32bits on Linux, but the broadcast message has 16 bits per character
    return sizeof(int) + sizeof(GUID) + sizeof(BYTE) * IPAddress::kIPAddressNumBytes + sizeof(unsigned short) + sizeof(BYTE) + sizeof(int) + server_->GetNameLength() * sizeof(short);
}

//virtual
void ServerBroadcastMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    if (ip_address_ == NULL)
    {
        GCSOutputDebugString(L"Invalid IP address in ServerBroadcastMessage\n");
        return;
    }

    const struct sockaddr_in *ipv4_address_ = ip_address_->GetIPV4Address();
    if (ipv4_address_ == NULL)
    {
        GCSOutputDebugString(L"Invalid IPv4 address in ServerBroadcastMessage\n");
        return;
    }

    buffer->PushInt(kVersionNumber);
    buffer->PushGUID(server_->GetServerIdentity());
    BYTE address[4];
    MemCopy(address, sizeof(address), &ipv4_address_->sin_addr.s_addr, sizeof(address));
    buffer->PushByte(address[0]);
    buffer->PushByte(address[1]);
    buffer->PushByte(address[2]);
    buffer->PushByte(address[3]);
    buffer->PushShort(ntohs(ipv4_address_->sin_port));

    //Server information
    //Type (BYTE)
    //NameByteLength (int)
    //Name(wchars)
    buffer->PushByte((BYTE)server_->GetType());

    std::wstring name;
    server_->GetName(name);

    buffer->PushString(name);
}
