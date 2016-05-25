#ifndef MEDIA_VOLUME_CHANGED_INPUT_MESSAGE_H_
#define MEDIA_VOLUME_CHANGED_INPUT_MESSAGE_H_
#include "InputMessage.h"

class ClientDevice;

class MediaVolumeChangedInputMessage : public InputMessage
{    
public:
    MediaVolumeChangedInputMessage();
    virtual ~MediaVolumeChangedInputMessage();

    int GetVolumeLevel() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(MediaVolumeChangedInputMessage);

    int volume_level_;
};

#endif // MEDIA_VOLUME_CHANGED_INPUT_MESSAGE_H_
