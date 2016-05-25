#ifndef INPUT_MESSAGE_BUILDER_H_
#define INPUT_MESSAGE_BUILDER_H_

#include "CommunicationBuffer.h"

class InputMessage;

// Function signature for message builders (creates an InputMessage object from the data received)
typedef InputMessage* (*InputMessageBuilder) (NetworkBuffer *network_buffer);


// Templatized helper function that can be used to build messages (since most messages are built similarly)
template<typename T>
InputMessage * TemplateMessageBuilder(NetworkBuffer *network_buffer)
{
    // Create a new object of the specified type
    T *message = new T();

    // Read the message data (virtual function on all base InputMessage objects)
    message->Init(network_buffer);

    return message;
}

#endif //INPUT_MESSAGE_BUILDER_H_
