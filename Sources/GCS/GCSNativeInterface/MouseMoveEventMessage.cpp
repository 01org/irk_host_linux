#include "StdAfx.h"
#include "MouseMoveEventMessage.h"
#include <unistd.h>

MouseMoveEventMessage::MouseMoveEventMessage() 
    : InputMessage(InputMessage::kMouseMoveEventMessage)
    , relative_(true)
    , x_(0.0f)
    , y_(0.0f)
{    
}

//virtual 
MouseMoveEventMessage::~MouseMoveEventMessage()
{
}

//virtual 
void MouseMoveEventMessage::ReadMessage()
{
    relative_ = ReadBool();
    x_ = ReadFloat();
    y_ = ReadFloat();
}

void MouseMoveEventMessage::SendMouseMoveEvent(int input_handler_fd) const
{
    if(input_handler_fd == -1)
    {
        return;
    }
    
    char type = 'M';
    
    write(input_handler_fd, &type, sizeof(type));
    write(input_handler_fd, &relative_, sizeof(relative_));
    write(input_handler_fd, &x_, sizeof(x_));
    write(input_handler_fd, &y_, sizeof(y_));
}

bool MouseMoveEventMessage::IsRelative() const
{
    return relative_;
}

float MouseMoveEventMessage::GetX() const
{
    return x_;
}

float MouseMoveEventMessage::GetY() const
{
    return y_;
}
