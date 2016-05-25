#ifndef CLIENT_INFO_MESSAGE_H_
#define CLIENT_INFO_MESSAGE_H_
#include "InputMessage.h"

class ClientDevice;

class ClientInfoMessage : public InputMessage
{    
public:
    ClientInfoMessage();
    virtual ~ClientInfoMessage();

    const std::map<std::wstring, std::wstring> &GetClientInfo() const;
    ClientDevice *GetClientDeviceFromInfo() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(ClientInfoMessage);

    std::map<std::wstring, std::wstring> client_info_;
};

#endif // CLIENT_INFO_MESSAGE_H_
