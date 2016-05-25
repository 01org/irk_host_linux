#include "StdAfx.h"
#include "AuthenticationFailedMessage.h"
#include "CommunicationBuffer.h"
#include "Utilities.h"

AuthenticationFailedMessage::AuthenticationFailedMessage(AuthenticationFailedReason reason) 
    : OutputMessage(OutputMessage::kAuthenticationFailed)
{
    failure_reason_ = reason;
}

// virtual 
AuthenticationFailedMessage::~AuthenticationFailedMessage()
{

}

// virtual 
int AuthenticationFailedMessage::GetDataSize() const
{
     return sizeof(AuthenticationFailedReason);
}

// virtual 
void AuthenticationFailedMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{	
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushInt((int)failure_reason_);
}
