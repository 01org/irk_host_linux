#ifndef LOGON_INFO_OUTPUT_MESSAGE_H_
#define LOGON_INFO_OUTPUT_MESSAGE_H_
#include "../GCSNativeInterface/OutputMessage.h"

class BasicClientDevice;
class CommunicationBuffer;

class LogonInfoOutputMessage : public OutputMessage
{
public:
    LogonInfoOutputMessage(bool is_at_logon);
    virtual ~LogonInfoOutputMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(LogonInfoOutputMessage);

    bool is_at_logon_;
};

#endif // LOGON_INFO_OUTPUT_MESSAGE_H_