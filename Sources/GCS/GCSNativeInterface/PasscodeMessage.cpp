#include "StdAfx.h"
#include "PasscodeMessage.h"

PasscodeMessage::PasscodeMessage()
    : InputMessage(InputMessage::kPasscodeMessage)
{
}

//virtual 
PasscodeMessage::~PasscodeMessage()
{
}

//virtual 
void PasscodeMessage::ReadMessage()
{
    ReadWString(passcode_);
}

const std::wstring &PasscodeMessage::GetPasscode() const
{
    return passcode_;
}
