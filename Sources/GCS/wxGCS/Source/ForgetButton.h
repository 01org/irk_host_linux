#ifndef FORGET_BUTTON_H_
#define FORGET_BUTTON_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class ForgetButton : public wxPanel
{
public:
    enum ForgetButtonState
    {
        MOUSE_OVER,
        MOUSE_EXIT,
        MOUSE_DOWN,
        MOUSE_UP
    };

    ForgetButton(wxWindow *parent, const wxBitmap &label);
    virtual ~ForgetButton();

private:
    DISALLOW_COPY_AND_ASSIGN(ForgetButton);
    void OnMouseOver(wxMouseEvent &event);
    void OnMouseExit(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);

    wxBitmap            normal_;
    wxBitmap            mouse_over_;
    wxBitmap            mouse_down_;
    wxBitmap            close_icon_;
    wxString            forget_;
    wxFont              text_font_;
    ForgetButtonState   state_;

    DECLARE_EVENT_TABLE()
};

#endif //FORGET_BUTTON_H_ 
