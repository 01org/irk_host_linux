#include "StdAfx.h"
#include "Server.h"
#include "AuthenticationFailedMessage.h"
#include "AuthenticationSuccessMessage.h"
#include "CommunicationBuffer.h"
#include "ClientAuthentication.h"
#include "ClientConnection.h"
#include "ClientDevice.h"
#include "ClientDeviceManager.h"
#include "ConnectionRequestHandler.h"
#include "InputMessageFactory.h"
#include "IPAddress.h"
#include "StopWatch.h"
#include "KeyboardEventMessage.h"
#include "MouseMoveEventMessage.h"
#include "MouseButtonEventMessage.h"
#include "MouseScrollEventMessage.h"
#include "DisconnectMessage.h"
#include "DockInfoMessage.h"
#include "PasscodeMessage.h"
#include "ServerInfoMessage.h"
#include "SetLogMessage.h"
#include "SuspendGCCMessage.h"
#include "UICallbacks.h"
#include "UnicharEventMessage.h"
#include "Utilities.h"
#include "ControlPCMessage.h"
#include "ClientInfoMessage.h"
#include "Version.h"
#include "NetworkSocket.h"
#include "MediaVolumeChangedOutputMessage.h"
#include "MediaVolumeChangedInputMessage.h"
#include "MediaMuteChangedOutputMessage.h"
#include "ElevatedPrivilegesOutputMessage.h"
#include "LogonInfoOutputMessage.h"
#include "PowerOptionInputMessage.h"
#include <fstream>
#include <ifaddrs.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <signal.h>

namespace
{
    static const std::wstring kDesktopSensorType(L"GCS-Desktop");
    static const std::wstring kLaptopSensorType(L"GCS-Laptop");
    static const std::wstring kDefaultGCSName(L"My Remote Keyboard Host");
    static const std::wstring kCCFStoreName(L"PCADK");
    static const int kInitialIPAddressBufferSize = 10 * 1024;
    static const int MAX_SENSOR_TYPE_LENGTH = 64;
    static const int kFullGUIDStringLength = 39;
    static const int kGUIDStringLength = kFullGUIDStringLength - 2; //FullGUID has braces, which are removed for the string we use
    static const int kMaxIntStringLength = 12; //10 characters for the number (2,147,483,647), 1 for the sign, and 1 for the terminating character
}

#define CHECK_ERROR_AND_RETURN_ON_ERROR(s, x) \
if (x != NoError) \
{ \
    GCSOutputDebugString(s, x); \
    return x; \
}

Server::Server(ServerType type)
    : type_(type)
    , server_identity_()
    , store_id_()
    , client_connections_cs_()
    , client_connections_(NULL)
    , input_message_factory_(NULL)
    , current_client_authentication_(NULL)
    , do_not_disturb_(false)
    , update_stop_watch_(NULL)
    , client_device_manager_(NULL)
    , server_name_()
    , allow_connections_(true)
    , wakeable_mac_addresses_()
    , connection_request_handlers_(NULL)
    , devices_changed_(true)
    , input_handler_fd_(-1)
    , last_input_heartbeat_time_(-1)
    , has_network_connection_(false)
    , current_volume_level_(1.0f)
    , is_muted_(false)
    , is_foreground_window_elevated_(false)
    , is_visible_(false)
    , should_show_(false)
    , should_hide_(false)
    , current_window_is_elevated_(false)
    , is_at_logon_(false)
{
GCSStartLog();

    client_connections_ = new std::vector<ClientConnection *>();

    input_message_factory_ = new InputMessageFactory();
    input_message_factory_->SetMessageBuilder(InputMessage::kKeyboardEventMessage, TemplateMessageBuilder<KeyboardEventMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kMouseMoveEventMessage, TemplateMessageBuilder<MouseMoveEventMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kMouseButtonEventMessage, TemplateMessageBuilder<MouseButtonEventMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kMouseScrollEventMessage, TemplateMessageBuilder<MouseScrollEventMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kDisconnectMessage, TemplateMessageBuilder<DisconnectMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kDockInfoMessage, TemplateMessageBuilder<DockInfoMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kUnicharEventMessage, TemplateMessageBuilder<UnicharEventMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kSuspendGCCMessage, TemplateMessageBuilder<SuspendGCCMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kControlPCMessage, TemplateMessageBuilder<ControlPCMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kClientInfoMessage, TemplateMessageBuilder<ClientInfoMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kPasscodeMessage, TemplateMessageBuilder<PasscodeMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kVolumeChangedMessage, TemplateMessageBuilder<MediaVolumeChangedInputMessage>);
    input_message_factory_->SetMessageBuilder(InputMessage::kPowerOptionMessage, TemplateMessageBuilder<PowerOptionInputMessage>);
    update_stop_watch_ = new StopWatch();
    update_stop_watch_->Start();
    
    input_handler_fd_ = StartLinuxInputHandler();

    //Set the name
    GetServerName(server_name_);

    std::vector<std::wstring> wakeable_mac_addresses;
    Utilities::EnumerateWakeableMacAddresses(&wakeable_mac_addresses);

    for(std::vector<std::wstring>::iterator it = wakeable_mac_addresses.begin(); it != wakeable_mac_addresses.end() ; it++)
    {
        wakeable_mac_addresses_.insert(wakeable_mac_addresses_.end(), it->begin(), it->end());
        if(it+1 != wakeable_mac_addresses.end())
        {
            wakeable_mac_addresses_.push_back(L',');
        }
    }

    client_device_manager_ = new ClientDeviceManager();

    SetDNDFromOptions();
}

