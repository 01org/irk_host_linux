#include "AuthenticationPopup.h"
#include <qrencode.h>
#include <sstream>
#include <wx/dir.h>
#include <time.h>
#include "QRGenerator.h"
#include "Constants.h"
#include "AuthenticationButton.h"
#include "DeviceList.h"
#include "../../GCSNativeInterface/Server.h"
#include "Localization.h"
#include <wx/app.h> 
#include "main.h"
#include <unistd.h>

wxDECLARE_APP(GCSApp);

wxBEGIN_EVENT_TABLE(AuthenticationPopup, wxPopupWindow)
    EVT_PAINT(AuthenticationPopup::OnPaint)
wxEND_EVENT_TABLE()

AuthenticationPopup::AuthenticationPopup(wxWindow *parent)
    : wxPopupWindow(parent)
    , do_not_disturb_button_(NULL)
    , cancel_button_(NULL)
    , title_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , passcode_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , background_()
    , qr_code_()
    , check_()
    , title_("")
    , subtitle_("")
    , passcode_("")
    , app_name_("")
    , is_authenticated_(false)
{
    title_font_.SetPointSize(Constants::kFontSizeTitle * Constants::Helpers::GetPPIRatio());
    text_font_.SetPointSize(Constants::kFontSizeText * Constants::Helpers::GetPPIRatio());
    passcode_font_.SetPointSize(Constants::kFontSizePasscode * Constants::Helpers::GetPPIRatio());

    background_.LoadFile("Resources/AuthenticationBG.png", wxBITMAP_TYPE_PNG);
    title_ = Localization::GetInstance()->GetString(LocalizedString::authentication_text);
    subtitle_ = Localization::GetInstance()->GetString(LocalizedString::scan_image_text);
    app_name_ = Localization::GetInstance()->GetString(LocalizedString::product_name_long_server);

    SetSize(background_.GetSize());


    bool is_rtl_language = wxGetApp().IsLanguageRTL();

    wxBitmap normal_disturb;
    normal_disturb.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Do_Not_Disturb_OFF.png" : "Resources/Authentication/Button_Do_Not_Disturb_OFF.png", wxBITMAP_TYPE_PNG);
    wxBitmap over_disturb;
    over_disturb.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Do_Not_Disturb_ON.png" : "Resources/Authentication/Button_Do_Not_Disturb_ON.png", wxBITMAP_TYPE_PNG);
    wxBitmap down_disturb;
    down_disturb.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Do_Not_Disturb_PRESS.png" : "Resources/Authentication/Button_Do_Not_Disturb_PRESS.png", wxBITMAP_TYPE_PNG);
    wxBitmap normal_cancel;
    normal_cancel.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Cancel_OFF.png" : "Resources/Authentication/Button_Cancel_OFF.png", wxBITMAP_TYPE_PNG);
    wxBitmap over_cancel;
    over_cancel.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Cancel_ON.png" : "Resources/Authentication/Button_Cancel_ON.png", wxBITMAP_TYPE_PNG);
    wxBitmap down_cancel;
    down_cancel.LoadFile((is_rtl_language) ? "Resources/reverse_UI/Button_Cancel_PRESS.png" : "Resources/Authentication/Button_Cancel_PRESS.png", wxBITMAP_TYPE_PNG);

    do_not_disturb_button_ = new AuthenticationButton(this, wxID_ANY, normal_disturb, over_disturb, down_disturb, normal_disturb, Localization::GetInstance()->GetString(LocalizedString::do_not_disturb_button_text));
    cancel_button_ = new AuthenticationButton(this, wxID_ANY, normal_cancel, over_cancel, down_cancel, normal_cancel, Localization::GetInstance()->GetString(LocalizedString::cancel_button_text));

    wxPoint disturb_target_position = wxPoint(Constants::kAuthButtonBuffer, background_.GetHeight() - do_not_disturb_button_->GetSize().y * 1.3f);
    wxPoint cancel_target_position = wxPoint(background_.GetWidth() - Constants::kAuthButtonBuffer - cancel_button_->GetSize().x, background_.GetHeight() - cancel_button_->GetSize().y * 1.3f);

    if(is_rtl_language)
    {
        // swap the values so the buttons are on opposite sides
        wxPoint temp = disturb_target_position;
        disturb_target_position = cancel_target_position;
        cancel_target_position = temp;
    }

    do_not_disturb_button_->SetPosition(disturb_target_position);
    cancel_button_->SetPosition(cancel_target_position);

    check_.LoadFile("Resources/Authentication/Authentication_Check.png", wxBITMAP_TYPE_PNG);
}

