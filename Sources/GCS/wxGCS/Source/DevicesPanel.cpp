#include "DevicesPanel.h"
#include "Constants.h"
#include "GCSFrame.h"
#include "DevicesPanelMainButton.h"

DevicesPanel::DevicesPanel(wxFrame *parent, int x, int y, int width, int height, wxBitmap main_devices_button_image)
    : wxPanel(parent)
    , main_button_(NULL)
    , device_list_(NULL)
    , is_options_open_(false)
{
    SetPosition(wxPoint(x,y));
    SetSize(wxSize(width,height));

    //the main label
    main_button_ = new DevicesPanelMainButton(this, main_devices_button_image);
    main_button_->SetId(Constants::kDevicesButton);

    device_list_ = new DeviceList(this);
    device_list_->SetPosition(wxPoint(0, main_button_->GetSize().y));
    device_list_->SetId(Constants::kDeviceList);

    SetSize(wxSize(Constants::kWindowWidth, Constants::kDevicesPanelOpenSizeY));
}

// virtual
DevicesPanel::~DevicesPanel()
{
}

void DevicesPanel::OptionsClicked(bool is_options_open)
{
    is_options_open_ = is_options_open;
}

DeviceList* DevicesPanel::GetDeviceList()
{
    return device_list_;
}

bool DevicesPanel::GetIsOptionsOpen()
{
    return is_options_open_;
}