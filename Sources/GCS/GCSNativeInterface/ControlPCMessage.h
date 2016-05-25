#ifndef CONTROL_PC_MESSAGE_H_
#define CONTROL_PC_MESSAGE_H_

#include "InputMessage.h"

class ControlPCMessage : public InputMessage
{
public:
    ControlPCMessage();
    virtual ~ControlPCMessage();

    void SetMouseVisibility() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(ControlPCMessage);

    bool visible_;
};

#endif // CONTROL_PC_MESSAGE_H_