#include "StdAfx.h"
#include "ServerDisconnectMessage.h"

ServerDisconnectMessage::ServerDisconnectMessage() 
    : OutputMessage(OutputMessage::kServerDisconnect)
{
}

//virtual 
ServerDisconnectMessage::~ServerDisconnectMessage()
{
}

//virtual 
int ServerDisconnectMessage::GetDataSize() const
{
    return 0;
}

//virtual 
void ServerDisconnectMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    UNREFERENCED_PARAMETER(buffer);
}
