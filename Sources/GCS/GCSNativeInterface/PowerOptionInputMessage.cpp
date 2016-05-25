#include "StdAfx.h"
#include "PowerOptionInputMessage.h"

PowerOptionInputMessage::PowerOptionInputMessage()
    : InputMessage(InputMessage::kPowerOptionMessage)
    , power_option_(0xff)
{
}

//virtual 
PowerOptionInputMessage::~PowerOptionInputMessage()
{
}

//virtual 
void PowerOptionInputMessage::ReadMessage()
{
    power_option_ = ReadByte();
}

BYTE PowerOptionInputMessage::GetOption() const
{
    return power_option_;
}

void PowerOptionInputMessage::HandlePowerOption(bool isAtLogon) const
{
} 