#include "StdAfx.h"
#include "NetworkBuffer.h"
#include "IPAddress.h"

NetworkBuffer::NetworkBuffer()
{
}

// virtual
NetworkBuffer::~NetworkBuffer()
{
}

bool NetworkBuffer::ReadInt(int &value, int timeoutMilliseconds)
{
    if (!ReadData(reinterpret_cast<BYTE *>(&value), sizeof(value), timeoutMilliseconds))
    {
        return false;
    }

    value = ntohl(value);
    return true;
}

bool NetworkBuffer::ReadUnsignedInt(unsigned int &value, int timeoutMilliseconds)
{
    if (!ReadData(reinterpret_cast<BYTE *>(&value), sizeof(value), timeoutMilliseconds))
    {
        return false;
    }

    value = ntohl(value);
    return true;
}

bool NetworkBuffer::ReadShort(short &value, int timeoutMilliseconds)
{
    if (!ReadData(reinterpret_cast<BYTE *>(&value), sizeof(value), timeoutMilliseconds))
    {
        return false;
    }

    value = ntohs(value);
    return true;
}

bool NetworkBuffer::ReadFloat(float &value, int timeoutMilliseconds)
{
    int int_value = 0;
    if (!ReadInt(int_value, timeoutMilliseconds))
    {
        return false;
    }

    value = *reinterpret_cast<float *>(&int_value);
    return true;
}

bool NetworkBuffer::ReadChar(char &value, int timeoutMilliseconds)
{
    return ReadData(reinterpret_cast<BYTE *>(&value), sizeof(value), timeoutMilliseconds);
}

bool NetworkBuffer::ReadWChar(wchar_t &value, int timeoutMilliseconds)
{
    short short_value = 0;
    if (!ReadShort(short_value, timeoutMilliseconds))
    {
        return false;
    }

    value = *reinterpret_cast<wchar_t *>(&short_value);
    value &= 0x0000ffff; //wchar_t is 32bits on linux, but we are sending 16 bits, so make sure first two bytes are all zeros

    return true;
}

bool NetworkBuffer::ReadByte(BYTE &value, int timeoutMilliseconds)
{
    return ReadData(reinterpret_cast<BYTE *>(&value), sizeof(value), timeoutMilliseconds);
}

bool NetworkBuffer::ReadBool(bool &value, int timeoutMilliseconds)
{
    BYTE byte = 0;
    if (!ReadByte(byte, timeoutMilliseconds))
    {
        return false;
    }

    value = (byte != 0);
    return true;
}

bool NetworkBuffer::ReadWString(std::wstring &value, int timeoutMilliseconds)
{
    int string_length = 0;
    if (!ReadInt(string_length, timeoutMilliseconds))
    {
        return false;
    }

    value.clear();

    if(string_length == 0)
    {
        return true;
    }

    if (string_length < 0)
    {
        GCSOutputDebugString(L"Unable to ReadString (string length is invalid)");
        return false;
    }

    wchar_t read_character = L'\0';

    for (int i = 0; i < string_length; ++i)
    {
        if (!ReadWChar(read_character, timeoutMilliseconds))
        {
            return false;
        }

        value.push_back(read_character);
    }

    return true;
}

bool NetworkBuffer::ReadIPAddress(IPAddress &address, int timeoutMilliseconds)
{
    BYTE bytes[IPAddress::kIPAddressNumBytes];
    short port = 0;

    for (int i = 0; i < IPAddress::kIPAddressNumBytes; ++i)
    {
        if (!ReadByte(bytes[i], timeoutMilliseconds))
        {
            return false;
        }
    }

    if (!ReadShort(port, timeoutMilliseconds))
    {
        return false;
    }

    // Create a new ip address and copy it into the object given to us
    IPAddress ip_address(bytes, port);
    address = ip_address;
    return true;
}
