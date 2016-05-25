#include "StdAfx.h"
#include "DockInfoMessage.h"
#include "IPAddress.h"

DockInfoMessage::DockInfoMessage()
    : InputMessage(InputMessage::kDockInfoMessage)
    , dock_info_()
{
}

//virtual 
DockInfoMessage::~DockInfoMessage()
{
}

//virtual 
void DockInfoMessage::ReadMessage()
{
    dock_info_.type_ = static_cast<ClientDockType>(ReadInt());
    dock_info_.screen_width_ = ReadInt();
    dock_info_.screen_height_ = ReadInt();
}

const ClientDockInfo &DockInfoMessage::GetDockInfo() const
{
    return dock_info_;
}
