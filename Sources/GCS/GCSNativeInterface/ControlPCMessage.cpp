#include "StdAfx.h"
#include "ControlPCMessage.h"

ControlPCMessage::ControlPCMessage() 
	: InputMessage(kControlPCMessage)
	, visible_(true)
{

}

ControlPCMessage::~ControlPCMessage()
{

}

void ControlPCMessage::ReadMessage()
{
	visible_ = ReadBool();
}

void ControlPCMessage::SetMouseVisibility() const
{
	
}

