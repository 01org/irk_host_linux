#include "StdAfx.h"
#include "HeartbeatOutputMessage.h"

HeartbeatOutputMessage::HeartbeatOutputMessage() 
    : OutputMessage(OutputMessage::kHeartbeat)
{
}

//virtual 
HeartbeatOutputMessage::~HeartbeatOutputMessage()
{
}

//virtual 
int HeartbeatOutputMessage::GetDataSize() const
{
    return 0;
}

//virtual 
void HeartbeatOutputMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    UNREFERENCED_PARAMETER(buffer);
}
