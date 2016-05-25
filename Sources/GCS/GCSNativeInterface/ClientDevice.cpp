#include "StdAfx.h"
#include "ClientDevice.h"
#include "ClientDeviceManager.h"
#include "ClientConnection.h"
#include "GUID.h"

namespace
{
    const std::wstring kDevices(L"Devices");
    const std::wstring kDevice(L"Device");
    const std::wstring kDeviceIdentityKey(L"DeviceID");
    const std::wstring kDeviceNameKey(L"DeviceName");
    const std::wstring kDeviceMakeKey(L"DeviceMake");
    const std::wstring kDeviceSystemKey(L"DeviceSystem");
    const std::wstring kDeviceTypeKey(L"DeviceType");
    const std::wstring kDeviceVersionKey(L"DeviceVersion");
    const std::wstring kDeviceWidthKey(L"DeviceWidth");
    const std::wstring kDeviceHeightKey(L"DeviceHeight");
    const std::wstring kDeviceWifiDirectKey(L"WifiDirectSupport");

    const std::wstring kPairingIdKey(L"PairingId");
    const std::wstring kCertId(L"CertId");
    const std::wstring kPeerCertIdKey(L"PeerCertId");
    const std::wstring kTrustStateKey(L"TrustState");
    const std::wstring kStrikeCountKey(L"StrikeCount");
    const std::wstring kTrustExpiryKey(L"TrustExpiry");
    const std::wstring kFirstAuthorizedKey(L"FirstAuth");
    const std::wstring kLastAuthorizedKey(L"LastAuth");
    const std::wstring kFirstConnectedKey(L"FirstConnected");
    const std::wstring kLastConnectedKey(L"LastConnected");

    const std::wstring kServerIdentityKey(L"ServerIdentity");
    const std::wstring kStoreIdKey(L"StoreId");
    const std::wstring kCanConnectKey(L"CanConnect");

    static const int kFullGUIDStringLength = 39;
    static const int kGUIDStringLength = kFullGUIDStringLength - 2; //FullGUID has braces, which are removed for the string we use
};

ClientDevice::ClientDevice()
    : RefObject()
    , type_(CLIENT_DEVICE_TYPE_UNKNOWN)
    , name_()
    , pairing_id_()
    , device_id_()
    , version_()
    , device_type_()
    , device_make_()
    , device_width_(0)
    , device_height_(0)
    , wifi_direct_support_(false)
    , trusted_(false)
    , last_connected_(0)
    , store_id_(GUID_NULL)
    , server_identity_(GUID_NULL)
    , client_connection_(NULL)
    , connected_(false)
    , can_connect_(false)
{
}

// virtual
ClientDevice::~ClientDevice()
{
}

void ClientDevice::AddProperty(const std::wstring &name, const std::wstring &value)
{
    if (name == kPairingIdKey)
    {
        pairing_id_ = value;
    }
    else if (name == kDeviceIdentityKey)
    {
        device_id_ = value;
    }
    else if (name == kTrustStateKey)
    {
        trusted_ = value == L"true";
    }
    else if (name == kDeviceNameKey)
    {
        name_ = value;
    }
    else if (name == kDeviceMakeKey)
    {
        device_make_ = value;
    }
    else if (name == kDeviceSystemKey)
    {
        if (value.substr(0, 2) == L"iP")  // Anything starting with iP is iOS (like iPhone, iPad, iPod)
        {
            type_ = CLIENT_DEVICE_TYPE_IOS;
        }
        else if (value.substr(0,7) == L"Android")
        {
            type_ = CLIENT_DEVICE_TYPE_ANDROID;
        }
        else
        {
            type_ = CLIENT_DEVICE_TYPE_UNKNOWN;
        }
    }
    else if (name == kDeviceTypeKey)
    {
        device_type_ = value;
    }
    else if (name == kDeviceVersionKey)
    {
        version_ = value;
    }
    else if (name == kDeviceWidthKey)
    {
        wchar_t *end = NULL;
        device_width_ = wcstol(value.c_str(), &end, 10);
    }
    else if (name == kDeviceHeightKey)
    {
        wchar_t *end = NULL;
        device_height_ = wcstol(value.c_str(), &end, 10);
    }
    else if (name == kDeviceWifiDirectKey)
    {
        wchar_t *end = NULL;
        int int_value =  wcstol(value.c_str(), &end, 10);;
        wifi_direct_support_ = (int_value != 0);
    }
    else if (name == kLastConnectedKey)
    {
        wchar_t *end = NULL;
        last_connected_ = wcstoul(value.c_str(), &end, 10);
    }
    else if (name == kStoreIdKey)
    {
        store_id_ = GUIDFromString(value);
    }
    else if (name == kServerIdentityKey)
    {
        server_identity_ = GUIDFromString(value);
    }
    else if (name == kCanConnectKey)
    {
        can_connect_ = value == L"true";
    }
}

void ClientDevice::SetConnected(bool connected)
{
    connected_ = connected;
}

void ClientDevice::SetClientConnection(ClientConnection *client_connection)
{
    if(client_connection_ != NULL)
    {
        GCSOutputDebugString(L"ClientDevice setting client_connection_ when it wasn't null");
    }
    client_connection_ = client_connection;
}

void ClientDevice::SetDeviceManager(ClientDeviceManager *device_manager)
{
    device_manager_ = device_manager;
}

void ClientDevice::Forget()
{
   device_manager_->ForgetDevice(this);
}

