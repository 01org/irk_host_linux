#ifndef DEVICES_PANEL_MAIN_BUTTON_H_
#define DEVICES_PANEL_MAIN_BUTTON_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class DevicesPanelMainButton : public wxPanel
{
public:
    DevicesPanelMainButton(wxWindow *parent, const wxBitmap &label);
    virtual ~DevicesPanelMainButton();
    void PaintNow();

private:
    DISALLOW_COPY_AND_ASSIGN(DevicesPanelMainButton);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC& dc);

    wxBitmap            open_;
    wxBitmap            close_;
    wxBitmap            icon_;
    wxString            label_;
    wxFont              text_font_;

    DECLARE_EVENT_TABLE()
};

#endif //DEVICES_PANEL_MAIN_BUTTON_H_
