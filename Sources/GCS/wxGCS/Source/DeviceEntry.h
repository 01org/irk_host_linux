#ifndef DEVICE_ENTRY_H_
#define DEVICE_ENTRY_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class DeviceEntry : public wxPanel
{
public:
    enum DeviceType
    {
        IOS,
        ANDROID
    };

    DeviceEntry(wxWindow *parent,
                wxWindowID id,
                const wxBitmap &bitmap,
                const wxString device_id,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                const DeviceType device_type = DeviceEntry::IOS,
                const wxString &label = wxT("DEVICE"),
                bool is_authenticated = false);

    virtual ~DeviceEntry();
    void SetIsSelected(bool is_selected);
    bool GetIsSelected();
    void SetIsAuthenticated(bool is_authenticated);
    bool GetIsAuthenticated();
    wxString GetDeviceID();

private:
    DISALLOW_COPY_AND_ASSIGN(DeviceEntry);
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);

    wxBitmap        empty_bitmap_;
    wxBitmap        selected_background_;
    wxBitmap        device_graphic_unselected_;
    wxBitmap        device_graphic_selected_;
    wxBitmap        status_graphic_check_;
    wxBitmap        status_graphic_arrow_;
    wxString        device_name_;
    wxFont          text_font_;
    bool            is_selected_;
    bool            is_authenticated_;
    bool            is_active_device_;
    wxString        device_id_;

    wxDECLARE_EVENT_TABLE();
};

#endif //DEVICE_ENTRY_H_
