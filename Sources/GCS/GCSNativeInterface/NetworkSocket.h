#ifndef NETWORK_SOCKET_H_
#define NETWORK_SOCKET_H_

typedef int SOCKET;

class IPAddress;

class NetworkSocket
{
public:
    enum Type
    {
        UDP,
        TCP,
        MULTICAST
    };

    NetworkSocket(Type type);
    virtual ~NetworkSocket();

    bool EnableBroadcast();
    bool EnableMulticast(const IPAddress &address);
    bool Bind(const IPAddress *ip_address);
    bool GetAddress(IPAddress *destination) const;
    bool Listen(int connection_queue_size);
    bool Send(const IPAddress *to_address, const BYTE *bytes, int num_bytes);
    bool Send(const BYTE *bytes, int num_bytes);
    int Receive(IPAddress *from_address, BYTE *bytes, int num_bytes);
    bool IsConnected();

    NetworkSocket * Accept();

private:
    DISALLOW_COPY_AND_ASSIGN(NetworkSocket);

    NetworkSocket(Type type, SOCKET socket_);
    void Init();

    Type type_;
    SOCKET socket_;

    bool is_connected_;
};

#endif // NETWORK_SOCKET_H_
