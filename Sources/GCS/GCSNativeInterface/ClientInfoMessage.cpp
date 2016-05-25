#include "StdAfx.h"
#include "ClientInfoMessage.h"
#include "ClientDevice.h"
#include <string>

ClientInfoMessage::ClientInfoMessage()
    : InputMessage(InputMessage::kClientInfoMessage)
    , client_info_()
{
}

//virtual 
ClientInfoMessage::~ClientInfoMessage()
{
}

//virtual 
void ClientInfoMessage::ReadMessage()
{
    int number_of_pairs = ReadInt();

    std::wstring key;
    std::wstring value;

    for(int i = 0; i < number_of_pairs; ++i)
    {
        ReadWString(key);
        ReadWString(value);
        client_info_.insert(std::pair<std::wstring, std::wstring>(key, value));
    }
}

const std::map<std::wstring, std::wstring> &ClientInfoMessage::GetClientInfo() const
{
    return client_info_;
}

ClientDevice *ClientInfoMessage::GetClientDeviceFromInfo() const
{
    ClientDevice *new_device = new ClientDevice();
    
    typedef std::map<std::wstring, std::wstring>::const_iterator it_type;
    for(it_type iterator = client_info_.begin(); iterator != client_info_.end(); iterator++) 
    {
        new_device->AddProperty(iterator->first.c_str(), iterator->second.c_str());
    }

    return new_device;
}
