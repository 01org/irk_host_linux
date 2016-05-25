#ifndef MOUSE_SCROLL_EVENT_MESSAGE_H_
#define MOUSE_SCROLL_EVENT_MESSAGE_H_
#include "InputMessage.h"

class MouseScrollEventMessage : public InputMessage
{
public:
    MouseScrollEventMessage();
    virtual ~MouseScrollEventMessage();

    void SendMouseScrollEvent(int input_handler_fd = -1) const;

    float GetX() const;
    float GetY() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(MouseScrollEventMessage);

    float delta_x_;
    float delta_y_;
};

#endif // MOUSE_SCROLL_EVENT_MESSAGE_H_
