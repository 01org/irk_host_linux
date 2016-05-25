#include "StdAfx.h"
#include "AuthenticationFailedMessage.h"
#include "AuthenticationRequestMessage.h"
#include "AuthenticationSuccessMessage.h"
#include "AuthenticationWrongPasscodeMessage.h"
#include "ClientAuthentication.h"
#include "ClientConnection.h"
#include "ClientDeviceManager.h"
#include "UICallbacks.h"
#include "Utilities.h"
#include <sstream>

// static
ClientAuthentication *ClientAuthentication::s_current_client_authentication = NULL;

ClientAuthentication::ClientAuthentication(ClientConnection *client_connection, ClientDeviceManager *device_manager)
    : client_connection_(client_connection)
    , device_manager_(device_manager)
{
}

// virtual
ClientAuthentication::~ClientAuthentication()
{
    Cleanup();
}

unsigned long Xorshift128() // a period of 2 power 128 - 1
{    
  static time_t x = time(0);  //123456789;
  static time_t y = 362436069;
  static time_t z = 521288629;
  static time_t w = 88675123;
  time_t t;
  
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  w = w ^ (w >> 19) ^ (t ^ (t >> 8));
  return (unsigned long)(0xffffffff & w);
}
 
char XorshiftRand(char min,char max)
{
    max++;
    return ((char)(Xorshift128()%(max-min))+min);
}

void ClientAuthentication::Start()
{
    if (s_current_client_authentication != NULL)
    {
        GCSOutputDebugString(L"ClientAuthentication::Start - Cannot start the authentication process if another client is being authenticated");
        return;
    }

    if (client_connection_ == NULL)
    {
        GCSOutputDebugString(L"ClientAuthentication::Start - ClientConnection is NULL");
        return;
    }
    
    //generate a passcode which is 6 random digits
    std::wstringstream passcode_ss;
    for(int i = 0; i < 6; ++i)
    {
        passcode_ss << XorshiftRand('0', '9'); //A random character in the range ['0', '9']
    }

    passcode_ss >> passcode_;
    
    AuthenticationRequestMessage authentication_request;
    client_connection_->SendMessage(&authentication_request);

    //reset attempt count
    attempts_ = 0;

    UICallbacks::DisplayAuthentication(passcode_);

    s_current_client_authentication = this;
}

void ClientAuthentication::Cancel()
{
    AuthenticationFailedMessage authentication_failed(AFR_Cancel);
    client_connection_->SendMessage(&authentication_failed);

    Cleanup();
}

void ClientAuthentication::Allow(bool remember)
{
    UNREFERENCED_PARAMETER(remember);

    //Set authenticated

    device_manager_->SetAuthenticated(client_connection_);

    Cleanup();
}

void ClientAuthentication::Cleanup()
{
    if (!IsInProgress())
    {
        return;
    }

    s_current_client_authentication = NULL;

    passcode_.clear();
}

void ClientAuthentication::OnCancel()
{
   Cleanup();
}

bool ClientAuthentication::IsInProgress() const
{
    return (s_current_client_authentication == this);
}

bool ClientAuthentication::HasClientConnection(const ClientConnection *client_connection) const
{
    return client_connection_ == client_connection;
}

//virtual
void ClientAuthentication::TryPasscode(const std::wstring &passcode)
{
    if(passcode_ == passcode)
    {
        //passcode is correct
        AuthenticationSuccessMessage authentication_success;
        client_connection_->SendMessage(&authentication_success);
        UICallbacks::HideAuthentication();
    }
    else
    {
        attempts_++;
        if(attempts_ >= 3)
        {
            AuthenticationFailedMessage authentication_failed(AFR_Cancel);
            client_connection_->SendMessage(&authentication_failed);
            UICallbacks::OnAuthenticationCanceled();
        }
        else
        {
            AuthenticationWrongPasscodeMessage authentication_wrong_passcode;
            client_connection_->SendMessage(&authentication_wrong_passcode);
        }
    }
}
