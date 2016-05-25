#include "StdAfx.h"
#include "MouseButtonEventMessage.h"
#include <linux/input.h>
#include <unistd.h>

MouseButtonEventMessage::MouseButtonEventMessage()
    : InputMessage(InputMessage::kMouseButtonEventMessage)
    , virtual_button_(0)
    , button_state_(BUTTON_STATE_DOWN)
{
}

//virtual
MouseButtonEventMessage::~MouseButtonEventMessage()
{
}

//virtual
void MouseButtonEventMessage::ReadMessage()
{
    virtual_button_ = ReadByte();
    button_state_ = static_cast<ButtonState>(ReadByte());
}

void MouseButtonEventMessage::SendMouseButtonEvent(int input_handler_fd) const
{
    if(input_handler_fd == -1)
    {
        return;
    }
    
    char type = 'B';
    unsigned short button = ButtonForLinux();
    write(input_handler_fd, &type, sizeof(type));
    write(input_handler_fd, &button, sizeof(button));
    write(input_handler_fd, &button_state_, sizeof(button_state_));
}

BYTE MouseButtonEventMessage::GetButton() const
{
    return virtual_button_;
}

ButtonState MouseButtonEventMessage::GetState() const
{
    return button_state_;
}

unsigned short MouseButtonEventMessage::ButtonForLinux() const
{
    switch(virtual_button_)
    {
        case 0x01:
            return BTN_LEFT;
        case 0x02:
            return BTN_RIGHT;
        case 0x04:
            return BTN_MIDDLE;
        default:
            return KEY_RESERVED;
    }
}
