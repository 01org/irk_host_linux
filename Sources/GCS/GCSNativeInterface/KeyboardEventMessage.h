#ifndef KEYBOARD_EVENT_MESSAGE_H_
#define KEYBOARD_EVENT_MESSAGE_H_
#include "ButtonState.h"
#include "InputMessage.h"

class KeyboardEventMessage : public InputMessage
{
public:
    KeyboardEventMessage();
    virtual ~KeyboardEventMessage();

    void SendKeyboardEvent(int input_handler_fd = -1) const;

    unsigned short GetKey() const;
    unsigned short GetData() const;
    ButtonState GetState() const;
    
    static unsigned int KeycodeFromVirtualKey(unsigned short virtual_key); 

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(KeyboardEventMessage);
    
    unsigned short virtual_key_;
    unsigned short data_;
    ButtonState button_state_;
};

#endif // KEYBOARD_EVENT_MESSAGE_H_
