#include "ForgetDevicePopup.h"
#include "Constants.h"
#include "DeviceList.h"
#include "ForgetButton.h"
#include "GCSFrame.h"
#include "../../GCSNativeInterface/Server.h"
#include "../../GCSNativeInterface/BasicClientDevice.h"
#include "../../GCSNativeInterface/ClientDevice.h"
#include "Localization.h"
#include <wx/app.h> 
#include "main.h"

wxDECLARE_APP(GCSApp);

wxBEGIN_EVENT_TABLE(ForgetDevicePopup, wxPopupTransientWindow)
    EVT_PAINT(ForgetDevicePopup::OnPaint)
wxEND_EVENT_TABLE()

ForgetDevicePopup::ForgetDevicePopup(wxWindow *parent)
    : wxPopupTransientWindow(parent)
    , popup_graphic_()
    , make_()
    , model_()
    , forget_()
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , device_to_forget_(NULL)
    , forget_button_(NULL)
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    popup_graphic_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Dropdown.png" : "Resources/Dropdown.png", wxBITMAP_TYPE_PNG);

    SetSize(popup_graphic_.GetSize()); //both are the same size so we can determine using either

    //pass an image for wxStaticBitmap's default constructor
    wxBitmap forget_button_off;
    forget_button_off.LoadFile("Resources/Button_ForgetOFF.png", wxBITMAP_TYPE_PNG);
    forget_button_ = new ForgetButton(this, forget_button_off);
    forget_button_->SetPosition(wxPoint(Constants::kPopupButtonPositionX, Constants::kPopupButtonPositionY));
}

// virtual
ForgetDevicePopup::~ForgetDevicePopup()
{
}

void ForgetDevicePopup::FillData(wxString make_data, wxString model_data)
{
    make_   = Localization::GetInstance()->GetString(LocalizedString::tool_tip_make_text ) + " " + make_data;
    model_  = Localization::GetInstance()->GetString(LocalizedString::tool_tip_model_text) + " " + model_data;
}

void ForgetDevicePopup::PositionPopup(const wxPoint &position)
{
    SetPosition(wxPoint(position.x + Constants::kPopupPositionOffsetX, position.y + Constants::kPopupBelowOffsetY));
}

void ForgetDevicePopup::SetSelectedDevice(DeviceEntry *device_to_forget, std::vector<DeviceEntry*> devices)
{
    device_to_forget_ = device_to_forget;

    for(std::vector<DeviceEntry*>::iterator iter = devices.begin(); iter != devices.end(); ++iter)
    {
        if((*iter) == device_to_forget)
        {
            (*iter)->SetIsSelected(true);
        }
        else
        {
            (*iter)->SetIsSelected(false);
        }
        (*iter)->Refresh();
    }

    if(device_to_forget_ != NULL)
    {
        DeviceList *device_list = dynamic_cast<DeviceList*>(device_to_forget_->GetParent());
        int number_of_devices = device_list->GetNumKnownDevices();
        device_list->LockDevices();
        for(int i = 0; i < number_of_devices; i++)
        {
            BasicClientDevice *device = device_list->GetKnownDevice(i);
            if(device_to_forget_->GetDeviceID() == wxString(device->GetDeviceID()))
            {
                wxString make(device->GetDeviceMake());
                wxString model(device->GetDeviceType());

                if(make == wxEmptyString && model[0] == 'i' && model[1] == 'P')
                {
                    make = Localization::GetInstance()->GetString(LocalizedString::tool_tip_apple); // we have issues grabbing the make from Apple devices. this is a pretty safe fallback
                }

                FillData(make, model);
                break;
            }
        }
        device_list->UnlockDevices();
    }
}

void ForgetDevicePopup::OnForgetButtonClicked()
{
    DeviceList *device_list = dynamic_cast<DeviceList*>(device_to_forget_->GetParent());
    
    if(device_list)
    {
        device_list->ForgetDevice(device_to_forget_->GetDeviceID().ToStdWstring());
        
        device_list->RemoveEntry(device_to_forget_);
        SetSelectedDevice(NULL, device_list->GetDevices());
        device_list->Refresh();
    }
}

bool ForgetDevicePopup::IsDeviceAssigned()
{
    return device_to_forget_ != NULL;
}

void ForgetDevicePopup::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    wxPaintDC dc(this);
    PrepareDC(dc);

    dc.SetFont(text_font_);
    dc.SetTextForeground(Constants::kIntelBlue);

    dc.DrawBitmap(popup_graphic_, 0, 0, true);
    // right justify if RTL language
    dc.DrawText(make_, (is_rtl_language ? popup_graphic_.GetWidth() - Constants::kPopupDataPositionX - dc.GetTextExtent(make_).x
                                        : Constants::kPopupDataPositionX), Constants::kPopupDataPositionY);
    dc.DrawText(model_, (is_rtl_language ? popup_graphic_.GetWidth() - Constants::kPopupDataPositionX - dc.GetTextExtent(model_).x
                                         : Constants::kPopupDataPositionX), Constants::kPopupDataPositionY + Constants::kPopupDataDeltaY);
}

void ForgetDevicePopup::OnDismiss()
{
    wxPopupTransientWindow::OnDismiss();
    
    if(device_to_forget_ == NULL)
    {
        return;
    }

    DeviceList *device_list = dynamic_cast<DeviceList*>(device_to_forget_->GetParent());
    
    if(device_list)
    {
        SetSelectedDevice(NULL, device_list->GetDevices());
        device_list->Refresh();
    }
}
