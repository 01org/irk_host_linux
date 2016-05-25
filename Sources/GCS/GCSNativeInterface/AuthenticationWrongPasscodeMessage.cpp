#include "StdAfx.h"
#include "AuthenticationWrongPasscodeMessage.h"
#include "CommunicationBuffer.h"
#include "Utilities.h"

AuthenticationWrongPasscodeMessage::AuthenticationWrongPasscodeMessage() 
    : OutputMessage(OutputMessage::kAuthenticationWrongPassword)
{
}

// virtual 
AuthenticationWrongPasscodeMessage::~AuthenticationWrongPasscodeMessage()
{

}

// virtual 
int AuthenticationWrongPasscodeMessage::GetDataSize() const
{
     return 0;
}

// virtual 
void AuthenticationWrongPasscodeMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{	
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }
}
