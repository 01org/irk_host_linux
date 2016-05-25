#include <wx/event.h>
#include <algorithm>
#include "DeviceList.h"
#include "ForgetDevicePopup.h"
#include "AuthenticationPopup.h"
#include "GCSFrame.h"
#include "../../GCSNativeInterface/Server.h"
#include "../../GCSNativeInterface/BasicClientDevice.h"
#include "../../GCSNativeInterface/ClientDevice.h"
#include <wx/app.h> 
#include "main.h"

#include "../../GCSNativeInterface/GUID.h"

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(DeviceList, wxWindow)
     EVT_MOUSEWHEEL(DeviceList::OnMouseWheel)
END_EVENT_TABLE()

DeviceList::DeviceList(wxWindow *parent)
    : wxScrolled<wxWindow>(parent, wxID_ANY)
    , devices_()
    , dividers_()
    , device_entry_image_()
    , device_divider_image_()
    , forget_device_popup_(NULL)
    , authentication_popup_(NULL)
    , passcode_()
    , should_show_authentication_screen_(false)
    , should_hide_authentication_screen_(false)
    , should_cancel_authentication_(false)
    , should_refresh_device_list_(false)
{
    SetBackgroundColour(*wxWHITE);
    EnableScrolling(true, true);

    device_entry_image_.LoadFile("Resources/Device_Selected.png", wxBITMAP_TYPE_PNG);
    device_divider_image_.LoadFile("Resources/DividingLine.png", wxBITMAP_TYPE_PNG);

    SetScrollRate(0, device_entry_image_.GetSize().y + device_divider_image_.GetSize().y + (Constants::kDeviceDividerBuffer * 2));

    InitPopulate();

    // this sets our "physical" size - what we see on the panel
    SetSize(parent->GetSize().x, Constants::kDevicesPanelOpenSizeY - Constants::kDevicesPanelCloseSizeY);
    SetWindowStyle(wxVSCROLL);
}

// virtual
DeviceList::~DeviceList()
{
    DismisForgetDevicePopup();

    //clean up known devices
    known_devices_cs_.Enter();
    for(unsigned int i = 0; i < known_devices_.size(); ++i)
    {
        SAFE_DELETE(known_devices_[i]);
    }

    known_devices_.clear();
    known_devices_cs_.Leave();
}

void DeviceList::InitPopulate()
{
    GCSFrame *main_frame  = dynamic_cast<GCSFrame*>(GetGrandParent());
    if(main_frame == NULL)
    {
        return;
    }

    Server *server = main_frame->GetServer();

    if(server == NULL)
    {
        return;
    }
    
    int number = GetNumKnownDevices();
    LockDevices();
    for(int i = 0; i < number; ++i)
    {
        BasicClientDevice *client_device = GetKnownDevice(i);

        ClientDeviceType type = client_device->GetType();
        wxString device_name = wxString(client_device->GetName());
        wxString device_id = wxString(client_device->GetDeviceID());

        DeviceEntry *device_entry = new DeviceEntry(this,
                                                    wxID_ANY,
                                                    device_entry_image_,
                                                    device_id,
                                                    wxDefaultPosition,
                                                    device_entry_image_.GetSize(),
                                                    (type == CLIENT_DEVICE_TYPE_IOS ? DeviceEntry::IOS : DeviceEntry::ANDROID),
                                                    device_name,
                                                    false);
        AddEntry(device_entry);
    }
    UnlockDevices();
}

void DeviceList::AddEntry(DeviceEntry *entry)
{
    if(entry == NULL)
    {
        return;
    }

    devices_.push_back(entry);
    
    //assure all entries are hooked up to their button event
    Connect(entry->GetId(), wxEVT_BUTTON, wxCommandEventHandler(DeviceList::OnEntryClicked));

    Arrange();
}

void DeviceList::RemoveEntry(DeviceEntry *entry)
{
    if(entry == NULL)
    {
        return;
    }

    for(unsigned int i = 0; i < devices_.size(); ++i)
    {
        if(devices_.at(i) == entry)
        {
            devices_.erase(devices_.begin() + i);
            entry->Hide();
            entry->Close();
            entry->Destroy();

            break;
        }
    }

    Arrange();
}