// virtual
AuthenticationPopup::~AuthenticationPopup()
{
}

void AuthenticationPopup::AssignQRCode(const std::wstring &passcode)
{
    wxString temp_string(passcode);
    wxCharBuffer temp_buffer = temp_string.ToUTF8();
    char* passed_passcode = temp_buffer.data();

    qr_code_ = QRGenerator::GetInstance()->GenerateQRCode(passed_passcode);
    passcode_ = passcode;
    Refresh();
    PaintNow();
}

void AuthenticationPopup::HandleButtonPress(wxWindowID id)
{
    if(id == do_not_disturb_button_->GetId())
    {
        DeviceList *parent = dynamic_cast<DeviceList*>(GetParent());
        parent->SetDoNotDisturb(true);
        
        wchar_t filename[MAX_PATH] = L"/var/lib/RemoteKeyboard/Options.xml";

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filename);
        if(result)
        {            
            doc.child(L"Options").attribute(L"bDNDMode").set_value(true);
            doc.save_file(filename);
        }

        parent->DismissAuthenticationPopup();

        //tell the server we have cancelled the authentication
        parent->CancelAuthenticationRequest();
    }
    else if(id == cancel_button_->GetId())
    {
        DeviceList *parent = dynamic_cast<DeviceList*>(GetParent());
        parent->DismissAuthenticationPopup();

        //tell the server we have cancelled the authentication
        parent->CancelAuthenticationRequest();
    }
}

void AuthenticationPopup::SetIsAuthenticated(bool authenticated)
{
    is_authenticated_ = authenticated;
    Refresh();
    PaintNow();

    if(is_authenticated_) // keep the screen up for a few seconds
    {
        usleep(2500000);
        
        DeviceList *parent = dynamic_cast<DeviceList*>(GetParent());
        parent->DismissAuthenticationPopup();

        //tell the server to allow the connection
        parent->AllowConnection(true);

        is_authenticated_ = false;
    }
}

void AuthenticationPopup::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void AuthenticationPopup::PaintNow()
{
    wxClientDC dc(this);
    Render(dc);
}

void AuthenticationPopup::Render(wxDC &dc)
{
    wxSize popup_size = GetSize();
    wxSize qr_size    = qr_code_.GetSize();

    dc.SetTextForeground(*wxWHITE);
    dc.DrawBitmap(background_, 0, 0, false);

    dc.SetFont(passcode_font_);
    dc.DrawText(passcode_, (popup_size.x / 2) - (dc.GetTextExtent(passcode_).x / 2), Constants::kAuthPasscodeY);
    
    dc.SetFont(title_font_);
    dc.DrawText(title_, Constants::kAuthTitleX, Constants::kAuthTitleY);

    dc.SetFont(text_font_);
    dc.DrawText(subtitle_, Constants::kAuthTitleX, Constants::kAuthSubtitleY);

    if(qr_code_.IsOk()) // safety check in case QR generation failed
    {
        dc.DrawBitmap(qr_code_,
                      (popup_size.x / 2) - (qr_size.x / 2),
                      (popup_size.y / 2) - (qr_size.y / 2),
                      false);
    }

    if(is_authenticated_)
    {
        dc.DrawBitmap(check_,
                      Constants::kAuthCheckX,
                      Constants::kAuthCheckY,
                      false);
    }

    dc.SetTextForeground(Constants::kIntelYellow);
    dc.DrawText(app_name_, wxPoint(GetSize().x - dc.GetTextExtent(app_name_).GetX() - Constants::kTitleLabelXRightPadding - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2), Constants::kAuthSubtitleY));
}
