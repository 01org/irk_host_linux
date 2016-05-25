#include "StdAfx.h"
#include "CommunicationBuffer.h"
#include "IPAddress.h"
#include <string.h>

CommunicationBuffer::CommunicationBuffer(unsigned int size)
    : size_(size)
    , buffer_(NULL)
    , current_data_pointer_(NULL)
{    
    if (size == 0)
    {
        return;
    }

    buffer_ = new BYTE[size_];
    current_data_pointer_ = buffer_;
}

CommunicationBuffer::~CommunicationBuffer()
{
    SAFE_DELETE_ARRAY(buffer_);
}

const BYTE *CommunicationBuffer::GetBuffer() const
{
    return buffer_;
}

unsigned int CommunicationBuffer::GetSize() const
{
    return size_;
}

void CommunicationBuffer::PushInt(int value)
{
    int net_value = htonl(value);
    unsigned int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(net_value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushInt (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&net_value), sizeof(net_value));
    current_data_pointer_ += sizeof(net_value);
}

void CommunicationBuffer::PushUnsignedInt(unsigned int value)
{
    unsigned int net_value = htonl(value);
    unsigned int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(net_value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushInt (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&net_value), sizeof(net_value));
    current_data_pointer_ += sizeof(net_value);
}

void CommunicationBuffer::PushShort(short value)
{
    short net_value = htons(value);
    unsigned int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(net_value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushShort (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&net_value), sizeof(net_value));
    current_data_pointer_ += sizeof(net_value);
}

void CommunicationBuffer::PushFloat(float value)
{  
    int int_value = *reinterpret_cast<int *>(&value);

    int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(int_value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushFloat (Buffer too small)\n");
        return;
    }
    
    int net_value = htonl(int_value);
    MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&net_value), sizeof(net_value));
    current_data_pointer_ += sizeof(net_value);
}

void CommunicationBuffer::PushChar(char value)
{
    int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushChar (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&value), sizeof(value));
    current_data_pointer_ += sizeof(value);
}

void CommunicationBuffer::PushByte(BYTE value)
{
    int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (sizeof(value) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushChar (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, &value, sizeof(value));
    current_data_pointer_ += sizeof(value);
}

void CommunicationBuffer::PushBool(bool value)
{
    PushByte(value ? 1 : 0);
}

void CommunicationBuffer::PushString(const std::wstring &value)
{
    unsigned int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);

    //wchar_t is 32bits on linux, but we use 16 bit characters for all network communication, so sending a string has to be handled specially
    if (sizeof(unsigned int) + value.length() * sizeof(short) > remaining)
    {
        GCSOutputDebugString(L"Unable to PushString (Buffer too small)\n");
        return;
    }

    PushUnsignedInt((unsigned int)value.length());
    remaining -= sizeof(unsigned int);
    for (unsigned int i = 0; i < value.length(); i++)
    {
        wchar_t character = value[i];
        short short_character = character;
        short net_character = htons(short_character);
        MemCopy(current_data_pointer_, remaining, reinterpret_cast<char *>(&net_character), sizeof(net_character));
        current_data_pointer_ += sizeof(net_character);
        remaining -= sizeof(net_character);
    }
}

void CommunicationBuffer::PushData(const BYTE *data, unsigned int data_size)
{
    if (data_size == 0)
    {
        GCSOutputDebugString(L"Wrong data size in PushData (value = 0)\n");
        return;
    }

    unsigned int remaining = size_ - static_cast<int>(current_data_pointer_ - buffer_);
    if (data_size > remaining)
    {
        GCSOutputDebugString(L"Unable to PushData (Buffer too small)\n");
        return;
    }

    MemCopy(current_data_pointer_, remaining, data, data_size);
    current_data_pointer_ += data_size;
}

void CommunicationBuffer::PushGUID(const GUID &value)
{
    PushInt(value.Data1);
    PushShort(value.Data2);
    PushShort(value.Data3);
    PushData(value.Data4, 8);
}
