#ifndef IP_ADDRESS_H_
#define IP_ADDRESS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

class IPAddress
{
public:
    static const int kIPAddressNumBytes = 4;
    static const int kMaxByteStringSize = 3;
    static const int kMaxPortStringSize = 5;

    IPAddress();
    IPAddress(BYTE bytes[kIPAddressNumBytes], unsigned short port, int mask_length = kDefaultMaskLength);
    IPAddress(BYTE bytes[kIPAddressNumBytes], unsigned short port, BYTE netmask_bytes[kIPAddressNumBytes]);
    IPAddress(const struct sockaddr_in *ipv4);
    IPAddress(const IPAddress &copy);
    ~IPAddress();

    IPAddress& operator=(const IPAddress &copy);

    const struct sockaddr_in *GetIPV4Address() const;
    const std::wstring &AsString() const;

    bool GetBroadcastAddress(BYTE *broadcast_address, int address_size) const;

private:
    void Init(BYTE bytes[kIPAddressNumBytes], unsigned short port);
    int MaskLengthFromNetMask(BYTE netmask_bytes[kIPAddressNumBytes]);

    // max string size = number of byte values * (3 digits + separator) + max port size + end of string character
    static const int kMaxAddressStringSize = (kIPAddressNumBytes * (kMaxByteStringSize + 1)) + kMaxPortStringSize + 1;

    struct sockaddr_in ipv4_address_;

    int mask_length_;

    std::wstring ipv4_address_as_string_;

    static const int kEmptyMaskLength = 0;
    static const int kDefaultMaskLength = 24; //bits
    static const int kFullMaskLength = 32;
};

#endif // IP_ADDRESS_H_
