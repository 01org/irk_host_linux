#ifndef AUTHENTICATION_FAILED_MESSAGE_H_
#define AUTHENTICATION_FAILED_MESSAGE_H_
#include "OutputMessage.h"

enum AuthenticationFailedReason
{
    AFR_Cancel,
    AFR_DoNotDisturb,
    AFR_AnotherClient
};

class AuthenticationFailedMessage : public OutputMessage
{
public:
    AuthenticationFailedMessage(AuthenticationFailedReason reason);
    virtual ~AuthenticationFailedMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationFailedMessage); 

    AuthenticationFailedReason failure_reason_;
};

#endif // AUTHENTICATION_FAILED_MESSAGE_H_