Server::~Server()
{
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        SAFE_DELETE((*client_connections_)[i]);
    }
    SAFE_DELETE(client_connections_);

    SAFE_DELETE(input_message_factory_);
    SAFE_DELETE(update_stop_watch_);
    SAFE_DELETE(client_device_manager_);

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        ConnectionRequestHandler *handler = connection_request_handlers_->at(i);
        SAFE_DELETE(handler);
    }
    SAFE_DELETE(connection_request_handlers_);
    
    signal(SIGPIPE, SIG_IGN);
    if(input_handler_fd_ != -1 && fcntl(input_handler_fd_, F_GETFD) != -1)
    {
        write(input_handler_fd_, "Q", 1);
        wait();
        input_handler_fd_ = -1;
    }
}

void Server::Init(GUID &store_id, GUID &server_identity)
{
    store_id_ = store_id;
    server_identity_ = server_identity;

    if(server_identity_ == GUID_NULL)
    {
        UuidCreate(&server_identity_);
    }

    if(store_id_ == GUID_NULL)
    {
        UuidCreate(&store_id_);
    }

    current_volume_level_ = GetVolumeLevel();
    is_muted_ = GetMuteState();
    is_foreground_window_elevated_ = GetElevatedPrivilegeState();

    connection_request_handlers_ = new std::vector<ConnectionRequestHandler *>();
    CreateRequestHandlers();    

    has_network_connection_ = !connection_request_handlers_->empty();

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        connection_request_handlers_->at(i)->NotifyServerInfoChanged();
    }

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        connection_request_handlers_->at(i)->Resume();
    }

    client_device_manager_->Init(kCCFStoreName, store_id, server_identity);
}

void Server::Uninit()
{
    //close all connections
    client_connections_cs_.Enter();
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        client_connections_->at(i)->Disconnect(false);
    }
    client_connections_cs_.Leave();

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        connection_request_handlers_->at(i)->Suspend();
    }

    client_device_manager_->Uninit();
}

void Server::SetName(const std::wstring &name)
{
    server_name_cs_.Enter();
    if (name != server_name_)
    {
        server_name_ = name;
    }
    server_name_cs_.Leave();

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        connection_request_handlers_->at(i)->NotifyServerInfoChanged();
    }
}