ClientDeviceType ClientDevice::GetType() const
{
    return type_;
}

const std::wstring &ClientDevice::GetName() const
{
    return name_;
}

const std::wstring &ClientDevice::GetDeviceMake() const
{
    return device_make_;
}

const std::wstring &ClientDevice::GetDeviceType() const
{
    return device_type_;
}

const std::wstring &ClientDevice::GetDeviceVersion() const
{
    return version_;
}

int ClientDevice::GetDeviceWidth() const
{
    return device_width_;
}

int ClientDevice::GetDeviceHeight() const
{
    return device_height_;
}

bool ClientDevice::IsWifiDirectSupported() const
{
    return wifi_direct_support_;
}

unsigned long ClientDevice::GetLastConnected() const
{
    return last_connected_;
}

const GUID & ClientDevice::GetStoreID() const
{
    return store_id_;
}

const GUID & ClientDevice::GetServerIdentity() const
{
    return server_identity_;
}

const std::wstring &ClientDevice::GetDeviceID()
{
    return device_id_;
}

bool ClientDevice::IsTrusted() const
{
    return trusted_;
}

bool ClientDevice::IsConnected() const
{
    return connected_;
}

bool ClientDevice::IsValid() const
{
    return (!device_id_.empty() &&
            !IsEqualGUID(store_id_, GUID_NULL) &&
            !IsEqualGUID(server_identity_, GUID_NULL));
}

bool ClientDevice::IsSameDevice(const ClientDevice *device) const
{
    return device_id_ == device->device_id_;
}

bool ClientDevice::IsSamePairing(const ClientDevice *device) const
{
    bool same = (wcscmp(pairing_id_.c_str(), device->pairing_id_.c_str()) == 0);
    return same;
}

bool ClientDevice::IsMoreRecentThanDevice(const ClientDevice *device) const
{
    return (last_connected_ > device->last_connected_);
}

bool ClientDevice::HasConnection(const ClientConnection *connection) const
{
    return (client_connection_ == connection &&  (connection != NULL && last_connected_ == static_cast<unsigned long>(connection->GetConnectionTime())));
}

bool ClientDevice::CanConnect() const
{

    return can_connect_;
}

void ClientDevice::SetCanConnect(bool can_connect)
{
    can_connect_ = can_connect;
}

void ClientDevice::SetAuthenticated(bool is_authenticated)
{
    trusted_ = is_authenticated;
}

void ClientDevice::WriteToXML(pugi::xml_document &doc)
{
    const int kBufferSize = 100;
    const int kFullGUIDStringSize = 39;

    // add document main root node
    if(!doc.child(kDevices.c_str()))
    {
        return;
    }

    pugi::xml_node device_root = doc.child(kDevices.c_str()).append_child(kDevice.c_str());

    // device system
    device_root.append_attribute(kDeviceSystemKey.c_str());
    device_root.append_attribute(kDeviceNameKey.c_str());
    device_root.append_attribute(kPairingIdKey.c_str());
    device_root.append_attribute(kDeviceIdentityKey.c_str());
    device_root.append_attribute(kDeviceVersionKey.c_str());
    device_root.append_attribute(kDeviceTypeKey.c_str());
    device_root.append_attribute(kDeviceMakeKey.c_str());
    device_root.append_attribute(kDeviceWidthKey.c_str());
    device_root.append_attribute(kDeviceHeightKey.c_str());
    device_root.append_attribute(kDeviceWifiDirectKey.c_str());
    device_root.append_attribute(kTrustStateKey.c_str());
    device_root.append_attribute(kLastConnectedKey.c_str());
    device_root.append_attribute(kStoreIdKey.c_str());
    device_root.append_attribute(kServerIdentityKey.c_str());
    device_root.append_attribute(kCanConnectKey.c_str());

    // populate the device xml with this device's data
    device_root.attribute(kDeviceSystemKey.c_str()) = type_ == CLIENT_DEVICE_TYPE_IOS ? L"iPhone" : type_ == CLIENT_DEVICE_TYPE_ANDROID ? L"Android" : L"Unknown";
    device_root.attribute(kDeviceNameKey.c_str()) = name_.c_str();
    device_root.attribute(kPairingIdKey.c_str()) = pairing_id_.c_str();
    device_root.attribute(kDeviceIdentityKey.c_str()) = device_id_.c_str();
    device_root.attribute(kDeviceVersionKey.c_str()) = version_.c_str();
    device_root.attribute(kDeviceTypeKey.c_str()) = device_type_.c_str();
    device_root.attribute(kDeviceMakeKey.c_str()) = device_make_.c_str();
    device_root.attribute(kDeviceWidthKey.c_str()) = device_width_;
    device_root.attribute(kDeviceHeightKey.c_str()) = device_height_;
    device_root.attribute(kDeviceWifiDirectKey.c_str()) = wifi_direct_support_;
    device_root.attribute(kTrustStateKey.c_str()) = trusted_;
    device_root.attribute(kLastConnectedKey.c_str()) = static_cast<unsigned int>(last_connected_);

    std::wstring store_id_string = StringFromGUID(store_id_);
    device_root.attribute(kStoreIdKey.c_str()) = store_id_string.c_str();
    
    std::wstring server_id_string = StringFromGUID(server_identity_);
    device_root.attribute(kServerIdentityKey.c_str()) = server_id_string.c_str();
    
    device_root.attribute(kCanConnectKey.c_str()) = can_connect_;
}
