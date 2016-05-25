#ifndef AUTHENTICATION_WRONG_PASSCODE_MESSAGE_H_
#define AUTHENTICATION_WRONG_PASSCODE_MESSAGE_H_
#include "OutputMessage.h"

class AuthenticationWrongPasscodeMessage : public OutputMessage
{
public:
    AuthenticationWrongPasscodeMessage();
    virtual ~AuthenticationWrongPasscodeMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationWrongPasscodeMessage); 
};

#endif // AUTHENTICATION_WRONG_PASSCODE_MESSAGE_H_
