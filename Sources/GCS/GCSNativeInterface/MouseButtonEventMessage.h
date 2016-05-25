#ifndef MOUSE_BUTTON_EVENT_MESSAGE_H_
#define MOUSE_BUTTON_EVENT_MESSAGE_H_
#include "ButtonState.h"
#include "InputMessage.h"

class MouseButtonEventMessage : public InputMessage
{
public:
    MouseButtonEventMessage();
    virtual ~MouseButtonEventMessage();

    void SendMouseButtonEvent(int input_handler_fd = -1) const;

    BYTE GetButton() const;
    ButtonState GetState() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(MouseButtonEventMessage);
    
    unsigned short ButtonForLinux() const;

    BYTE virtual_button_;
    ButtonState button_state_;
};

#endif // MOUSE_BUTTON_EVENT_MESSAGE_H_
