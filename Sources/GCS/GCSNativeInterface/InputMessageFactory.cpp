#include "StdAfx.h"
#include "InputMessageFactory.h"

InputMessageFactory::InputMessageFactory()
    : message_builder_map_()
{
}

InputMessageFactory::~InputMessageFactory()
{
}

void InputMessageFactory::SetMessageBuilder(InputMessage::InputMessageType type, InputMessageBuilder builder)
{
    if (builder != NULL)
    {
        message_builder_map_[type] = builder;
    }
}

InputMessage * InputMessageFactory::CreateInputMessage(InputMessage::InputMessageType type, NetworkBuffer *network_buffer) const
{
    MessageBuilderMap::const_iterator iter = message_builder_map_.find(type);

    if (iter == message_builder_map_.end())
    {
        GCSOutputDebugString(L"Unknown message type (%d) received in InputMessageFactory::CreateInputMessage\n", type);
        return NULL;
    }

    InputMessage *message = iter->second(network_buffer);
    return message;
}