void DeviceList::Arrange()
{
    if(devices_.size() < 1)
    {
        return; // no devices
    }

    Scroll(0, 0);

    wxSize virtual_size = device_entry_image_.GetSize();

    // adjust number of dividers as necessary
    int dividers_count = devices_.size() - 1;
    unsigned int target_dividers_count = std::max(0, dividers_count);

    while(dividers_.size() != target_dividers_count)
    {
        if(dividers_.size() < target_dividers_count)
        {
            wxStaticBitmap *divider = new wxStaticBitmap(this, wxID_ANY, device_divider_image_, wxDefaultPosition);
            dividers_.push_back(divider);
        }
        else if(dividers_.size() > target_dividers_count)
        {
            wxStaticBitmap *divider = dividers_.back();
            dividers_.pop_back();
            divider->Hide();
            divider->Close();
            divider->Destroy();
        }
    }

    for (unsigned int i = 0; i < devices_.size(); ++i)
    {
        devices_[i]->SetPosition(i > 0 ? wxPoint(0, dividers_[i - 1]->GetPosition().y + Constants::kDeviceDividerBuffer * 2)
                                       : wxPoint(0, device_entry_image_.GetSize().y * i));

        if(i < devices_.size() - 1) //only set up dividing line between first and last devices
        {
            dividers_[i]->SetPosition(wxPoint(0, devices_[i]->GetPosition().y + devices_[i]->GetSize().y));
        }

        devices_[i]->SetIsSelected(false);
    }

    int height = devices_.size()  * devices_[devices_.size() - 1]->GetSize().y +
                 (dividers_.size() >= 1 ? dividers_.size() * dividers_[dividers_.size() - 1]->GetSize().y : 0) +
                 dividers_.size();

    virtual_size.SetHeight(height); // the width is fine. Height needs to be calculated
    SetVirtualSize(virtual_size);

    Update();
}

std::vector<DeviceEntry*> DeviceList::GetDevices()
{
    return devices_;
}

void DeviceList::DismissAuthenticationPopup()
{
    if(authentication_popup_ != NULL)
    {
        authentication_popup_->Show(false);
    }
}

Server *DeviceList::GetServer() const
{
    GCSFrame *main_frame  = dynamic_cast<GCSFrame*>(GetGrandParent());
    return (main_frame != NULL ? main_frame->GetServer() : NULL);
}

void DeviceList::DisplayAuthenticationCallback(const std::wstring &passcode)
{
    passcode_ = passcode;
    should_show_authentication_screen_ = true;
}

void DeviceList::HideAuthenticationCallback()
{
    should_hide_authentication_screen_ = true;
}

void DeviceList::OnCanceledAuthenticationCallback()
{
    should_cancel_authentication_ = true;
}

void DeviceList::RefreshDeviceListCallback()
{
    should_refresh_device_list_ = true;
}

void DeviceList::DisplayAuthenticationHandler()
{
    if(authentication_popup_ == NULL)
    {
        authentication_popup_ = new AuthenticationPopup(this);
    }

    if(!authentication_popup_->IsShownOnScreen())
    {
        authentication_popup_->Show();
        authentication_popup_->CenterOnParent();
        wxPoint target_position((wxSystemSettings::GetMetric (wxSYS_SCREEN_X) / 2) - (authentication_popup_->GetSize().x / 2),
                                (wxSystemSettings::GetMetric (wxSYS_SCREEN_Y) / 2) - (authentication_popup_->GetSize().y / 2));

        authentication_popup_->SetPosition(target_position);
        authentication_popup_->AssignQRCode(passcode_);

        authentication_popup_->Update();
    }
}

void DeviceList::HideAuthenticationHandler()
{
    authentication_popup_->SetIsAuthenticated(true);
}

void DeviceList::OnCanceledAuthenticationHandler()
{
    if(authentication_popup_ != NULL)
    {
        authentication_popup_->SetIsAuthenticated(false);
        DismissAuthenticationPopup();

       SendOnAuthenticationCanceled();
    }
}

void DeviceList::RefreshDeviceListHandler()
{
    int number_of_devices = GetNumKnownDevices();

    LockDevices();
    for(int i = 0; i < number_of_devices; ++i)
    {
        BasicClientDevice *client_device = GetKnownDevice(i);

        wxString device_id = wxString(client_device->GetDeviceID());

        bool is_unique = true;
        for(unsigned int i = 0; i < devices_.size(); ++i)
        {
            if(device_id == wxString(devices_[i]->GetDeviceID()))
            {
                is_unique = false;
                break;
            }
        }

        if(is_unique)
        {
            DeviceEntry *device_entry = new DeviceEntry(this,
                                                        wxID_ANY,
                                                        device_entry_image_,
                                                        device_id,
                                                        wxDefaultPosition,
                                                        device_entry_image_.GetSize(),
                                                        (client_device->GetType() == CLIENT_DEVICE_TYPE_IOS ? DeviceEntry::IOS : DeviceEntry::ANDROID),
                                                        wxString(client_device->GetName()),
                                                        false);
            AddEntry(device_entry);
        }
    }
    UnlockDevices();

    Arrange();
}

bool DeviceList::GetShouldShowAuthentication()
{
    return should_show_authentication_screen_;
}

bool DeviceList::GetShouldHideAuthentication()
{
    return should_hide_authentication_screen_;
}

bool DeviceList::GetShouldCancelAuthentication()
{
    return should_cancel_authentication_;
}

bool DeviceList::GetShouldRefreshDeviceList()
{
    return should_refresh_device_list_;
}

void DeviceList::SetShouldShowAuthentication(bool should_show_authentication_screen)
{
    should_show_authentication_screen_ = should_show_authentication_screen;
}

void DeviceList::SetShouldHideAuthentication(bool should_hide_authentication_screen)
{
    should_hide_authentication_screen_ = should_hide_authentication_screen;
}

