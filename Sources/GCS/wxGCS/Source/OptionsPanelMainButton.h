#ifndef OPTIONS_PANEL_MAIN_BUTTON_H_
#define OPTIONS_PANEL_MAIN_BUTTON_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class OptionsPanelMainButton : public wxPanel
{
public:
    OptionsPanelMainButton(wxWindow *parent, wxWindowID id, const wxBitmap &label, const wxPoint &position = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    virtual ~OptionsPanelMainButton();
    void PaintNow();

private:
    DISALLOW_COPY_AND_ASSIGN(OptionsPanelMainButton);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC& dc);

    wxBitmap    open_;
    wxBitmap    close_;
    wxBitmap    icon_;
    wxString    label_;
    wxFont      title_font_;

    DECLARE_EVENT_TABLE()
};

#endif //OPTIONS_PANEL_MAIN_BUTTON_H_