void Server::QueueAuthenticationRequest(ClientConnection *connection)
{
    ClientDevice *device = client_device_manager_->DeviceWithConnection(connection);
    if(device == NULL)
    {
        return;
    }
    else if(!device->CanConnect())
    {
        AuthenticationFailedMessage authentication_failed(AFR_DoNotDisturb);
        connection->SendMessage(&authentication_failed);
    }
    else if(device->IsTrusted())
    {
        GCSOutputDebugString(L"SocketServer::QueueAuthenticationRequest - Client is already authenticated\n");
        AuthenticationSuccessMessage authentication_success;
        connection->SendMessage(&authentication_success);
        
        device->SetConnected(true);

        client_device_manager_->SetDeviceConnected(true);
    }
    else
    {
        current_client_authentication_cs_.Enter();
            if (current_client_authentication_ != NULL)
            {
                GCSOutputDebugString(L"SocketServer::QueueAuthenticationRequest - Client is already being authenticated.");
            }
            else if (connection != NULL)
            {
                if(client_device_manager_ == NULL)
                {
                    GCSOutputDebugString(L"SocketServer::QueueAuthenticationRequest - no ClientDeviceManager");
                }
                else
                {
                    current_client_authentication_ = new ClientAuthentication(connection, client_device_manager_);
                    current_client_authentication_->Start();
                }
            }
        current_client_authentication_cs_.Leave();
    }
}

void Server::AllowConnections(bool allow_connections)
{
    if (allow_connections_ != allow_connections)
    {
        allow_connections_ = allow_connections;
        if(allow_connections_)
        {
            for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
            {
                connection_request_handlers_->at(i)->Resume();
            }
        }
        else
        {
            for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
            {
                connection_request_handlers_->at(i)->Suspend();
            }
        }
    }
}

void Server::SocketClientAdded(NetworkSocket *clientSocket)
{
    bool added = false;
    client_connections_cs_.Enter();
    
    bool have_active_connection = false;
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        if (client_connections_->at(i)->IsActive())
        {
            have_active_connection = true;
            break;
        }
    }

    if (!have_active_connection)
    {
        ClientConnection *new_client_connection = new ClientConnection(this, clientSocket);
        client_connections_->push_back(new_client_connection);
        devices_changed_ = true;
        added = true;
    }
    else
    {
        AuthenticationFailedMessage authentication_failed(AFR_AnotherClient);
        const CommunicationBuffer *communication_buffer = authentication_failed.GetCommunicationBuffer();
        clientSocket->Send(communication_buffer->GetBuffer(), communication_buffer->GetSize());

        SAFE_DELETE(clientSocket);
    }

    client_connections_cs_.Leave();
    
    if(added)
    {
        SendElevatedPrivilegesMessage(current_window_is_elevated_);
        SendLogonInfo(is_at_logon_);
    }
}

void Server::Suspend()
{
    //Cancel authentication
    CancelAuthenticationRequest();

    // Close any current connections
    client_connections_cs_.Enter();
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        ClientConnection *client_connection = (*client_connections_)[i];
        client_connection->Disconnect(true);
    }
    client_connections_cs_.Leave();

    Uninit();
}

void Server::Resume()
{
    Init(store_id_, server_identity_);
}

void Server::Shutdown()
{
    // Execute the same code as the suspend function
    Suspend();
}

void Server::CancelAuthenticationRequest()
{
    current_client_authentication_cs_.Enter();
        if (current_client_authentication_ == NULL)
        {
            GCSOutputDebugString(L"Server::ClearAuthenticationRequest - client authentication request is already cancelled");
        }
        else
        {
            current_client_authentication_->Cancel();
            SAFE_DELETE(current_client_authentication_);
        }
    current_client_authentication_cs_.Leave();
}

void Server::OnAuthenticationCanceled()
{
    current_client_authentication_cs_.Enter();
        if (current_client_authentication_ == NULL)
        {
            GCSOutputDebugString(L"Server::OnAuthenticationCanceled - client authentication request is already cancelled");
        }
        else
        {
            current_client_authentication_->OnCancel();
            SAFE_DELETE(current_client_authentication_);
        }
    current_client_authentication_cs_.Leave();
}

void Server::SendServerInfo(ClientConnection *connection)
{
    if (connection != NULL)
    {
        if(wakeable_mac_addresses_.size() > 0)
        {
            ServerInfoMessage server_info_message(wakeable_mac_addresses_, server_identity_);
            connection->SendMessage(&server_info_message);
        }
        else
        {
            ServerInfoMessage server_info_message;
            connection->SendMessage(&server_info_message);
        }
    }
}

