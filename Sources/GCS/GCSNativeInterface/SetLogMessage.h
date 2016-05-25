#ifndef SET_LOG_MESSAGE_H_
#define SET_LOG_MESSAGE_H_
#include "OutputMessage.h"

class CommunicationBuffer;

class SetLogMessage : public OutputMessage
{
public:
    SetLogMessage(int debug_level);
    virtual ~SetLogMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(SetLogMessage);

    int debug_level_;
};

#endif // SET_LOG_MESSAGE_H_
