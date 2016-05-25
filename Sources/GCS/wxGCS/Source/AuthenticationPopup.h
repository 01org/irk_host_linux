#ifndef AUTHENTICATION_POPUP_H_
#define AUTHENTICATION_POPUP_H_

#include <wx/wx.h>
#include <wx/popupwin.h>
#include "../../GCSNativeInterface/macros.h"

class AuthenticationButton;

class AuthenticationPopup : public wxPopupWindow
{
public:
    AuthenticationPopup(wxWindow *parent);
    virtual ~AuthenticationPopup();
    void AssignQRCode(const std::wstring &passcode);
    void HandleButtonPress(wxWindowID id);
    void SetIsAuthenticated(bool authenticated);

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationPopup);
    void OnPaint(wxPaintEvent &event);
    void PaintNow();
    void Render(wxDC &dc);

    AuthenticationButton *do_not_disturb_button_;
    AuthenticationButton *cancel_button_;
    wxFont          title_font_;
    wxFont          text_font_;
    wxFont          passcode_font_;
    wxBitmap        background_;
    wxBitmap        qr_code_;
    wxBitmap        check_;
    wxString        title_;
    wxString        subtitle_;
    wxString        passcode_;
    wxString        app_name_;
    bool            is_authenticated_;

    DECLARE_EVENT_TABLE()
};

#endif //AUTHENTICATION_POPUP_H_