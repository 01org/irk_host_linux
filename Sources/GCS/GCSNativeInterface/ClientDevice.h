#ifndef CLIENT_DEVICE_H_
#define CLIENT_DEVICE_H_

#include "RefObject.h"
#include "ClientDeviceType.h"
#include <pugixml.hpp>
#include "GUID.h"

class ClientConnection;
class ClientDeviceManager;
struct IXmlWriter;
struct IStream;

class ClientDevice : public RefObject
{
public:
    ClientDevice();
    
    void AddProperty(const std::wstring &name, const std::wstring &value);
    void SetConnected(bool connected);
    void SetClientConnection(ClientConnection *client_connection);
    void SetDeviceManager(ClientDeviceManager *device_manager);
    void Forget();

    ClientDeviceType GetType() const;
    const std::wstring &GetName() const;
    const std::wstring &GetDeviceType() const;
    const std::wstring &GetDeviceVersion() const;
    const std::wstring &GetDeviceMake() const;
    int GetDeviceWidth() const;
    int GetDeviceHeight() const;
    unsigned long GetLastConnected() const;
    const GUID &GetStoreID() const;
    const GUID &GetServerIdentity() const;
    const std::wstring &GetDeviceID();
    bool IsTrusted() const;
    bool IsWifiDirectSupported() const;

    bool IsConnected() const;
    bool IsValid() const;
    bool IsSameDevice(const ClientDevice *device) const;
    bool IsSamePairing(const ClientDevice *device) const;
    bool IsMoreRecentThanDevice(const ClientDevice *device) const;
    bool HasConnection(const ClientConnection *connection) const;
    bool CanConnect() const;
    void SetCanConnect(bool can_connect);

    void SetAuthenticated(bool is_authenticated);
    
    void WriteToXML(pugi::xml_document &doc);

protected:
    virtual ~ClientDevice();

private:
    DISALLOW_COPY_AND_ASSIGN(ClientDevice);

    ClientDeviceType type_;
    std::wstring name_;
    std::wstring pairing_id_;
    std::wstring device_id_;
    std::wstring version_;
    std::wstring device_type_;
    std::wstring device_make_;
    int device_width_;
    int device_height_;
    bool wifi_direct_support_;
    bool trusted_;
    unsigned long last_connected_;
    GUID store_id_;
    GUID server_identity_;
    bool can_connect_;

    ClientConnection *client_connection_;
    bool connected_;

    ClientDeviceManager *device_manager_;
};

#endif // CLIENT_DEVICE_H_
