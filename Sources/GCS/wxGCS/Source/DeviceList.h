#ifndef DEVICE_LIST_H_
#define DEVICE_LIST_H_

#include <wx/wx.h>
#include "Constants.h"
#include "DeviceEntry.h"
#include <vector>
#include "../../GCSNativeInterface/UICallbacks.h"
#include "../../GCSNativeInterface/CriticalSection.h"

class ForgetDevicePopup;
class AuthenticationPopup;
class BasicClientDevice;
class Server;

class DeviceList : public wxScrolled<wxWindow>, public UIListener
{
public:
    DeviceList(wxWindow *parent);
    virtual ~DeviceList();
    void InitPopulate();
    void AddEntry(DeviceEntry *entry);
    void RemoveEntry(DeviceEntry *entry);
    void Arrange();
    std::vector<DeviceEntry*> GetDevices();
    void DismissAuthenticationPopup();
    Server *GetServer() const;

    virtual void DisplayAuthenticationCallback(const std::wstring &passcode);
    virtual void HideAuthenticationCallback();
    virtual void OnCanceledAuthenticationCallback();
    virtual void RefreshDeviceListCallback();

    virtual void DeviceConnectedCallback();
    virtual void DeviceDisconnectedCallback();
    virtual void NoNetworkConnectionCallback();

    void DisplayAuthenticationHandler();
    void HideAuthenticationHandler();
    void OnCanceledAuthenticationHandler();
    void RefreshDeviceListHandler();

    bool GetShouldShowAuthentication();
    bool GetShouldHideAuthentication();
    bool GetShouldCancelAuthentication();
    bool GetShouldRefreshDeviceList();

    void SetShouldShowAuthentication(bool should_show_authentication_screen);
    void SetShouldHideAuthentication(bool should_hide_authentication_screen);
    void SetShouldCancelAuthentication(bool should_cancel_authentication);
    void SetShouldRefreshDeviceList(bool should_refresh_device_list);
    
    virtual void SetKnownDevices(std::vector<BasicClientDevice *> devices);

    void LockDevices();
    void UnlockDevices();
    virtual int GetNumKnownDevices();
    virtual BasicClientDevice *GetKnownDevice(int index);
    virtual void ForgetDevice(const std::wstring &device_id);
    virtual void CancelAuthenticationRequest();
    virtual void AllowConnection(bool allow);
    virtual void SendOnAuthenticationCanceled();
    virtual void SetDoNotDisturb(bool dnd);
    virtual void SetAutoStart(bool auto_start);
    virtual void SetServerName(std::wstring &name);
    
    virtual bool GetServerName(std::wstring &name);

    bool IsStartMenuVisible();

protected:
    void OnEntryClicked(wxCommandEvent &event);
    void OnMouseWheel(wxMouseEvent &event);
    void DismisForgetDevicePopup();
    
    std::vector<BasicClientDevice *> known_devices_;

private:
    DISALLOW_COPY_AND_ASSIGN(DeviceList);

    std::vector<DeviceEntry*>       devices_;
    std::vector<wxStaticBitmap*>    dividers_;
    wxBitmap                        device_entry_image_;
    wxBitmap                        device_divider_image_;
    ForgetDevicePopup               *forget_device_popup_;
    AuthenticationPopup             *authentication_popup_;
    std::wstring                    passcode_;
    bool                            should_show_authentication_screen_;
    bool                            should_hide_authentication_screen_;
    bool                            should_cancel_authentication_;
    bool                            should_refresh_device_list_;
    static const int                kMinBelowIndex = 4;

    bool 			    did_minimize_all_apps_ = false;

    CriticalSection known_devices_cs_;

    DECLARE_EVENT_TABLE();
};

#endif //DEVICE_LIST_H_
