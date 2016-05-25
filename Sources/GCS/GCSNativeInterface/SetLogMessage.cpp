#include "StdAfx.h"
#include "SetLogMessage.h"
#include "CommunicationBuffer.h"

SetLogMessage::SetLogMessage(int debug_level)
    : OutputMessage(OutputMessage::kSetLog)
    , debug_level_(debug_level)
{
}

//virtual 
SetLogMessage::~SetLogMessage()
{
}

//virtual 
int SetLogMessage::GetDataSize() const
{
    return sizeof(int);
}

//virtual 
void SetLogMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushInt(debug_level_);
}
