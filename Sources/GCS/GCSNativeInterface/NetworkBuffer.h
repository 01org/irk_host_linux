#ifndef NETWORK_BUFFER_H_
#define NETWORK_BUFFER_H_

class IPAddress;

class NetworkBuffer
{
public:
    NetworkBuffer();
    virtual ~NetworkBuffer();
    
    bool ReadInt(int &value, int timeoutMilliseconds);
    bool ReadUnsignedInt(unsigned int &value, int timeoutMilliseconds);
    bool ReadShort(short &value, int timeoutMilliseconds);
    bool ReadFloat(float &value, int timeoutMilliseconds);
    bool ReadChar(char &value, int timeoutMilliseconds);
    bool ReadWChar(wchar_t &value, int timeoutMilliseconds);
    bool ReadByte(BYTE &value, int timeoutMilliseconds);
    bool ReadBool(bool &value, int timeoutMilliseconds);
    bool ReadWString(std::wstring &value, int timeoutMilliseconds);
    bool ReadIPAddress(IPAddress &address, int timeoutMilliseconds);
    virtual bool ReadData(BYTE *data_buffer, unsigned int data_size, int timeoutMilliseconds) = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(NetworkBuffer);
};

#endif // NETWORK_BUFFER_H_
