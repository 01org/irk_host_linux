#ifndef AUTHENTICATION_REQUEST_MESSAGE_H_
#define AUTHENTICATION_REQUEST_MESSAGE_H_
#include "OutputMessage.h"

class AuthenticationRequestMessage : public OutputMessage
{
public:
    AuthenticationRequestMessage();
    virtual ~AuthenticationRequestMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationRequestMessage); 
};

#endif // AUTHENTICATION_REQUEST_MESSAGE_H_