void Server::ProcessMessage(ClientConnection *connection, const InputMessage *message, bool &exitThread)
{
    exitThread = false;

    if (message != NULL && message->IsValid())
    {
        switch (message->GetType())
        {
            case InputMessage::kKeyboardEventMessage:
            {
                const KeyboardEventMessage *keyboard_message = dynamic_cast<const KeyboardEventMessage *>(message);
                if (keyboard_message != NULL)
                {
                    UICallbacks::SendKeyboardEventMessage(keyboard_message, input_handler_fd_);
                }
                break;
            }
            case InputMessage::kMouseMoveEventMessage:
            {
                const MouseMoveEventMessage *mouse_move_message = dynamic_cast<const MouseMoveEventMessage *>(message);
                if (mouse_move_message != NULL)
                {
                    UICallbacks::SendMouseMoveEventMessage(mouse_move_message, input_handler_fd_);
                }
                break;
            }
            case InputMessage::kMouseButtonEventMessage:
            {
                const MouseButtonEventMessage *mouse_button_message = dynamic_cast<const MouseButtonEventMessage *>(message);
                if (mouse_button_message != NULL)
                {
                    UICallbacks::SendMouseButtonEventMessage(mouse_button_message, input_handler_fd_);
                }
                break;
            }
            case InputMessage::kMouseScrollEventMessage:
            {
                const MouseScrollEventMessage *mouse_scroll_message = dynamic_cast<const MouseScrollEventMessage *>(message);
                if (mouse_scroll_message != NULL)
                {
                    UICallbacks::SendMouseScrollEventMessage(mouse_scroll_message, input_handler_fd_);
                }
                break;
            }
            case InputMessage::kUnicharEventMessage:
            {
                const UnicharEventMessage *unichar_event_message = dynamic_cast<const UnicharEventMessage *>(message);
                if (unichar_event_message != NULL)
                {
                    UICallbacks::SendUnicharEventMessage(unichar_event_message, input_handler_fd_);
                }
                break;
            }
            case InputMessage::kDisconnectMessage:
            {
                const DisconnectMessage *disconnect_message = dynamic_cast<const DisconnectMessage *>(message);
                if (disconnect_message != NULL)
                {
                    exitThread = true;
                }
                break;
            }
            case InputMessage::kDockInfoMessage:
            {
                const DockInfoMessage *dock_info_message = dynamic_cast<const DockInfoMessage *>(message);
                if (dock_info_message != NULL && connection != NULL)
                {
                    connection->SetDockInfo(dock_info_message->GetDockInfo());
                }
                break;
            }
            case InputMessage::kSuspendGCCMessage:
            {
                const SuspendGCCMessage *suspend_gcc_message = dynamic_cast<const SuspendGCCMessage *>(message);
                if (suspend_gcc_message != NULL && connection != NULL)
                {
                    connection->Suspend();
                    exitThread = true;
                }
                break;
            }
            case InputMessage::kWindows8CommandMessage:
            {
                break;
            }
            case InputMessage::kControlPCMessage:
            {
                const ControlPCMessage *control_pc_message = dynamic_cast<const ControlPCMessage *>(message);
                if(control_pc_message != NULL)
                {
                    control_pc_message->SetMouseVisibility();
                }
                break;
            }
            case InputMessage::kClientInfoMessage:
            {
                const ClientInfoMessage *client_info_message = dynamic_cast<const ClientInfoMessage *>(message);
                if(client_info_message != NULL)
                {
                    ClientDevice *client_device = client_info_message->GetClientDeviceFromInfo();

                    client_device->AddProperty(L"StoreId", StringFromGUID(store_id_));
                    client_device->AddProperty(L"ServerIdentity", StringFromGUID(server_identity_));

                    std::wstring last_connect_string;
                    std::wstringstream last_connect_stream;
                    last_connect_stream << connection->GetConnectionTime();
                    last_connect_stream >> last_connect_string;

                    client_device->AddProperty(L"LastConnected", last_connect_string); //TODO: Get a proper connected value and update

                    client_device->SetClientConnection(connection);

                    client_device->SetCanConnect(!do_not_disturb_);

                    SendVolumeLevelMessage(connection);
                    SendMuteStateMessage(connection);

                    //TODO: Pairing ID

                    client_device_manager_->AddKnownDevice(client_device);

                    client_device->Release();

                    QueueAuthenticationRequest(connection);
                }
                break;
            }
            case InputMessage::kPasscodeMessage:
            {
                const PasscodeMessage *passcode_message = dynamic_cast<const PasscodeMessage *>(message);
                current_client_authentication_cs_.Enter();
                    if(current_client_authentication_ != NULL && passcode_message != NULL)
                    {
                        current_client_authentication_->TryPasscode(passcode_message->GetPasscode());
                    }
                current_client_authentication_cs_.Leave();
                break;
            }
            case InputMessage::kVolumeChangedMessage:
            {
                const MediaVolumeChangedInputMessage *volume_message = dynamic_cast<const MediaVolumeChangedInputMessage *>(message);
                float volume_level = volume_message->GetVolumeLevel() / 100.0f;

                SetVolumeLevel(volume_level);
                break;
            }
            case InputMessage::kPowerOptionMessage:
            {
                const PowerOptionInputMessage *power_option_message = dynamic_cast<const PowerOptionInputMessage *>(message);
                UICallbacks::HandlePowerOption(power_option_message);
                break;
            }
            default:
            {
                GCSOutputDebugString(L"Unknown message type received in Server (%d)\n", message->GetType());
                break;
            }
        }
    }
}

