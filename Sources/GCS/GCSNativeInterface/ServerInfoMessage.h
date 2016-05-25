#ifndef SERVER_INFO_MESSAGE_H_
#define SERVER_INFO_MESSAGE_H_
#include "OutputMessage.h"
#include "GUID.h"

class CommunicationBuffer;

class ServerInfoMessage : public OutputMessage
{
public:
    ServerInfoMessage();
    ServerInfoMessage(GUID server_identity);
    ServerInfoMessage(std::wstring wol_mac_addresses, GUID server_identity);
    virtual ~ServerInfoMessage();

protected:
    virtual int GetDataSize() const;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ServerInfoMessage); 

    bool windows_8_or_higher_;
    int windows_major_version_;
    int windows_minor_version_;
    GUID server_identity_;
    int wol_mac_addresses_length_;
    std::wstring wol_mac_addresses_value_;
};

#endif // SERVER_INFO_MESSAGE_H_
