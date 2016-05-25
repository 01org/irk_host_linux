#include "StdAfx.h"
#include "ClientConnection.h"
#include "ClientConnectionMessageThread.h"
#include "HeartbeatInputMessage.h"
#include "HeartbeatOutputMessage.h"
#include "InputMessage.h"
#include "InputMessageBuilder.h"
#include "InputMessageFactory.h"
#include "NetworkSocket.h"
#include "OutputMessage.h"
#include "ServerDisconnectMessage.h"
#include "UICallbacks.h"
#include "Server.h"
#include "macros.h"

static const float kHeartbeatSendInterval = 1.0f;
static const float kHeartbeatReceiveInterval = 15.0f;

ClientConnection::ClientConnection(Server *server, NetworkSocket *socket)
    : server_(server)
    , current_message_thread_(NULL)
    , state_(ClientConnection::PendingConnection)
    , suspended_(false)
    , current_dock_info()
    , time_since_last_heartbeat_send_(0.0f)
    , time_since_last_heartbeat_receive_(0.0f)
    , socket_(socket)
    , connection_time_(0)
{
    //Connection is already established when this is created
    OnConnectionComplete();
}

// virtual
ClientConnection::~ClientConnection()
{
    SAFE_DELETE(current_message_thread_);
    DestroyConnection();
}

bool ClientConnection::SendMessage(OutputMessage *message)
{
    if (message == NULL)
    {
        GCSOutputDebugString(L"SocketClientConnection::SendMessage - Message is NULL\n");
        return true;    // We assume that sending nothing is always successful
    }

    const CommunicationBuffer *communication_buffer = message->GetCommunicationBuffer();
    int num_bytes_written = DataStreamWrite(communication_buffer->GetBuffer(), 0, communication_buffer->GetSize());

    if (num_bytes_written != static_cast<int>(communication_buffer->GetSize()))
    {
        GCSOutputDebugString(L"SocketClientConnection::SendMessage - Number of bytes sent (%d) is different than the number of bytes in the message (%d)\n", num_bytes_written, communication_buffer->GetSize());
        return false;
    }
    return true;
}

void ClientConnection::OnConnectionComplete()
{
    // Create a new thread to run the receive message loop
    if (current_message_thread_ == NULL)
    {
        current_message_thread_ = new ClientConnectionMessageThread(this, server_->GetInputMessageFactory(), socket_);
    }
    else
    {
        GCSOutputDebugString(L"SocketClientConnection::OnConnectionComplete() : There is an existing thread, the thread should be null at this point");
    }

    connection_time_ = time(NULL);

    state_ = ClientConnection::ConnectionComplete;
    suspended_ = false;

    server_->GetInputMessageFactory()->SetMessageBuilder(InputMessage::kHeartbeatMessage, TemplateMessageBuilder<HeartbeatInputMessage>);
    SendHeartbeat();
    time_since_last_heartbeat_receive_ = 0.0f;

    server_->SendServerInfo(this);
}

int ClientConnection::DataStreamWrite(const BYTE *buffer, int offset, int length)
{
    if(socket_ == NULL)
    {
        return 0;
    }

    if(socket_->Send(buffer+offset, length))
    {
        return length;
    }
    else
    {
        if(!socket_->IsConnected())
        {
            Disconnect(false);
        }
    }
    
    return 0;
}

void ClientConnection::DestroyConnection()
{
    // Destroy the connection
    if (socket_ != NULL)
    {
        delete socket_;
        socket_ = NULL;
        state_ = ClientConnection::CloseComplete;
    }
}

void ClientConnection::ProcessMessage(InputMessage *message, bool &exitThread)
{
    if (message != NULL && message->GetType() == InputMessage::kHeartbeatMessage)
    {
        ReceiveHeartbeat();
    }
    else if (server_ != NULL)
    {
        server_->ProcessMessage(this, message, exitThread);
    }

    if(exitThread)
    {
        state_ = ClientConnection::CloseComplete;
    }
}

void ClientConnection::OnAuthorizationRequired()
{
    GCSOutputDebugString(L"OnAuthorizationRequired\n");

    server_->QueueAuthenticationRequest(this);
}

void ClientConnection::OnConnectionError(int error)
{
    UNREFERENCED_PARAMETER(error);

    // Clean up the connection
    Disconnect(false);

    GCSOutputDebugString(L"OnConnectionError\n");
}

void ClientConnection::Disconnect(bool send_message)
{
    GCSOutputDebugString(L"ClientConnection::Disconnect\n");
    if (send_message)
    {
        ServerDisconnectMessage *disconnect_message = new ServerDisconnectMessage();
        SendMessage(disconnect_message);
        SAFE_DELETE(disconnect_message);
    }

    if (current_message_thread_ != NULL)
    {
        state_ = ClientConnection::PendingMessageThreadDestroy;
        current_message_thread_->SignalDestroy();
    }
    else
    { 
        state_ = ClientConnection::CloseComplete;
    }
}

bool ClientConnection::IsConnecting() const
{
    return (state_ == PendingConnection);
}

bool ClientConnection::IsConnected() const
{
    return (state_ == ConnectionComplete);
}

bool ClientConnection::IsDisconnected() const
{
    return (state_ == ClientConnection::CloseComplete);
}

void ClientConnection::Suspend()
{
    suspended_ = true;
    Disconnect(false);
}

bool ClientConnection::IsSuspended() const
{
    return suspended_;
}

bool ClientConnection::IsActive() const
{
    return state_ <= ConnectionComplete;
}

void ClientConnection::Update(float seconds)
{
    UpdateHeartbeat(seconds);

    if (current_message_thread_ != NULL && current_message_thread_->IsDestroyed())
    {
        SAFE_DELETE(current_message_thread_);
        DestroyConnection();
        state_ = ClientConnection::CloseComplete;
    }
}

void ClientConnection::SetDockInfo(const ClientDockInfo &info)
{
    current_dock_info = info;
}

time_t ClientConnection::GetConnectionTime() const
{
    return connection_time_;
}

void ClientConnection::UpdateHeartbeat(float seconds)
{
    if (state_ == ConnectionComplete)
    {
        time_since_last_heartbeat_send_ += seconds;
        time_since_last_heartbeat_receive_ += seconds;

        if (time_since_last_heartbeat_receive_ > kHeartbeatReceiveInterval)
        {
            Disconnect(false);
        }
        else if (time_since_last_heartbeat_send_ > kHeartbeatSendInterval)
        {
            SendHeartbeat();
        }
    }
}

void ClientConnection::SendHeartbeat()
{
    HeartbeatOutputMessage *message = new HeartbeatOutputMessage();
    bool success = SendMessage(message);
    SAFE_DELETE(message);

    time_since_last_heartbeat_send_ = 0.0f;

    // The quickest way to detect if the client connection is no longer
    // valid is for us to gauge the success of sending a heartbeat message.
    // If we were unable to send the message, then we can assume that we
    // have been disconnected without waiting for the heartbeat reception
    // to time out.
    if (!success)
    {
        Disconnect(false);
    }
}

void ClientConnection::ReceiveHeartbeat()
{
    time_since_last_heartbeat_receive_ = 0.0f;
}
