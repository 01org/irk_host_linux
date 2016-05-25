#ifndef FORGET_DEVICE_POPUP_H_
#define FORGET_DEVICE_POPUP_H_

#include <wx/wx.h>
#include <wx/popupwin.h>
#include "DeviceEntry.h"
#include <vector>

class ForgetButton;

class ForgetDevicePopup : public wxPopupTransientWindow
{
public:
    ForgetDevicePopup(wxWindow *parent);
    virtual ~ForgetDevicePopup();

    void PositionPopup(const wxPoint &position);
    void SetSelectedDevice(DeviceEntry *device_to_forget, std::vector<DeviceEntry*> device_list);
    void OnForgetButtonClicked();
    bool IsDeviceAssigned();

private:
    DISALLOW_COPY_AND_ASSIGN(ForgetDevicePopup);
    void FillData(wxString make_data, wxString model_data);
    void OnPaint(wxPaintEvent &event);
    void OnDismiss();

    wxBitmap        popup_graphic_;
    wxString        make_;
    wxString        model_;
    wxString        forget_;
    wxFont          text_font_;
    DeviceEntry     *device_to_forget_;
    ForgetButton    *forget_button_;

    DECLARE_EVENT_TABLE()
};

#endif //FORGET_DEVICE_POPUP_H_