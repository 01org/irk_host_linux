#include "StdAfx.h"
#include "ClientDeviceManager.h"
#include "BasicClientDevice.h"
#include "ClientDevice.h"
#include "UICallbacks.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

namespace
{
    static const float kWaitTimeBeforeUpdate = 0.5f;
    const std::wstring kDevices(L"Devices");
    const std::wstring kDeviceID(L"DeviceID");
}

ClientDeviceManager::ClientDeviceManager()
    : store_name_()
    , store_id_(GUID_NULL)
    , server_identity_(GUID_NULL)
    , known_devices_cs_()
    , known_devices_(NULL)
    , update_request_cs_()
    , needs_update_(false)
    , time_until_next_update_(0.0f)
    , device_connected_(false)
{
    known_devices_ = new std::vector<ClientDevice *>();
}

ClientDeviceManager::~ClientDeviceManager()
{
    Uninit();
    SAFE_DELETE(known_devices_);
}

void ClientDeviceManager::Init(const std::wstring &store_name, const GUID &store_id, const GUID &server_identity)
{
    store_name_ = store_name;
    store_id_ = store_id;
    server_identity_ = server_identity;

    LoadSavedDevices();

    RequestUpdate();
    UICallbacks::RefreshDeviceList();
}

void ClientDeviceManager::Uninit()
{
    if(IsEqualGUID(store_id_, GUID_NULL))
    {
        //We are not initialized, so there is nothing to do
        return;
    }

    SaveAllDevices();

    ClearDeviceList();

    store_name_.clear();
    store_id_ = GUID_NULL;
    server_identity_ = GUID_NULL;
}

void ClientDeviceManager::Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

void ClientDeviceManager::AddKnownDevice(ClientDevice *device)
{
    AddKnownDevice(device, true);
}

void ClientDeviceManager::AddKnownDevice(ClientDevice *device, bool save)
{
    if (device != NULL)
    {
        device->SetDeviceManager(this);
        known_devices_cs_.Enter();

        //Check if we already have an entry for the device being added
        int existing_device_index = -1;
        for(unsigned int i = 0; i < known_devices_->size(); ++i)
        {
            ClientDevice *d = known_devices_->at(i);
            if(d->IsSameDevice(device))
            {
                existing_device_index = i;
                break;
            }
        }

        known_devices_cs_.Leave(); 

        if(existing_device_index != -1)
        {
            ClientDevice *existing_device = known_devices_->at(existing_device_index);
            device->SetAuthenticated(existing_device->IsTrusted());
            device->SetCanConnect(existing_device->CanConnect());
            device->SetConnected(existing_device->IsConnected());
            RemoveKnownDevice(known_devices_->at(existing_device_index));
        }
        
        known_devices_cs_.Enter();

        device->AddRef();
        known_devices_->push_back(device);

        known_devices_cs_.Leave(); 

        if(save && device->IsTrusted())
        {
            SaveAllDevices();
        }

        UICallbacks::RefreshDeviceList();
    }
}

void ClientDeviceManager::SetAuthenticated(const ClientConnection *connection)
{
    ClientDevice *device = DeviceWithConnection(connection);
    if(device != NULL)
    {
        device->SetAuthenticated(true);
        device->SetConnected(true);
        SaveAllDevices();
        UICallbacks::RefreshDeviceList();
    }
}

void ClientDeviceManager::GetBasicClientDevices(std::vector<BasicClientDevice *> &devices)
{
    for(unsigned int i = 0; i < devices.size(); ++i)
    {
        delete devices[i];
    }
    devices.clear();

    known_devices_cs_.Enter();

    for(unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *known_device = known_devices_->at(i);
        if(known_device->IsTrusted())
        {
            BasicClientDevice *device = new BasicClientDevice(known_device->GetType(), known_device->GetName(), known_device->GetDeviceID(), known_device->GetDeviceType(), known_device->GetDeviceMake());
            devices.push_back(device);
        }
    }

    known_devices_cs_.Leave();
}

