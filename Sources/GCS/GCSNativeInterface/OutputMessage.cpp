#include "StdAfx.h"
#include "OutputMessage.h"
#include "CommunicationBuffer.h"

OutputMessage::OutputMessage(OutputMessageType type)
    : buffer_(NULL)
{
    message_type_ = type;
}

//virtual 
OutputMessage::~OutputMessage()
{
    SAFE_DELETE(buffer_);
}

const CommunicationBuffer *OutputMessage::GetCommunicationBuffer()
{
    int data_size = GetDataSize();

    int buffer_size = sizeof(char) + data_size;

    SAFE_DELETE_ARRAY(buffer_);
    buffer_ = new CommunicationBuffer(buffer_size);

    // Set Message Type
    buffer_->PushChar(static_cast<char>(message_type_));

    // Set Data
    SetData(buffer_, data_size);

    return buffer_;
}

const OutputMessage::OutputMessageType &OutputMessage::GetType() const
{
    return message_type_;
}
