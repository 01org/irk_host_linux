#ifndef INPUT_MESSAGE_FACTORY_H_
#define INPUT_MESSAGE_FACTORY_H_

#include "InputMessage.h"
#include "InputMessageBuilder.h"

class NetworkBuffer;

class InputMessageFactory
{
public:
    InputMessageFactory();
    ~InputMessageFactory();

    void SetMessageBuilder(InputMessage::InputMessageType type, InputMessageBuilder builder);

    InputMessage *CreateInputMessage(InputMessage::InputMessageType type, NetworkBuffer *network_buffer) const;

private:
    DISALLOW_COPY_AND_ASSIGN(InputMessageFactory);

    typedef std::map<InputMessage::InputMessageType, InputMessageBuilder> MessageBuilderMap;
    MessageBuilderMap message_builder_map_;
};

#endif //INPUT_MESSAGE_FACTORY_H_