void ClientDeviceManager::SetDeviceConnected(bool connected)
{
    if (device_connected_ != connected)
    {
        device_connected_ = connected;
        UpdateConnectedDevice();
        UICallbacks::RefreshDeviceList();
    }
}

void ClientDeviceManager::UpdateConnectedDevice()
{
    ClientDevice *latest_connected_device = NULL;
    
    known_devices_cs_.Enter();

    // We must first clear the connected flag from all the devices.
    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *device = (*known_devices_)[i];
        device->SetConnected(false);

        unsigned long last_connected_value = device->GetLastConnected();
        if (last_connected_value > 0 && !device->HasConnection(NULL))
        {
            if (latest_connected_device == NULL || last_connected_value > latest_connected_device->GetLastConnected())
            {
                latest_connected_device = device;
            }
        }
    }

    // If we have a connected device, we can set the device that has the latest connection info.
    if (device_connected_ && latest_connected_device != NULL)
    {
        latest_connected_device->SetConnected(true);
    }

    known_devices_cs_.Leave();
}

void ClientDeviceManager::AllowConnections(bool only_connected)
{ 
    UpdateConnectedDevice();
    known_devices_cs_.Enter();

    int number_allowed = 0;

    // We must first clear the connected flag from all the devices.
    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *device = (*known_devices_)[i];
        device->SetCanConnect(only_connected ? device->IsConnected() && device->IsTrusted() : true);
        number_allowed += only_connected 
            ? device->IsConnected() ? 1 : 0
            : true;
    }

    known_devices_cs_.Leave();

    SaveAllDevices();
}

void ClientDeviceManager::RemoveKnownDevice(ClientDevice *device)
{
    if (device != NULL)
    {
        bool removed_a_device = false;

        known_devices_cs_.Enter();

        std::vector<unsigned int> device_indices_to_remove;
        std::vector<ClientDevice *>::iterator iter = known_devices_->begin();
        for(unsigned int i = 0; i < known_devices_->size(); ++i)
        {
            ClientDevice *iter_device = known_devices_->at(i);
            if (device->IsSameDevice(iter_device))
            {
                device_indices_to_remove.push_back(i);
            }
        }

        removed_a_device = !device_indices_to_remove.empty();

        //Go backwards through the indices to remove so the later indices don't become wrong
        for(int i = static_cast<int>(device_indices_to_remove.size()) - 1; i >= 0 && i < static_cast<int>(device_indices_to_remove.size()); --i)
        {
            known_devices_->erase(known_devices_->begin() + device_indices_to_remove[i]);
        }

        known_devices_cs_.Leave();

        if(removed_a_device)
        {
            SaveAllDevices();
            //device->Release();
        }
        
        UICallbacks::RefreshDeviceList();
    }
}

void ClientDeviceManager::ForgetDevice(ClientDevice *device)
{
    wchar_t fileName[MAX_PATH] = L"/var/lib/RemoteKeyboard/Devices.xml";

    pugi::xml_document doc;
    doc.load_file(fileName);

    const std::wstring name_compare(kDeviceID);

    std::wstring value_compare = device->GetDeviceID();

    //Remove the entry from the XML
    pugi::xml_node devices = doc.child(kDevices.c_str());
    pugi::xml_node node_to_remove;
    bool found = false;

    for(pugi::xml_node_iterator node_it = devices.begin(); node_it != devices.end(); ++node_it)
    {
        //ClientDevice *client_device = new ClientDevice();
        for(pugi::xml_attribute_iterator attribute_it = node_it->attributes_begin(); attribute_it != node_it->attributes_end(); ++attribute_it)
        {
            std::wstring name(attribute_it->name());
            std::wstring value(attribute_it->value());

            if(name_compare == name)
            {
                if(value_compare == value)
                {
                    node_to_remove = doc.child(kDevices.c_str()).find_child_by_attribute(name.c_str(), value.c_str());
                    found = true;
                    break;
                }
            }
        }
        if(found)
        {
            break;
        }
    }

    doc.child(kDevices.c_str()).remove_child(node_to_remove);

    RemoveKnownDevice(device);
}

