#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "MediaVolumeChangedOutputMessage.h"


MediaVolumeChangedOutputMessage::MediaVolumeChangedOutputMessage(float new_volume_)
    : OutputMessage(OutputMessage::kMediaVolumeChanged)
    , media_volume_level_(new_volume_)
{
    //media_volume_level_ = new_volume_;
}

//virtual 
MediaVolumeChangedOutputMessage::~MediaVolumeChangedOutputMessage()
{
}

//virtual 
int MediaVolumeChangedOutputMessage::GetDataSize() const
{
    return sizeof(float);
}

//virtual 
void MediaVolumeChangedOutputMessage::SetData(CommunicationBuffer *buffer, int data_size) const
{
    UNREFERENCED_PARAMETER(buffer);
    if (data_size != GetDataSize())
    {
        GCSOutputDebugString(L"Wrong Data Size\n");
        return;
    }

    buffer->PushFloat(media_volume_level_);
}
