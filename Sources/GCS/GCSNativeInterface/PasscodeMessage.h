#ifndef PASSCODE_MESSAGE_H_
#define PASSCODE_MESSAGE_H_
#include "InputMessage.h"

class ClientDevice;

class PasscodeMessage : public InputMessage
{    
public:
    PasscodeMessage();
    virtual ~PasscodeMessage();

    const std::wstring &GetPasscode() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(PasscodeMessage);

    std::wstring passcode_;
};

#endif // PASSCODE_MESSAGE_H_
