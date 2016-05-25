#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#define __stdcall 

#include <string>

class InputMessage;

class MessageHandler
{
public:
        virtual void ProcessMessage(InputMessage *input_message) = 0;
};

#endif // MESSAGE_HANDLER_H_
