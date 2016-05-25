#include "StdAfx.h"
#include "AuthenticationSuccessMessage.h"
#include "CommunicationBuffer.h"
#include "Utilities.h"

AuthenticationSuccessMessage::AuthenticationSuccessMessage() 
    : OutputMessage(OutputMessage::kAuthenticationSuccess)
{
}

// virtual 
AuthenticationSuccessMessage::~AuthenticationSuccessMessage()
{

}

// virtual 
int AuthenticationSuccessMessage::GetDataSize() const
{
     return 0;
}

// virtual 
void AuthenticationSuccessMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{	
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }
}
