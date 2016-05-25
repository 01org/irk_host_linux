#include "DevicesPanelMainButton.h"
#include "Constants.h"
#include "DevicesPanel.h"
#include "main.h"
#include "Localization.h"
#include <wx/app.h> 

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(DevicesPanelMainButton, wxPanel)
    EVT_LEFT_DOWN(DevicesPanelMainButton::OnMouseDown)
    EVT_LEFT_UP(DevicesPanelMainButton::OnMouseUp)
    EVT_PAINT(DevicesPanelMainButton::OnPaint)
END_EVENT_TABLE()

DevicesPanelMainButton::DevicesPanelMainButton(wxWindow *parent, const wxBitmap &label)
    : wxPanel(parent)
    , open_()
    , close_()
    , icon_()
    , label_(Localization::GetInstance()->GetString(LocalizedString::device_tab_text))
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearProName).Encoding(wxFONTENCODING_SYSTEM))
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    open_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/DevicesBG_ON.png" : "Resources/DevicesBG_ON.png", wxBITMAP_TYPE_PNG);
    close_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/DevicesBG_OFF.png" : "Resources/DevicesBG_OFF.png", wxBITMAP_TYPE_PNG);
    icon_.LoadFile("Resources/IconDevice.png", wxBITMAP_TYPE_PNG);

    text_font_.SetPointSize((is_rtl_language) ? Constants::kFontSizeTitleRTL * Constants::Helpers::GetPPIRatio()
                                              : Constants::kFontSizeTitle * Constants::Helpers::GetPPIRatio());
    
    SetSize(open_.GetSize());
    PaintNow();
}

// virtual
DevicesPanelMainButton::~DevicesPanelMainButton()
{

}

void DevicesPanelMainButton::PaintNow()
{
    //wxPaintDC dc(this);
    //Render(dc);
}

void DevicesPanelMainButton::OnMouseDown(wxMouseEvent &WXUNUSED(event))
{
    Refresh();
    PaintNow();
}

void DevicesPanelMainButton::OnMouseUp(wxMouseEvent &event)
{
    DevicesPanel *panel_parent = dynamic_cast<DevicesPanel*>(GetParent());

    if(panel_parent)
    {
        wxCommandEvent event(wxEVT_LEFT_DOWN, Constants::kDevicesButton);
        wxGetApp().ProcessEvent(event);
    }

    Refresh();
    PaintNow();
    event.Skip();
}

void DevicesPanelMainButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void DevicesPanelMainButton::Render(wxDC &dc)
{
    PrepareDC(dc);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetFont(text_font_);
    dc.SetTextForeground(Constants::kIntelLightBlue);

    int center_y = GetSize().y * 0.5 - Constants::kTitleLabelYOffset;
    wxSize text_size = dc.GetTextExtent(label_);

    DevicesPanel *panel_parent = dynamic_cast<DevicesPanel*>(GetParent());

    if(panel_parent != NULL)
    {
        if(panel_parent->GetIsOptionsOpen())
        {
            dc.DrawBitmap(close_, 0, 0);
        }
        else
        {
            dc.DrawBitmap(open_, 0, 0);
        }
    }

    dc.DrawBitmap(icon_, wxPoint(Constants::kMainIconX, center_y - (icon_.GetSize().y * 0.5)));
    dc.DrawText(label_, wxPoint(Constants::kTitleLabelX + (Constants::kMainIconX * 2), center_y - (text_size.y * 0.5)));
}
