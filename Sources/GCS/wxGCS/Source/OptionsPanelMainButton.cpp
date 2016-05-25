#include "OptionsPanelMainButton.h"
#include "OptionsPanel.h"
#include <wx/app.h> 
#include "main.h"
#include "Localization.h"

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(OptionsPanelMainButton, wxPanel)
    EVT_PAINT(OptionsPanelMainButton::OnPaint)
    EVT_LEFT_DOWN(OptionsPanelMainButton::OnMouseDown)
    EVT_LEFT_UP(OptionsPanelMainButton::OnMouseUp)
END_EVENT_TABLE()

OptionsPanelMainButton::OptionsPanelMainButton(wxWindow *parent, wxWindowID id, const wxBitmap &label, const wxPoint &position, const wxSize &size)
    : wxPanel(parent, id, position, size)
    , open_()
    , close_()
    , icon_()
    , label_(Localization::GetInstance()->GetString(LocalizedString::options_tab_text))
    , title_font_(wxFontInfo().FaceName(Constants::kFontIntelClearProName).Encoding(wxFONTENCODING_SYSTEM))
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();

    title_font_.SetPointSize((is_rtl_language) ? Constants::kFontSizeTitleRTL * Constants::Helpers::GetPPIRatio()
                                               : Constants::kFontSizeTitle * Constants::Helpers::GetPPIRatio());

    open_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/OptionsBG_ON.png" : "Resources/OptionsBG_ON.png", wxBITMAP_TYPE_PNG);
    close_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/OptionsBG_OFF.png" : "Resources/OptionsBG_OFF.png", wxBITMAP_TYPE_PNG);
    icon_.LoadFile("Resources/IconOptions.png", wxBITMAP_TYPE_PNG);

    SetSize(open_.GetSize());
    PaintNow();
}

//virtual
OptionsPanelMainButton::~OptionsPanelMainButton()
{
}

void OptionsPanelMainButton::PaintNow()
{
    //wxPaintDC dc(this);
    //Render(dc);
}

void OptionsPanelMainButton::OnMouseDown(wxMouseEvent &WXUNUSED(event))
{
    Refresh();
    PaintNow();
}

void OptionsPanelMainButton::OnMouseUp(wxMouseEvent &event)
{
    OptionsPanel *panel_parent = dynamic_cast<OptionsPanel*>(GetParent());

    if(panel_parent)
    {
        wxCommandEvent event(wxEVT_LEFT_DOWN, Constants::kOptionsButton);
        wxGetApp().ProcessEvent(event);
    }

    Refresh();
    PaintNow();
    event.Skip();
}

void OptionsPanelMainButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void OptionsPanelMainButton::Render(wxDC &dc)
{
    PrepareDC(dc);

    bool is_rtl_language = wxGetApp().IsLanguageRTL();

    OptionsPanel *parent_panel = dynamic_cast<OptionsPanel*>(GetParent());

    bool is_options_open = parent_panel->GetIsOptionsOpen();
    
    if(title_font_.IsOk())
    {
        dc.SetFont(title_font_);
    }

    dc.SetTextBackground(wxTransparentColour);
    dc.SetTextForeground(Constants::kIntelLightBlue);

    int offset (is_options_open ? -Constants::kTitleLabelYOffset : Constants::kTitleLabelYOffset);
    int center_y = (GetSize().y * 0.5) + offset;
    wxSize text_size = dc.GetTextExtent(label_);

    dc.DrawBitmap(is_options_open ? open_ : close_, 0, 0, true);
    dc.DrawBitmap(icon_, wxPoint(Constants::kMainIconX, center_y - (icon_.GetSize().y * 0.5)));
    dc.DrawText(label_, wxPoint(Constants::kTitleLabelX + (Constants::kMainIconX * 2), center_y - (text_size.y * 0.5)));
}
