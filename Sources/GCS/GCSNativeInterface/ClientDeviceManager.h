#ifndef CLIENT_DEVICE_MANAGER_H_
#define CLIENT_DEVICE_MANAGER_H_

#include "CriticalSection.h"
#include "GUID.h"

class BasicClientDevice;

class ClientConnection;
class ClientDevice;

class ClientDeviceManager
{
public:
    ClientDeviceManager();
    virtual ~ClientDeviceManager();

    void Init(const std::wstring &store_name, const GUID &store_id, const GUID &server_identity);
    void Uninit();
    void SetDeviceConnected(bool connected);
    void Update(float dt);
    
    void AddKnownDevice(ClientDevice *device);

    bool DeviceRequiresAuthentication(const ClientDevice *device) const;
    ClientDevice *DeviceWithConnection(const ClientConnection *connection) const;
    ClientDevice *DeviceWithID(const std::wstring &device_id) const;

    void ForgetAllDevices();

    void ForgetDevice(ClientDevice *device);

    int GetNumKnownDevices() const;
    ClientDevice * GetKnownDevice(int index);
    void SetAuthenticated(const ClientConnection *connection);

    void GetBasicClientDevices(std::vector<BasicClientDevice *> &devices);

    void AllowConnections(bool only_connected);

private:
    DISALLOW_COPY_AND_ASSIGN(ClientDeviceManager);

    void PopulateDeviceList();
    void ClearDeviceList();
    void RequestUpdate();
    void UpdateConnectedDevice();
    
    void RemoveKnownDevice(ClientDevice *device);

    void AddKnownDevice(ClientDevice *device, bool save);

    void LoadSavedDevices();
    void SaveAllDevices();

    bool device_connected_;

    std::wstring store_name_;
    GUID store_id_;
    GUID server_identity_;
    
    CriticalSection update_request_cs_;
    CriticalSection known_devices_cs_;
    std::vector<ClientDevice *> *known_devices_;

    bool needs_update_;
    float time_until_next_update_;
};

#endif // CLIENT_DEVICE_MANAGER_H_
