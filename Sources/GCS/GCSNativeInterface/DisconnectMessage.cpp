#include "StdAfx.h"
#include "DisconnectMessage.h"

DisconnectMessage::DisconnectMessage()
    : InputMessage(InputMessage::kDisconnectMessage)
{
}

//virtual 
DisconnectMessage::~DisconnectMessage()
{
}

//virtual 
void DisconnectMessage::ReadMessage()
{
}
