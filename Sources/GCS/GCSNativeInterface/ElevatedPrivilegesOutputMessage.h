#ifndef ELEVATED_PRIVILEGES_OUTPUT_MESSAGE_H_
#define ELEVATED_PRIVILEGES_OUTPUT_MESSAGE_H_
#include "../GCSNativeInterface/OutputMessage.h"

class BasicClientDevice;
class CommunicationBuffer;

class ElevatedPrivilegesOutputMessage : public OutputMessage
{
public:
    ElevatedPrivilegesOutputMessage(bool muted);
    virtual ~ElevatedPrivilegesOutputMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ElevatedPrivilegesOutputMessage);

    bool is_elevated_;
};

#endif // ELEVATED_PRIVILEGES_OUTPUT_MESSAGE_H_