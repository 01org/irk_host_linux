#ifndef AUTHENTICATION_BUTTON_H_
#define AUTHENTICATION_BUTTON_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class AuthenticationButton : public wxPanel
{
public:
    enum AuthenticationButtonState
    {
        MOUSE_OVER,
        MOUSE_EXIT,
        MOUSE_DOWN,
        MOUSE_UP
    };
    AuthenticationButton(wxWindow *parent, wxWindowID id, wxBitmap normal, wxBitmap over, wxBitmap down, const wxBitmap &label, const wxString &button_text);
    virtual ~AuthenticationButton();
    void PaintNow();

private:
    DISALLOW_COPY_AND_ASSIGN(AuthenticationButton);
    void OnMouseOver(wxMouseEvent &event);
    void OnMouseExit(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC &dc);

    wxBitmap                    normal_;
    wxBitmap                    mouse_over_;
    wxBitmap                    mouse_down_;
    wxString                    button_text_;
    wxFont                      text_font_;
    AuthenticationButtonState   state_;

    DECLARE_EVENT_TABLE()

};

#endif //AUTHENTICATION_BUTTON_H_