void Server::AllowConnection(bool remember)
{
    current_client_authentication_cs_.Enter();
        if (current_client_authentication_ == NULL)
        {
            GCSOutputDebugString(L"Server::AllowConnection - No client is being authenticated at this time.  Ignoring the Allow call.\n");
        }
        else
        {
            current_client_authentication_->Allow(remember);
            SAFE_DELETE(current_client_authentication_);
        }
    current_client_authentication_cs_.Leave();
}

InputMessageFactory *Server::GetInputMessageFactory()
{
    return input_message_factory_;
}

void Server::UpdateClientConnections()
{
    update_stop_watch_->Stop();
    float elapsed_time = update_stop_watch_->GetElapsedSeconds();
    update_stop_watch_->Start();

    bool connected = false;
    bool refresh_list = false;
    bool allow_connection = true;

    CheckForVolumeChange();
    CheckForMuteChange();
    CheckForElevatedPrivilegeChange();

    client_connections_cs_.Enter();
    std::vector<ClientConnection *>::iterator iter = client_connections_->begin();
    while(iter != client_connections_->end())
    {
        ClientConnection *client_connection = *iter;
        client_connection->Update(elapsed_time);

        // IsConnected() ensures that we have finished the connection process (in between states return false)
        if (client_connection->IsConnected())
        {
            connected = true;
        }

        if (client_connection->IsConnecting() || client_connection->IsConnected())
        {
            allow_connection = false;
        }

        // IsDisconnected() ensures that we have cleaned up the connection (closing, pending and intermediate states return false)
        if (!client_connection->IsDisconnected() && !client_connection->IsSuspended())
        {
            iter++;
            continue;
        }

        iter = client_connections_->erase(iter);
        devices_changed_ = true;
        GCSOutputDebugString(L"Server::Update removed Client Connection 0x%p\n", client_connection);

        if(current_client_authentication_ != NULL && current_client_authentication_->HasClientConnection(client_connection))
        {
            GCSOutputDebugString(L"Server::Update ClientAuthentication has connection being removed\n");
            UICallbacks::OnAuthenticationCanceled();
            SAFE_DELETE(current_client_authentication_);
        }
        else
        {
            GCSOutputDebugString(L"Server::Update ClientAuthentication does not have connection being removed\n");
        }

        ClientDevice *connected_device = client_device_manager_->DeviceWithConnection(client_connection);

        if(connected_device != NULL)
        {
            connected_device->SetClientConnection(NULL);
            connected_device->SetConnected(false);
        }

        SAFE_DELETE(client_connection);
    }

    refresh_list = devices_changed_;
    devices_changed_ = false;
    client_connections_cs_.Leave();

    client_device_manager_->SetDeviceConnected(connected && !do_not_disturb_);
    client_device_manager_->Update(elapsed_time);

    if (refresh_list)
    {
        UICallbacks::RefreshDeviceList();

        if (connected)
        {
            UICallbacks::OnDeviceConnected();
            SendElevatedPrivilegesMessage(GetElevatedPrivilegeState());
            SendLogonInfo(is_at_logon_);
        }
        else
        {
            UICallbacks::OnDeviceDisconnected();
        }
    }

    AllowConnections(allow_connection);

    std::vector<NetworkSocket *> new_sockets;

    if(client_connections_->empty())
    {
        CreateRequestHandlers(); //Updates the handlers to correspond to the computer's active network connections
    }

    bool current_network_connection = !connection_request_handlers_->empty();
    if (has_network_connection_ != current_network_connection)
    {
        if (!current_network_connection)
        {
            UICallbacks::OnNoNetworkConnection();
        }
        else
        {
            UICallbacks::OnDeviceDisconnected();
        }
        has_network_connection_ = current_network_connection;
    }

    for (unsigned int i = 0; i < connection_request_handlers_->size(); ++i)
    {
        connection_request_handlers_->at(i)->Update();
    }
    
    time_t now = time(0);
    if(now - last_input_heartbeat_time_ > 5)
    {
        //Send heartbeat message to the input handler
        char message = 'H';
        write(input_handler_fd_, &message, sizeof(message));
        last_input_heartbeat_time_ = now;
    }
}

