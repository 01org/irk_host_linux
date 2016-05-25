#ifndef DEVICES_PANEL_H_
#define DEVICES_PANEL_H_

#include <wx/wx.h>
#include "DeviceList.h"
#include "../../GCSNativeInterface/macros.h"

class DevicesPanelMainButton;

class DevicesPanel : public wxPanel
{
public:
    DevicesPanel(wxFrame *parent, int x, int y, int width, int height, wxBitmap main_devices_button_image);
    virtual ~DevicesPanel();
    void OptionsClicked(bool is_options_open);
    DeviceList* GetDeviceList();
    bool GetIsOptionsOpen();

private:
    DevicesPanelMainButton  *main_button_;
    DeviceList              *device_list_;
    bool                    is_options_open_;
};

#endif //DEVICES_PANEL_H_