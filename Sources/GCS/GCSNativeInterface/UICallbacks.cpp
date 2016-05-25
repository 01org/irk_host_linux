#include "StdAfx.h"
#include "UICallbacks.h"

#include "KeyboardEventMessage.h"
#include "MouseButtonEventMessage.h"
#include "MouseMoveEventMessage.h"
#include "MouseScrollEventMessage.h"
#include "PowerOptionInputMessage.h"
#include "UnicharEventMessage.h"

using namespace UICallbacks;

namespace
{
    static UIListener *g_listener = NULL;
}

void UICallbacks::SetUIListener(UIListener *listener)
{
    g_listener = listener;
}

void UICallbacks::DisplayAuthentication(const std::wstring &passcode)
{
    if (g_listener != NULL)
    {
        g_listener->DisplayAuthenticationCallback(passcode);
    }
}

void UICallbacks::HideAuthentication()
{
    if (g_listener != NULL)
    {
        g_listener->HideAuthenticationCallback();
    }
}

void UICallbacks::OnAuthenticationCanceled()
{
    if (g_listener != NULL)
    {
        g_listener->OnCanceledAuthenticationCallback();
    }
}

void UICallbacks::RefreshDeviceList()
{
    if (g_listener != NULL)
    {
        g_listener->RefreshDeviceListCallback();
    }
}

void UICallbacks::SendKeyboardEventMessage(const KeyboardEventMessage *message, int input_handler_fd)
{
    if (g_listener != NULL)
    {
        g_listener->SendKeyboardEventMessage(message, input_handler_fd);
    }
}

void UICallbacks::SendUnicharEventMessage(const UnicharEventMessage *message, int input_handler_fd)
{
    if (g_listener != NULL)
    {
        g_listener->SendUnicharEventMessage(message, input_handler_fd);
    }
}

void UICallbacks::SendMouseButtonEventMessage(const MouseButtonEventMessage *message, int input_handler_fd)
{
    if (g_listener != NULL)
    {
        g_listener->SendMouseButtonEventMessage(message, input_handler_fd);
    }
}

void UICallbacks::SendMouseMoveEventMessage(const MouseMoveEventMessage *message, int input_handler_fd)
{
    if (g_listener != NULL)
    {
        g_listener->SendMouseMoveEventMessage(message, input_handler_fd);
    }
}

void UICallbacks::SendMouseScrollEventMessage(const MouseScrollEventMessage *message, int input_handler_fd)
{
    if (g_listener != NULL)
    {
        g_listener->SendMouseScrollEventMessage(message, input_handler_fd);
    }
}

void UICallbacks::OnDeviceConnected()
{
    if (g_listener != NULL)
    {
        g_listener->DeviceConnectedCallback();
    }
}

void UICallbacks::OnDeviceDisconnected()
{
    if (g_listener != NULL)
    {
        g_listener->DeviceDisconnectedCallback();
    }
}

void UICallbacks::OnNoNetworkConnection()
{
    if (g_listener != NULL)
    {
        g_listener->NoNetworkConnectionCallback();
    }
}

void UICallbacks::HandlePowerOption(const PowerOptionInputMessage *message)
{
    if (g_listener != NULL)
    {
        g_listener->PowerOptionCallback(message);
    }
}

//virtual
void UIListener::SendKeyboardEventMessage(const KeyboardEventMessage *message, int input_handler_fd)
{
    message->SendKeyboardEvent(input_handler_fd);
}

//virtual
void UIListener::SendUnicharEventMessage(const UnicharEventMessage *message, int input_handler_fd)
{
    message->SendUnicharEvent(input_handler_fd);
}

//virtual
void UIListener::SendMouseButtonEventMessage(const MouseButtonEventMessage *message, int input_handler_fd)
{
    message->SendMouseButtonEvent(input_handler_fd);
}

//virtual
void UIListener::SendMouseMoveEventMessage(const MouseMoveEventMessage *message, int input_handler_fd)
{
    message->SendMouseMoveEvent(input_handler_fd);
}

//virtual
void UIListener::SendMouseScrollEventMessage(const MouseScrollEventMessage *message, int input_handler_fd)
{
    message->SendMouseScrollEvent(input_handler_fd);
}

//virtual
void UIListener::PowerOptionCallback(const PowerOptionInputMessage *message)
{
    message->HandlePowerOption(false);
}
