#ifndef MEDIA_VOLUME_CHANGED_OUTPUT_MESSAGE_H_
#define MEDIA_VOLUME_CHANGED_OUTPUT_MESSAGE_H_
#include "../GCSNativeInterface/OutputMessage.h"

class BasicClientDevice;
class CommunicationBuffer;

class MediaVolumeChangedOutputMessage : public OutputMessage
{
public:
    MediaVolumeChangedOutputMessage(float new_volume_);
    virtual ~MediaVolumeChangedOutputMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(MediaVolumeChangedOutputMessage);

    float media_volume_level_;
};

#endif // MEDIA_VOLUME_CHANGED_OUTPUT_MESSAGE_H_