void Server::SetDoNotDisturb(bool do_not_disturb)
{
    do_not_disturb_ = do_not_disturb;
    client_device_manager_->AllowConnections(do_not_disturb);
}

int Server::GetNumKnownDevices() const
{
    return client_device_manager_->GetNumKnownDevices();
}

ClientDevice * Server::GetKnownDevice(int index)
{
    return client_device_manager_->GetKnownDevice(index);
}

void Server::ForgetDevice(ClientDevice *device)
{
    if (device != NULL)
    {
        if (device->IsConnected())
        {
            // Close any current connections
            client_connections_cs_.Enter();
            for (unsigned int i = 0; i < client_connections_->size(); ++i)
            {
                ClientConnection *client_connection = (*client_connections_)[i];
                client_connection->Disconnect(true);
            }
            client_connections_cs_.Leave();
        }

        client_device_manager_->ForgetDevice(device);
    }
}

void Server::ForgetDevice(const std::wstring &device_id)
{
    ForgetDevice(client_device_manager_->DeviceWithID(device_id));
}

void Server::ForgetAllDevices()
{
    // Close any current connections
    client_connections_cs_.Enter();
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        ClientConnection *client_connection = (*client_connections_)[i];
        client_connection->Disconnect(true);
    }
    client_connections_cs_.Leave();

    client_device_manager_->ForgetAllDevices();
}

