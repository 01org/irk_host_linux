#ifndef MEDIA_MUTE_CHANGED_OUTPUT_MESSAGE_H_
#define MEDIA_MUTE_CHANGED_OUTPUT_MESSAGE_H_
#include "../GCSNativeInterface/OutputMessage.h"

class BasicClientDevice;
class CommunicationBuffer;

class MediaMuteChangedOutputMessage : public OutputMessage
{
public:
    MediaMuteChangedOutputMessage(bool muted);
    virtual ~MediaMuteChangedOutputMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(MediaMuteChangedOutputMessage);

    bool media_muted_;
};

#endif // MEDIA_MUTE_CHANGED_OUTPUT_MESSAGE_H_