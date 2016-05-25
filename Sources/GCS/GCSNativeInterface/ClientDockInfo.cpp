#include "StdAfx.h"
#include "ClientDockInfo.h"

_ClientDockInfo::_ClientDockInfo()
    : type_(ClientDock_NoDock)
    , screen_width_(0)
    , screen_height_(0)
{
}

void _ClientDockInfo::Clear()
{
    type_ = ClientDock_NoDock;
    screen_width_ = 0;
    screen_height_ = 0;
}

