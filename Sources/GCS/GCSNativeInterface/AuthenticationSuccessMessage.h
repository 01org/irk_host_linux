#ifndef AUTHENTICATION_SUCCESS_MESSAGE_H_
#define AUTHENTICATION_SUCCESS_MESSAGE_H_
#include "OutputMessage.h"

class AuthenticationSuccessMessage : public OutputMessage
{
public:
    AuthenticationSuccessMessage();
    virtual ~AuthenticationSuccessMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationSuccessMessage); 
};

#endif // AUTHENTICATION_SUCCESS_MESSAGE_H_
