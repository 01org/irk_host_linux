#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "LogonInfoOutputMessage.h"


LogonInfoOutputMessage::LogonInfoOutputMessage(bool is_at_logon)
    : OutputMessage(OutputMessage::kLogonInfo)
    , is_at_logon_(is_at_logon)
{
}

//virtual 
LogonInfoOutputMessage::~LogonInfoOutputMessage()
{
}

//virtual 
int LogonInfoOutputMessage::GetDataSize() const
{
    return sizeof(bool);
}

//virtual 
void LogonInfoOutputMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushBool(is_at_logon_);
}
