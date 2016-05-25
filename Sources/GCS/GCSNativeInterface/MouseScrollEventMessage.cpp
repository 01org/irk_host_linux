#include "StdAfx.h"
#include "MouseScrollEventMessage.h"
#include <linux/input.h>
#include <unistd.h>

MouseScrollEventMessage::MouseScrollEventMessage() 
    : InputMessage(InputMessage::kMouseScrollEventMessage)
    , delta_x_(0.0f)
    , delta_y_(0.0f)
{
}

//virtual 
MouseScrollEventMessage::~MouseScrollEventMessage()
{
}

//virtual 
void MouseScrollEventMessage::ReadMessage()
{
    delta_x_ = ReadFloat();
    delta_y_ = ReadFloat();
}

void MouseScrollEventMessage::SendMouseScrollEvent(int input_handler_fd) const
{
    if(input_handler_fd == -1)
    {
        return;
    }
    
    char type = 'S';
    
    write(input_handler_fd, &type, sizeof(type));
    write(input_handler_fd, &delta_x_, sizeof(delta_x_));
    write(input_handler_fd, &delta_y_, sizeof(delta_y_));
}

float MouseScrollEventMessage::GetX() const
{
    return delta_x_;
}

float MouseScrollEventMessage::GetY() const
{
    return delta_y_;
}
