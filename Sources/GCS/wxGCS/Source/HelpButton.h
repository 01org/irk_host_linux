#ifndef HELP_BUTTON_H_
#define HELP_BUTTON_H_

#include <wx/wx.h>
#include <wx/generic/statbmpg.h>
#include "../../GCSNativeInterface/macros.h"

class HelpButton : public wxPanel
{
public:
    enum HelpButtonState
    {
        MOUSE_OVER,
        MOUSE_EXIT,
        MOUSE_DOWN,
        MOUSE_UP
    };

    HelpButton(wxWindow *parent, const wxBitmap &label);
    virtual ~HelpButton();
    void PaintNow();

private:
    DISALLOW_COPY_AND_ASSIGN(HelpButton);
    void OnMouseOver(wxMouseEvent &event);
    void OnMouseExit(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC &dc);

    wxBitmap        normal_;
    wxBitmap        mouse_over_;
    wxBitmap        mouse_down_;
    wxString        help_text_;
    wxFont          text_font_;
    HelpButtonState state_;

    DECLARE_EVENT_TABLE()
};

#endif //HELP_BUTTON_H_
