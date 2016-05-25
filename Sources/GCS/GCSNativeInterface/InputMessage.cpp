#include "StdAfx.h"
#include "InputMessage.h"
#include "NetworkBuffer.h"

namespace
{
    static const int kMessageReadDataTimeoutMilliseconds = 1000;
}

InputMessage::InputMessage(InputMessageType type)
    : message_type_(type)
    , is_valid_(true)
    , network_buffer_(NULL)
{
}

//virtual 
InputMessage::~InputMessage()
{
}

void InputMessage::Init(NetworkBuffer *network_buffer)
{
    // Only store the network buffer for the duration of this function call
    network_buffer_ = network_buffer;
    ReadMessage();
    network_buffer_ = NULL;
}

InputMessage::InputMessageType InputMessage::GetType() const
{
    return message_type_;
}

bool InputMessage::IsValid() const
{
    return is_valid_;
}

int InputMessage::ReadInt(int defaultValue)
{
    int value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadInt(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

unsigned int InputMessage::ReadUnsignedInt(unsigned int defaultValue)
{
    unsigned int value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadUnsignedInt(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

short InputMessage::ReadShort(short defaultValue)
{
    short value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadShort(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

float InputMessage::ReadFloat(float defaultValue)
{
    float value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadFloat(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

char InputMessage::ReadChar(char defaultValue)
{
    char value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadChar(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

wchar_t InputMessage::ReadWChar(wchar_t defaultValue)
{
    wchar_t value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadWChar(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

BYTE InputMessage::ReadByte(BYTE defaultValue)
{
    BYTE value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadByte(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

bool InputMessage::ReadBool(bool defaultValue)
{
    bool value;

    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadBool(value, kMessageReadDataTimeoutMilliseconds))
    {
        value = defaultValue;
        is_valid_ = false;
    }

    return value;
}

void InputMessage::ReadWString(std::wstring &value)
{
    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadWString(value, kMessageReadDataTimeoutMilliseconds))
    {
        is_valid_ = false;
    }
}

void InputMessage::ReadIPAddress(IPAddress &address)
{
    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadIPAddress(address, kMessageReadDataTimeoutMilliseconds))
    {
        is_valid_ = false;
    }
}

void InputMessage::ReadData(BYTE *data_buffer, unsigned int data_size)
{
    if (network_buffer_ == NULL || !is_valid_ || !network_buffer_->ReadData(data_buffer, data_size, kMessageReadDataTimeoutMilliseconds))
    {
        is_valid_ = false;
    }
}
