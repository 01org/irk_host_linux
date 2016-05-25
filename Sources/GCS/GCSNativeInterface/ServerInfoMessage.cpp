#include "StdAfx.h"
#include "ServerInfoMessage.h"
#include "CommunicationBuffer.h"
#include "Utilities.h"

ServerInfoMessage::ServerInfoMessage() 
    : OutputMessage(OutputMessage::kServerInfo)
    , windows_8_or_higher_(false)
    , windows_major_version_(0)
    , windows_minor_version_(0)
	, wol_mac_addresses_length_(0)
	, wol_mac_addresses_value_()
{
}

ServerInfoMessage::ServerInfoMessage(GUID server_identity) 
    : OutputMessage(OutputMessage::kServerInfo)
    , windows_8_or_higher_(false)
    , windows_major_version_(0)
    , windows_minor_version_(0)
	, wol_mac_addresses_length_(0)
	, wol_mac_addresses_value_()
	, server_identity_(server_identity)
{
}

ServerInfoMessage::ServerInfoMessage(std::wstring wol_mac_addresses, GUID server_identity) 
    : OutputMessage(OutputMessage::kServerInfo)
    , windows_8_or_higher_(false)
    , windows_major_version_(0)
    , windows_minor_version_(0)
	, wol_mac_addresses_length_((int)wol_mac_addresses.length() * sizeof(wchar_t))
	, wol_mac_addresses_value_(wol_mac_addresses)
	, server_identity_(server_identity)
{
}

// virtual 
ServerInfoMessage::~ServerInfoMessage()
{

}

// virtual 
int ServerInfoMessage::GetDataSize() const
{
     return sizeof(BYTE) + sizeof(windows_major_version_) + sizeof(windows_minor_version_) + sizeof(server_identity_) + sizeof(wol_mac_addresses_length_) + wol_mac_addresses_length_;
}

// virtual 
void ServerInfoMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
	
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushBool(windows_8_or_higher_);
    buffer->PushInt(windows_major_version_);
    buffer->PushInt(windows_minor_version_);

    buffer->PushInt(server_identity_.Data1);
    buffer->PushShort(server_identity_.Data2);
    buffer->PushShort(server_identity_.Data3);
    buffer->PushData(server_identity_.Data4, 8);

    buffer->PushInt(wol_mac_addresses_length_);

    if(wol_mac_addresses_length_ > 0)
            for(unsigned int i = 0; i < wol_mac_addresses_value_.size(); i++)
                    buffer->PushShort(wol_mac_addresses_value_[i]);
}
