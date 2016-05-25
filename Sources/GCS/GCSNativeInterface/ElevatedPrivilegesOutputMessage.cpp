#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "ElevatedPrivilegesOutputMessage.h"


ElevatedPrivilegesOutputMessage::ElevatedPrivilegesOutputMessage(bool is_elevated)
    : OutputMessage(OutputMessage::kElevatedPrivilegesChanged)
    , is_elevated_(is_elevated)
{
}

//virtual 
ElevatedPrivilegesOutputMessage::~ElevatedPrivilegesOutputMessage()
{
}

//virtual 
int ElevatedPrivilegesOutputMessage::GetDataSize() const
{
    return sizeof(bool);
}

//virtual 
void ElevatedPrivilegesOutputMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushBool(is_elevated_);
}
