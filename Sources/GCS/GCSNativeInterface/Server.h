#ifndef SERVER_H_
#define SERVER_H_

#include "CriticalSection.h"
#include "ServerType.h"
#include <string>
#include <vector>
#include "GUID.h"

class BasicClientDevice;

class ClientAuthentication;
class ClientConnection;
class ClientDevice;
class ClientDeviceManager;
class ConnectionRequestHandler;
class InputMessage;
class InputMessageFactory;
class NetworkSocket;
class StopWatch;

class Server
{
public:
    Server(ServerType type);
    virtual ~Server();

    void Init(GUID &store_id, GUID &server_identity);
    void Uninit();

    void SocketClientAdded(NetworkSocket *clientSocket);

    void Suspend();
    void Resume();
    void Shutdown();

    void SetName(const std::wstring &name);

    int GetNumKnownDevices() const;
    ClientDevice * GetKnownDevice(int index);
    void ForgetDevice(ClientDevice *device);
    void ForgetDevice(const std::wstring &device_id);
    void ForgetAllDevices();

    void QueueAuthenticationRequest(ClientConnection *connection);
    void SendServerInfo(ClientConnection *connection);
    void ProcessMessage(ClientConnection *connection, const InputMessage *message, bool &exitThread);
    void AllowConnection(bool remember);
    void CancelAuthenticationRequest();
    void OnAuthenticationCanceled();

    InputMessageFactory *GetInputMessageFactory();

    void UpdateClientConnections();
    void SetDoNotDisturb(bool do_not_disturb);
    static void GetServerName(std::wstring &name);

    ServerType GetType() const;
    bool GetName(std::wstring &name) const;
    unsigned int GetNameLength() const;

    const GUID GetServerIdentity() const;
    
    void GetKnownDevices(std::vector<BasicClientDevice *> &devices);

    void CheckForVolumeChange();
    void CheckForMuteChange();
    void SetVolumeLevel(float volume_level);
    void SendVolumeLevelMessage(ClientConnection* client_connection);
    void SendMuteStateMessage(ClientConnection* client_connection);

    void CheckForElevatedPrivilegeChange();
    void SendElevatedPrivilegesMessage(bool is_elevated);
    void SendLogonInfo(bool is_at_logon);

    void SendCurrentStatus();

    void Show(bool show);
    bool IsVisible() const;
    bool ShouldShow() const;
    void SetShouldShow(bool show);
    bool ShouldHide() const;
    void SetShouldHide(bool hide);

private:
    DISALLOW_COPY_AND_ASSIGN(Server);

    void AllowConnections(bool allow_connections);
    void CreateRequestHandlers();

    int StartLinuxInputHandler();

    static bool GetUserGCSName(std::wstring &name);
    static bool GetComputerDescriptionName(std::wstring &name);
    static bool GetHostName(std::wstring &name);

    void SetDNDFromOptions();

    float GetVolumeLevel() const;
    bool GetMuteState() const;
    bool GetElevatedPrivilegeState() const;

    InputMessageFactory *input_message_factory_;
    StopWatch *update_stop_watch_;
    std::wstring wakeable_mac_addresses_;
    ServerType type_;
    GUID       store_id_;
    GUID       server_identity_;
    CriticalSection server_name_cs_;
    std::wstring server_name_;
    bool allow_connections_;
    
    CriticalSection client_connections_cs_;
    std::vector<ClientConnection *> *client_connections_;
    std::vector<ConnectionRequestHandler *> *connection_request_handlers_;
    ClientDeviceManager *client_device_manager_;
    
    CriticalSection current_client_authentication_cs_;
    ClientAuthentication *current_client_authentication_;

    bool do_not_disturb_;

    bool devices_changed_;

    bool has_network_connection_;

    float current_volume_level_;
    bool is_muted_;

    bool is_foreground_window_elevated_;

    bool is_visible_;
    bool should_show_;
    bool should_hide_;
    bool current_window_is_elevated_;
    bool is_at_logon_;
    
    int input_handler_fd_;
    time_t last_input_heartbeat_time_;
};

inline ServerType Server::GetType() const
{
    return type_;
}

#endif // SERVER_H_
