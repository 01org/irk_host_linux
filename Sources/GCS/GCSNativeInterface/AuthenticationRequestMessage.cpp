#include "StdAfx.h"
#include "AuthenticationRequestMessage.h"
#include "CommunicationBuffer.h"
#include "Utilities.h"

AuthenticationRequestMessage::AuthenticationRequestMessage() 
    : OutputMessage(OutputMessage::kAuthenticationRequest)
{
}

// virtual 
AuthenticationRequestMessage::~AuthenticationRequestMessage()
{

}

// virtual 
int AuthenticationRequestMessage::GetDataSize() const
{
     return 0;
}

// virtual 
void AuthenticationRequestMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{	
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }
}
