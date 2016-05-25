#ifndef MOUSE_MOVE_EVENT_MESSAGE_H_
#define MOUSE_MOVE_EVENT_MESSAGE_H_
#include "InputMessage.h"

class MouseMoveEventMessage : public InputMessage
{
public:
    MouseMoveEventMessage();
    virtual ~MouseMoveEventMessage();

    void SendMouseMoveEvent(int input_handler_fd = -1) const;

    bool IsRelative() const;
    float GetX() const;
    float GetY() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(MouseMoveEventMessage);

    bool relative_;
    float x_;
    float y_;
};

#endif // MOUSE_MOVE_EVENT_MESSAGE_H_
