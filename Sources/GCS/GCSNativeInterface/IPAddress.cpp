#include "StdAfx.h"
#include "IPAddress.h"
#include <sstream>
#include <string.h>

IPAddress::IPAddress()
    : ipv4_address_()
    , mask_length_(kEmptyMaskLength)
{
    ipv4_address_as_string_[0] = L'\0';

    BYTE bytes[kIPAddressNumBytes];
    memset(&bytes, 0, sizeof(bytes));

    Init(bytes, 0);
}

IPAddress::IPAddress(BYTE bytes[kIPAddressNumBytes], unsigned short port, int mask_length)
    : ipv4_address_()
    , mask_length_(mask_length)
{
    ipv4_address_as_string_[0] = L'\0';
    Init(bytes, port);
}

IPAddress::IPAddress(BYTE bytes[kIPAddressNumBytes], unsigned short port, BYTE netmask_bytes[kIPAddressNumBytes])
    : ipv4_address_()
    , mask_length_(kDefaultMaskLength)
{
    ipv4_address_as_string_[0] = L'\0';
    
    mask_length_ = MaskLengthFromNetMask(netmask_bytes);
    Init(bytes, port);
}

//counts the number of bits set for a mask where all the bits left of the lowest set bit are ones
int NumberOfSetBits(BYTE byte)
{
    if(byte == 0)
    {
        return 0;
    }
    
    if(byte == 255)
    {
        return 8;
    }
    
    for(int i = 0; i < 8; ++i)
    {
        BYTE m = 1 << i;
        if(byte & m)
        {
            return 8 - i;
        }
    }

    return 0;
}

int IPAddress::MaskLengthFromNetMask(BYTE netmask_bytes[kIPAddressNumBytes])
{
    int mask_length = 0;
    for(int i = 0; i < kIPAddressNumBytes; ++i)
    {
        mask_length += NumberOfSetBits(netmask_bytes[i]);
    }
    return mask_length;
}

void IPAddress::Init(BYTE bytes[kIPAddressNumBytes], unsigned short port)
{
    std::wstringstream byte_buffer;

    for (int i = 0; i < IPAddress::kIPAddressNumBytes; ++i)
    {       
        if (i != 0)
        {
            byte_buffer << L"."; 
        }
        
        byte_buffer << bytes[i];
    }

    byte_buffer << L":";
    byte_buffer << port;

    byte_buffer >> ipv4_address_as_string_;

    memset(&ipv4_address_, 0, sizeof(ipv4_address_));
    ipv4_address_.sin_family = AF_INET;
    ipv4_address_.sin_port = htons(port);
    MemCopy(&(ipv4_address_.sin_addr.s_addr), sizeof(ipv4_address_.sin_addr.s_addr), bytes, sizeof(BYTE) * kIPAddressNumBytes);
}

IPAddress::IPAddress(const struct sockaddr_in *ipv4)
    : ipv4_address_()
    , mask_length_(kEmptyMaskLength)
{
    ipv4_address_as_string_[0] = L'\0';

    if (ipv4 != NULL)
    {
        BYTE bytes[kIPAddressNumBytes];
        MemCopy(bytes, sizeof(bytes), &(ipv4->sin_addr.s_addr), sizeof(bytes));
        unsigned short port = ntohs(ipv4->sin_port);
        Init(bytes, port);
    }
    else
    {
        BYTE bytes[kIPAddressNumBytes];
        memset(&bytes, 0, sizeof(bytes));
        Init(bytes, 0);
    }
}

IPAddress::IPAddress(const IPAddress &copy)
    : ipv4_address_()
{
    *this = copy;
}

IPAddress::~IPAddress()
{
}

IPAddress& IPAddress::operator=(const IPAddress &copy)
{
    MemCopy(&ipv4_address_, sizeof(ipv4_address_), &copy.ipv4_address_, sizeof(copy.ipv4_address_));
    ipv4_address_as_string_ = copy.ipv4_address_as_string_;

    return *this;
}

const struct sockaddr_in *IPAddress::GetIPV4Address() const
{
    return &ipv4_address_;
}

const std::wstring &IPAddress::AsString() const
{
    return ipv4_address_as_string_;
}

bool IPAddress::GetBroadcastAddress(BYTE *broadcast_address, int address_size) const
{
    if(broadcast_address == NULL || address_size != 4)
    {
        return false;
    }

    const unsigned long &address = ipv4_address_.sin_addr.s_addr;
    unsigned long mask = 0xffffffff >> (kFullMaskLength - mask_length_);
    unsigned long network = address & mask;
    unsigned long inverted_mask = ~mask;
    unsigned long &broadcast = *((unsigned long *)broadcast_address);
    broadcast = network | inverted_mask;

    return true;
}