void Server::CreateRequestHandlers()
{
    struct ifaddrs *if_addrs = NULL;
    struct ifaddrs *ifa = NULL;
    void *temp_address_pointer = NULL;

    getifaddrs(&if_addrs);

    std::vector<IPAddress *> valid_network_addresses;

    for(ifa = if_addrs; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(!ifa->ifa_addr)
        {
            continue;
        }

        if(ifa->ifa_addr->sa_family == AF_INET)
        {
            temp_address_pointer = &(reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr)->sin_addr);
            BYTE ip[4];
            MemCopy(ip, sizeof(ip), temp_address_pointer, 4);

            temp_address_pointer = &(reinterpret_cast<struct sockaddr_in *>(ifa->ifa_netmask)->sin_addr);
            BYTE netmask[4];
            MemCopy(netmask, sizeof(netmask), temp_address_pointer, 4);

            if(!( ip[0] == 0 || ip[0] == 127 || ip[0] == 169)) //Skip any with address 0.0.0.0, or local host, or a bad connection
            {
                valid_network_addresses.push_back(new IPAddress(ip, 0, netmask));
            }
        }
    }
    
    //Use valid_network_addresses to update the current request handlers
    //First remove any that are no longer valid
    for(size_t handler_index = connection_request_handlers_->size() - 1; handler_index >= 0 && handler_index < connection_request_handlers_->size(); --handler_index)
    {
        ConnectionRequestHandler *handler = connection_request_handlers_->at(handler_index);
        bool found = false;
        for(size_t address_index = 0; address_index < valid_network_addresses.size(); ++address_index)
        {
            const IPAddress *address = valid_network_addresses[address_index];
            if(handler->HasAddress(address))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            //Request Handler is no longer valid            
            handler->Suspend();
            SAFE_DELETE(handler);
            connection_request_handlers_->erase(connection_request_handlers_->begin() + handler_index);
        }
    }

    //Add a new handler for each address that doesn't currently match a handler
    for(size_t address_index = 0; address_index < valid_network_addresses.size(); ++address_index)
    {
        const IPAddress *address = valid_network_addresses[address_index];
        bool found = false;
        for(size_t handler_index = connection_request_handlers_->size() - 1; handler_index >= 0 && handler_index < connection_request_handlers_->size(); --handler_index)
        {
            ConnectionRequestHandler *handler = connection_request_handlers_->at(handler_index);
            if(handler->HasAddress(address))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            ConnectionRequestHandler *new_handler = new ConnectionRequestHandler(this, address);
            connection_request_handlers_->push_back(new_handler);
            new_handler->Resume();
        }
    }

    for(size_t address_index = 0; address_index < valid_network_addresses.size(); ++address_index)
    {
        IPAddress *address = valid_network_addresses[address_index];
        SAFE_DELETE(address);
    }
}

 void Server::GetKnownDevices(std::vector<BasicClientDevice *> &devices)
 {
     client_device_manager_->GetBasicClientDevices(devices);
 }

bool Server::GetName(std::wstring &name) const
{
    server_name_cs_.Enter();
    name = server_name_;
    server_name_cs_.Leave();
    return true;
}

unsigned int Server::GetNameLength() const
{
    int length = 0;
    server_name_cs_.Enter();
    length = (unsigned int)server_name_.length();
    server_name_cs_.Leave();
    return length;
}

const GUID Server::GetServerIdentity() const
{
    return server_identity_;
}

float Server::GetVolumeLevel() const
{
    return 1.0f;
}

void Server::SetVolumeLevel(float volume_level)
{
}

void Server::CheckForVolumeChange()
{
}

void Server::CheckForMuteChange()
{
}

bool Server::GetMuteState() const
{
    return false;
}

bool Server::GetElevatedPrivilegeState() const
{
    return false;
}

void Server::CheckForElevatedPrivilegeChange()
{
}

void Server::SendVolumeLevelMessage(ClientConnection* client_connection)
{
    MediaVolumeChangedOutputMessage message(current_volume_level_);
    client_connection->SendMessage(&message);
}

void Server::SendMuteStateMessage(ClientConnection* client_connection)
{
    MediaMuteChangedOutputMessage message(is_muted_);
    client_connection->SendMessage(&message);
}

void Server::SendElevatedPrivilegesMessage(bool is_elevated)
{
    current_window_is_elevated_ = is_elevated;

    // Tell the connected client(s) that the volume changed.
    ElevatedPrivilegesOutputMessage message(is_elevated);

    client_connections_cs_.Enter();
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        ClientConnection *client_connection = (*client_connections_)[i];
        if (client_connection->IsActive())
        {
            client_connection->SendMessage(&message);
        }
    }
    client_connections_cs_.Leave();
}

void Server::SendLogonInfo(bool is_at_logon)
{
    is_at_logon_ = is_at_logon;

    LogonInfoOutputMessage message(is_at_logon);

    client_connections_cs_.Enter();
    for (unsigned int i = 0; i < client_connections_->size(); ++i)
    {
        ClientConnection *client_connection = (*client_connections_)[i];
        if (client_connection->IsActive())
        {
            client_connection->SendMessage(&message);
        }
    }
    client_connections_cs_.Leave();
}

void Server::SendCurrentStatus()
{
    if(connection_request_handlers_->size() == 0)
    {
        UICallbacks::OnNoNetworkConnection();
        return;
    }

    client_connections_cs_.Enter();
    bool connected = client_connections_->size() != 0;
    client_connections_cs_.Leave();

    if(connected)
    {
        UICallbacks::OnDeviceConnected();
    }
    else
    {
        UICallbacks::OnDeviceDisconnected();
    }
}

