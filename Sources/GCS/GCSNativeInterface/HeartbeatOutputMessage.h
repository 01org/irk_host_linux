#ifndef HEARTBEAT_OUTPUT_MESSAGE_H_
#define HEARTBEAT_OUTPUT_MESSAGE_H_
#include "OutputMessage.h"

class CommunicationBuffer;

class HeartbeatOutputMessage : public OutputMessage
{
public:
    HeartbeatOutputMessage();
    virtual ~HeartbeatOutputMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(HeartbeatOutputMessage); 
};

#endif // HEARTBEAT_OUTPUT_MESSAGE_H_