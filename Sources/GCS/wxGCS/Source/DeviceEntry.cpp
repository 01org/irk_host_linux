#include "DeviceEntry.h"
#include "Constants.h"
#include <wx/app.h> 
#include "main.h"

wxDECLARE_APP(GCSApp);

wxBEGIN_EVENT_TABLE(DeviceEntry, wxPanel)
    EVT_PAINT(DeviceEntry::OnPaint)
    EVT_LEFT_DOWN(DeviceEntry::OnMouseDown)
    EVT_LEFT_UP(DeviceEntry::OnMouseUp)
wxEND_EVENT_TABLE()

DeviceEntry::DeviceEntry(wxWindow *parent,
                         wxWindowID id,
                         const wxBitmap &bitmap,
                         const wxString device_id,
                         const wxPoint &pos,
                         const wxSize &size,
                         const DeviceType device_type,
                         const wxString &label,
                         bool is_authenticated)
    : wxPanel(parent, id, pos, size)
    , empty_bitmap_(wxBitmap(0,0))
    , selected_background_(bitmap)
    , device_graphic_unselected_()
    , device_graphic_selected_()
    , status_graphic_check_()
    , status_graphic_arrow_()
    , device_name_(label)
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , is_selected_(false)
    , is_authenticated_(is_authenticated)
    , is_active_device_(false)
    , device_id_(device_id)
{
    SetWindowStyle(wxBORDER_NONE);
    SetBackgroundColour(*wxWHITE);

    text_font_.SetPointSize(Constants::kFontSizeText * Constants::Helpers::GetPPIRatio());

    if(device_type == IOS)
    {
        device_graphic_selected_.LoadFile("Resources/Icon_iPhone_ON.png", wxBITMAP_TYPE_PNG);
        device_graphic_unselected_.LoadFile("Resources/Icon_iPhone_OFF.png", wxBITMAP_TYPE_PNG);
    }
    else
    {
        device_graphic_selected_.LoadFile("Resources/Icon_Android_ON.png", wxBITMAP_TYPE_PNG);
        device_graphic_unselected_.LoadFile("Resources/Icon_Android_OFF.png", wxBITMAP_TYPE_PNG);
    }
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    status_graphic_check_.LoadFile("Resources/Device_Check.png", wxBITMAP_TYPE_PNG);
    status_graphic_arrow_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/IconArrow.png" : "Resources/IconArrow.png", wxBITMAP_TYPE_PNG);
}

// virtual
DeviceEntry::~DeviceEntry()
{
}

void DeviceEntry::SetIsSelected(bool is_selected)
{
    is_selected_ = is_selected;
}

bool DeviceEntry::GetIsSelected()
{
    return is_selected_;
}

void DeviceEntry::SetIsAuthenticated(bool is_authenticated)
{
    is_authenticated_ = is_authenticated;
}

bool DeviceEntry::GetIsAuthenticated()
{
    return is_authenticated_;
}

wxString DeviceEntry::GetDeviceID()
{
    return device_id_;
}

void DeviceEntry::OnMouseDown(wxMouseEvent &event)
{
    Refresh();
}

void DeviceEntry::OnMouseUp(wxMouseEvent &event)
{
    int mouse_x_position = wxGetMouseState().GetX();
    int mouse_y_position = wxGetMouseState().GetY();
    wxPoint screen_position = GetScreenPosition();
    wxSize size = GetSize();

    bool is_rtl_language = wxGetApp().IsLanguageRTL();

    if(is_rtl_language)
    {
        if(mouse_x_position > screen_position.x ||
            mouse_y_position < screen_position.y ||
            mouse_x_position < screen_position.x - size.x ||
            mouse_y_position > screen_position.y + size.y)
        {
            return;
        }
    }
    else
    {
        if(mouse_x_position < screen_position.x ||
            mouse_y_position < screen_position.y ||
            mouse_x_position > screen_position.x + GetSize().x ||
            mouse_y_position > screen_position.y + GetSize().y)
        {
            return;
        }
    }

    DeviceList *list_parent = dynamic_cast<DeviceList*>(GetParent());

    if(list_parent)
    {
        wxCommandEvent event(wxEVT_BUTTON, GetId());
        event.SetEventObject(this);
        list_parent->ProcessWindowEvent(event);
    }
}

void DeviceEntry::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    dc.SetFont(text_font_);

    dc.DrawBitmap((is_selected_ ? selected_background_ : empty_bitmap_), 0, 0, false); //draw the button or not
    dc.DrawBitmap((is_selected_ ? device_graphic_selected_ : device_graphic_unselected_), Constants::kDeviceEntryDeviceX, Constants::kDeviceEntryDeviceY, false);
    dc.DrawBitmap((is_authenticated_ ? status_graphic_check_ : status_graphic_arrow_), Constants::kDeviceEntryStatusX, Constants::kDeviceEntryStatusY, false);
    dc.SetTextForeground(is_selected_ ? *wxWHITE : Constants::kIntelBlue);
    dc.DrawText(device_name_, Constants::kDeviceEntryLabelX, Constants::kDeviceEntryLabelY);
}