void Server::Show(bool show)
{
    is_visible_ = show;

    should_show_ = false;
    should_hide_ = false;
}

bool Server::IsVisible() const
{
    return is_visible_;
}

bool Server::ShouldShow() const
{
    return should_show_;
}

void Server::SetShouldShow(bool show)
{
    should_show_ = show;
}

void Server::SetShouldHide(bool hide)
{
    should_hide_ = hide;
}

bool Server::ShouldHide() const
{
    return should_hide_;
}
// static
void Server::GetServerName(std::wstring &name)
{
    if (GetUserGCSName(name))
    {
        return;
    }

    if (GetComputerDescriptionName(name))
    {
        return;
    }

    if (GetHostName(name))
    {
        return;
    }

    name = kDefaultGCSName;
}

// static
void Server::SetDNDFromOptions()
{
    wchar_t filename[MAX_PATH] = L"/var/lib/RemoteKeyboard/Options.xml";
    const int dir_err = mkdir("/var/lib/RemoteKeyboard/", S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH | S_IXOTH);

    pugi::xml_document options_doc;

    if(!options_doc.load_file(filename))
    {
        return;
    }

    do_not_disturb_ = options_doc.child(L"Options").attribute(L"bDNDMode").as_bool();
}

// static
bool Server::GetUserGCSName(std::wstring &name)
{
    wchar_t filename[MAX_PATH] = L"/var/lib/RemoteKeyboard/Options.xml";
    const int dir_err = mkdir("/var/lib/RemoteKeyboard/", S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH | S_IXOTH);

    pugi::xml_document options_doc;

    if(!options_doc.load_file(filename))
    {
        return false;
    }

    name = options_doc.child(L"Options").attribute(L"FriendlyName").as_string();
    return true;
}

// static
bool Server::GetComputerDescriptionName(std::wstring &name)
{
    return false;
}

// static
bool Server::GetHostName(std::wstring &name)
{
    //Read from /etc/hostname
    std::wifstream hostnameFile("/etc/hostname", std::wifstream::in);
    if(hostnameFile.is_open())
    {
        hostnameFile.seekg(0, std::ios::end);
        name.reserve(hostnameFile.tellg());
        hostnameFile.seekg(0, std::ios::beg);
        name.assign(std::istreambuf_iterator<wchar_t>(hostnameFile), std::istreambuf_iterator<wchar_t>());
        hostnameFile.close();
        name.erase(name.end()-1);
        GCSOutputDebugString(L"Found hostname %ls\n", name.c_str());
        return true;
    }
    else
    {
        GCSOutputDebugString(L"Failed to open /etc/hostname\n");
        return false;
    }
}

int Server::StartLinuxInputHandler()
{
    pid_t pid;
    int pipe_descriptors[2];
    
    //setup pipeline
    if(pipe(pipe_descriptors))
    {
        //could not create pipes
        GCSOutputDebugString(L"Error creating pipe for input handler: %d\n", errno);
        return -1;
    }
    
    //Fork
    pid = fork();
    if(pid == -1)
    {
        //could not fork
        GCSOutputDebugString(L"Error forking for input handler: %d\n", errno);
        return -1;
    }
    
    if(pid == 0)
    {
        //is child process
        
        GCSOutputDebugString(L"In InputHandler fork: %d\n", pipe_descriptors[0]);
        close(pipe_descriptors[1]); //close unused side of the pipe
        
        char fd_string[12];
        sprintf(fd_string, "%d", pipe_descriptors[0]);
        
        //replace the child fork with the Input Handler
        if(execl("InputHandler","InputHandler", fd_string, NULL) == -1)
        {
            //failed
            exit(1);
        }
    }
    else
    {
        //is parent process
        usleep(100000); //sleep 0.1 seconds to give child process time to start
        close(pipe_descriptors[0]); //close unused side of the pipe
        
        time_t now = time(0);
        last_input_heartbeat_time_ = now;
    }
    
    return pipe_descriptors[1];
}
