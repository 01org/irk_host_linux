#include "StdAfx.h"
#include "HeartbeatInputMessage.h"

HeartbeatInputMessage::HeartbeatInputMessage()
    : InputMessage(InputMessage::kHeartbeatMessage)
{
}

// virtual 
HeartbeatInputMessage::~HeartbeatInputMessage()
{
}

// virtual 
void HeartbeatInputMessage::ReadMessage()
{
}