void DeviceList::SetShouldCancelAuthentication(bool should_cancel_authentication)
{
    should_cancel_authentication_ = should_cancel_authentication;
}

void DeviceList::SetShouldRefreshDeviceList(bool should_refresh_device_list)
{
    should_refresh_device_list_ = should_refresh_device_list;
}

void DeviceList::SetKnownDevices(std::vector<BasicClientDevice *> devices)
{
    LockDevices();

    for(unsigned int i = 0; i < known_devices_.size(); ++i)
    {
        SAFE_DELETE(known_devices_[i]);
    }
    known_devices_.clear();

    for(unsigned int i = 0; i < devices.size(); ++i)
    {
        BasicClientDevice *from = devices[i];
        if(from != NULL)
        {
            known_devices_.push_back(new BasicClientDevice(from->GetType(), from->GetName(), from->GetDeviceID(), from->GetDeviceType(), from->GetDeviceMake()));
        }
    }

    UnlockDevices();
}

void DeviceList::LockDevices()
{
    known_devices_cs_.Enter();
}

void DeviceList::UnlockDevices()
{
    known_devices_cs_.Leave();
}

int DeviceList::GetNumKnownDevices()
{
    int number = 0;
    LockDevices();
    GetServer()->GetKnownDevices(known_devices_);
    number = known_devices_.size();
    UnlockDevices();

    return number;
}

BasicClientDevice *DeviceList::GetKnownDevice(int index)
{
    if(index >= 0 && index < static_cast<int>(known_devices_.size()))
    {
        return known_devices_[index];
    }
    else
    {
        return NULL;
    }
}

void DeviceList::ForgetDevice(const std::wstring &device_id)
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->ForgetDevice(device_id);
    }

    DismisForgetDevicePopup();
}

void DeviceList::DismisForgetDevicePopup()
{
    if(forget_device_popup_ != NULL)
    {
        forget_device_popup_->Dismiss();
    }
}

void DeviceList::CancelAuthenticationRequest()
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->CancelAuthenticationRequest();
    }
}

void DeviceList::AllowConnection(bool allow)
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->AllowConnection(allow);
    }
}

void DeviceList::SendOnAuthenticationCanceled()
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->OnAuthenticationCanceled();
    }
}

void DeviceList::SetDoNotDisturb(bool dnd)
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->SetDoNotDisturb(dnd);
    }
}

void DeviceList::SetAutoStart(bool auto_start)
{
}

void DeviceList::SetServerName(std::wstring &name)
{
    Server *server = GetServer();
    if(server != NULL)
    {
        server->SetName(name);
    }
}

bool DeviceList::GetServerName(std::wstring &name)
{
    Server *server = GetServer();
    if(server != NULL)
    {
        return server->GetName(name);
    }
    
    return false;
}

void DeviceList::OnEntryClicked(wxCommandEvent &event)
{
    DeviceEntry *device_entry   = dynamic_cast<DeviceEntry*>(event.GetEventObject());

    if(forget_device_popup_ == NULL)
    {
        wxFrame *parent         = dynamic_cast<wxFrame*>(GetGrandParent());
        forget_device_popup_    = new ForgetDevicePopup(parent);
    }

    if(!forget_device_popup_->IsDeviceAssigned())
    {
        forget_device_popup_->SetSelectedDevice(device_entry, devices_);

        bool is_rtl_language = wxGetApp().IsLanguageRTL();
        wxPoint desired_position = device_entry->GetScreenPosition();
        if(is_rtl_language)
        {
            desired_position.x -= GetParent()->GetParent()->GetSize().x * 0.75f; //adjust position of pop up for arabic language
        }
        forget_device_popup_->PositionPopup(desired_position);
        forget_device_popup_->Popup();
    }
}

void DeviceList::OnMouseWheel(wxMouseEvent &event)
{
    if(event.GetWheelRotation() < 0)
    {
        Scroll(0, GetScrollPos(wxVERTICAL) + 1);
    }
    else if(event.GetWheelRotation() > 0)
    {
        Scroll(0, GetScrollPos(wxVERTICAL) - 1);
    }
    event.Skip();
}

//virtual
void DeviceList::DeviceConnectedCallback()
{
    GCSFrame *main_frame = dynamic_cast<GCSFrame*>(GetGrandParent());
    if (main_frame == NULL)
    {
        return;
    }

    main_frame->UpdateSystemTrayIcon(GCSFrame::ON);
}

//virtual
void DeviceList::DeviceDisconnectedCallback()
{
    GCSFrame *main_frame = dynamic_cast<GCSFrame*>(GetGrandParent());
    if (main_frame == NULL)
    {
        return;
    }

    main_frame->UpdateSystemTrayIcon(GCSFrame::OFF);
}

//virtual
void DeviceList::NoNetworkConnectionCallback()
{
    GCSFrame *main_frame = dynamic_cast<GCSFrame*>(GetGrandParent());
    if (main_frame == NULL)
    {
        return;
    }

    main_frame->UpdateSystemTrayIcon(GCSFrame::DISCONNECTED);
}
