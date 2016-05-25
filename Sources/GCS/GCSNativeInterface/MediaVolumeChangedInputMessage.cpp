#include "StdAfx.h"
#include "MediaVolumeChangedInputMessage.h"

MediaVolumeChangedInputMessage::MediaVolumeChangedInputMessage()
    : InputMessage(InputMessage::kVolumeChangedMessage)
{
}

//virtual 
MediaVolumeChangedInputMessage::~MediaVolumeChangedInputMessage()
{
}

//virtual 
void MediaVolumeChangedInputMessage::ReadMessage()
{
    volume_level_ = ReadInt(0);
}

int MediaVolumeChangedInputMessage::GetVolumeLevel() const
{
    return volume_level_;
}
