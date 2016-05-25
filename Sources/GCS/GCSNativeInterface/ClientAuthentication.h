#ifndef CLIENT_AUTHENTICATION_H_
#define CLIENT_AUTHENTICATION_H_

class ClientConnection;
class ClientDeviceManager;

class ClientAuthentication
{
public:
    ClientAuthentication(ClientConnection *client_connection, ClientDeviceManager *device_manager);
    virtual ~ClientAuthentication();

    void Start();
    void Cancel();
    void OnCancel();
    void Allow(bool remember);
    void TryPasscode(const std::wstring &passcode);

    bool IsInProgress() const;
    bool HasClientConnection(const ClientConnection *client_connection) const;

private:
    DISALLOW_COPY_AND_ASSIGN(ClientAuthentication);
    void Cleanup();

    //Due to the UI, we can only have one QR code displayed at a time, so we can only authenticate
    //one client at a time.
    static ClientAuthentication *s_current_client_authentication;

    ClientConnection * const client_connection_;
    ClientDeviceManager *device_manager_;
    std::wstring passcode_;
    int attempts_;
};

#endif // CLIENT_AUTHENTICATION_H_
