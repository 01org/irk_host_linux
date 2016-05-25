#include "StdAfx.h"
#include "SuspendGCCMessage.h"

SuspendGCCMessage::SuspendGCCMessage()
    : InputMessage(InputMessage::kSuspendGCCMessage)
{
}

// virtual 
SuspendGCCMessage::~SuspendGCCMessage()
{
}

// virtual 
void SuspendGCCMessage::ReadMessage()
{
}