bool ClientDeviceManager::DeviceRequiresAuthentication(const ClientDevice *device) const
{
    return device->IsTrusted();
}

ClientDevice *ClientDeviceManager::DeviceWithConnection(const ClientConnection *connection) const
{
    known_devices_cs_.Enter();

    ClientDevice *found_device = NULL;
    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *known_device = (*known_devices_)[i];
        if(known_device->HasConnection(connection))
        {
            found_device = known_device;
            break;
        }
    }

    known_devices_cs_.Leave();

    return found_device;
}

ClientDevice *ClientDeviceManager::DeviceWithID(const std::wstring &device_id) const
{
    known_devices_cs_.Enter();

    ClientDevice *found_device = NULL;
    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *known_device = (*known_devices_)[i];
        if(known_device->GetDeviceID() == device_id)
        {
            found_device = known_device;
            break;
        }
    }

    known_devices_cs_.Leave();

    return found_device;
}

void ClientDeviceManager::ForgetAllDevices()
{
    known_devices_cs_.Enter();

    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *known_device = (*known_devices_)[i];
        known_device->Forget();
        known_device->Release();
    }
    known_devices_->clear();

    known_devices_cs_.Leave();
    
    UICallbacks::RefreshDeviceList();
}

int ClientDeviceManager::GetNumKnownDevices() const
{
    known_devices_cs_.Enter();
    int num_devices = static_cast<int>(known_devices_->size());
    known_devices_cs_.Leave();

    return num_devices;
}

ClientDevice * ClientDeviceManager::GetKnownDevice(int index)
{
    ClientDevice *client_device = NULL;

    known_devices_cs_.Enter();

    if (index >= 0 && index < static_cast<int>(known_devices_->size()))
    {
        client_device = (*known_devices_)[index];
    }

    known_devices_cs_.Leave();

    return client_device;
}

void ClientDeviceManager::ClearDeviceList()
{
    known_devices_cs_.Enter();

    for (unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        SAFE_RELEASE((*known_devices_)[i]);
    }
    known_devices_->clear();

    known_devices_cs_.Leave();
    
    UICallbacks::RefreshDeviceList();
}

void ClientDeviceManager::RequestUpdate()
{
    update_request_cs_.Enter();
    if (!needs_update_)
    {
        needs_update_ = true;
        time_until_next_update_ = kWaitTimeBeforeUpdate;
    }
    update_request_cs_.Leave();
}

void ClientDeviceManager::LoadSavedDevices()
{
    wchar_t fileName[MAX_PATH] = L"/var/lib/RemoteKeyboard/Devices.xml";

    pugi::xml_document doc;
    doc.load_file(fileName);
    
    pugi::xml_node devices = doc.child(kDevices.c_str());

    for(pugi::xml_node_iterator node_it = devices.begin(); node_it != devices.end(); ++node_it)
    {
        ClientDevice *client_device = new ClientDevice();
        client_device->SetCanConnect(true); //Default to true in case the device file doesn't have the attribute
        for(pugi::xml_attribute_iterator attribute_it = node_it->attributes_begin(); attribute_it != node_it->attributes_end(); ++attribute_it)
        {
            const std::wstring name(attribute_it->name());
            const std::wstring value(attribute_it->value());

            client_device->AddProperty(name, value);
        }

        if(client_device->IsValid() && IsEqualGUID(store_id_, client_device->GetStoreID()) && IsEqualGUID(server_identity_, client_device->GetServerIdentity()))
        {
            AddKnownDevice(client_device, false);
        }

        SAFE_RELEASE(client_device);
    }
}

void ClientDeviceManager::SaveAllDevices()
{
    wchar_t fileName[MAX_PATH] = L"/var/lib/RemoteKeyboard/Devices.xml";

    pugi::xml_document doc;
    doc.append_child(kDevices.c_str());

    //Loop through devices
    known_devices_cs_.Enter();

    for(unsigned int i = 0; i < known_devices_->size(); ++i)
    {
        ClientDevice *d = known_devices_->at(i);
        d->WriteToXML(doc);
    }

    known_devices_cs_.Leave();
    doc.save_file(fileName);
}
