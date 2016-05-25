#ifndef UI_CALLBACKS_H_
#define UI_CALLBACKS_H_

#define __stdcall 

#include <string>

class KeyboardEventMessage;
class MouseButtonEventMessage;
class MouseMoveEventMessage;
class MouseScrollEventMessage;
class UnicharEventMessage;
class PowerOptionInputMessage;

class UIListener
{
public:
        virtual void DisplayAuthenticationCallback(const std::wstring &passcode) = 0;
        virtual void HideAuthenticationCallback() = 0;
        virtual void OnCanceledAuthenticationCallback() = 0;
        virtual void RefreshDeviceListCallback() = 0;
        virtual void SendKeyboardEventMessage(const KeyboardEventMessage *message, int input_handler_fd);
        virtual void SendUnicharEventMessage(const UnicharEventMessage *message, int input_handler_fd);
        virtual void SendMouseButtonEventMessage(const MouseButtonEventMessage *message, int input_handler_fd);
        virtual void SendMouseMoveEventMessage(const MouseMoveEventMessage *message, int input_handler_fd);
        virtual void SendMouseScrollEventMessage(const MouseScrollEventMessage *message, int input_handler_fd);
        virtual void DeviceConnectedCallback() = 0;
        virtual void DeviceDisconnectedCallback() = 0;
        virtual void NoNetworkConnectionCallback() = 0;
        virtual void PowerOptionCallback(const PowerOptionInputMessage *message);
};

namespace UICallbacks
{
    // Define all the callback signatures
#ifdef __cplusplus
    extern "C"
    {
#endif
        typedef void(__stdcall * DisplayAuthenticationCallback)(const std::wstring &passcode);
        typedef void(__stdcall * HideAuthenticationCallback)();
        typedef void(__stdcall * OnCanceledAuthenticationCallback)();
        typedef void(__stdcall * RefreshDeviceListCallback)();
        typedef void(__stdcall * DeviceConnectedCallback)();
        typedef void(__stdcall * DeviceDisconnectedCallback)();
        typedef void(__stdcall * NoNetworkConnectionCallback)();
#ifdef __cplusplus
    }
#endif

    void SetUIListener(UIListener *listener);
   
    // Expose functions to the GCSNativeInterface to call the various callbacks
    void DisplayAuthentication(const std::wstring &passcode);
    void HideAuthentication();
    void OnAuthenticationCanceled();
    void RefreshDeviceList();
    void OnDeviceConnected();
    void OnDeviceDisconnected();
    void OnNoNetworkConnection();
    void SendKeyboardEventMessage(const KeyboardEventMessage *message, int input_handler_fd = -1);
    void SendUnicharEventMessage(const UnicharEventMessage *message, int input_handler_fd = -1);
    void SendMouseButtonEventMessage(const MouseButtonEventMessage *message, int input_handler_fd = -1);
    void SendMouseMoveEventMessage(const MouseMoveEventMessage *message, int input_handler_fd = -1);
    void SendMouseScrollEventMessage(const MouseScrollEventMessage *message, int input_handler_fd = -1);
    void HandlePowerOption(const PowerOptionInputMessage *message);
}

#endif // UI_CALLBACKS_H_
