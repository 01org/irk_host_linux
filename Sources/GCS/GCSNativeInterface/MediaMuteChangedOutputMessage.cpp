#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "MediaMuteChangedOutputMessage.h"


MediaMuteChangedOutputMessage::MediaMuteChangedOutputMessage(bool muted)
    : OutputMessage(OutputMessage::kMediaMuteChanged)
    , media_muted_(muted)
{
}

//virtual 
MediaMuteChangedOutputMessage::~MediaMuteChangedOutputMessage()
{
}

//virtual 
int MediaMuteChangedOutputMessage::GetDataSize() const
{
    return sizeof(bool);
}

//virtual 
void MediaMuteChangedOutputMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushBool(media_muted_);
}
