#include "HelpButton.h"
#include <wx/app.h> 
#include <wx/filename.h>
#include "main.h"
#include "Localization.h"

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(HelpButton, wxPanel)
    EVT_ENTER_WINDOW(HelpButton::OnMouseOver)
    EVT_LEAVE_WINDOW(HelpButton::OnMouseExit)
    EVT_LEFT_DOWN(HelpButton::OnMouseDown)
    EVT_LEFT_UP(HelpButton::OnMouseUp)
    EVT_PAINT(HelpButton::OnPaint)
END_EVENT_TABLE()

HelpButton::HelpButton(wxWindow *parent, const wxBitmap &label)
    : wxPanel(parent)
    , normal_()
    , mouse_over_()
    , mouse_down_()
    , state_(MOUSE_EXIT)
    , help_text_(Localization::GetInstance()->GetString(LocalizedString::help_text))
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearProName).Encoding(wxFONTENCODING_SYSTEM))
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    normal_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/IconHelp_OFF.png" : "Resources/IconHelp_OFF.png", wxBITMAP_TYPE_PNG);
    mouse_over_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/IconHelp_OVER.png" : "Resources/IconHelp_OVER.png", wxBITMAP_TYPE_PNG);
    mouse_down_.LoadFile((is_rtl_language) ? "Resources/reverse_UI/IconHelp_PRESS.png" : "Resources/IconHelp_PRESS.png", wxBITMAP_TYPE_PNG);

    //SetBitmap(normal_);
    SetSize(normal_.GetSize());

    text_font_.SetPointSize(Constants::kFontSizeTitle * Constants::Helpers::GetPPIRatio());
    PaintNow();
}

// virtual
HelpButton::~HelpButton()
{
}

void HelpButton::PaintNow()
{
    //wxPaintDC dc(this);
    //Render(dc);
}

void HelpButton::OnMouseOver(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_OVER;
    Refresh();
    PaintNow();
}

void HelpButton::OnMouseExit(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_EXIT;
    Refresh();
    PaintNow();
}

void HelpButton::OnMouseDown(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_DOWN;
    Refresh();
    PaintNow();
}

void HelpButton::OnMouseUp(wxMouseEvent &event)
{
    wxString html_path;
    wxString language_code = Localization::GetInstance()->GetLanguageCode();

    html_path = "Resources/help/" + language_code + "/help.html";

    wxFileName file;
    wxString full_path;
    file.Assign(html_path);
    file.MakeAbsolute();
    full_path = file.GetFullPath();
    
    system(wxString("xdg-open ") + full_path);

    state_ = MOUSE_UP;
    Refresh();
    PaintNow();
    event.Skip();
}

void HelpButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void HelpButton::Render(wxDC &dc)
{
    PrepareDC(dc);

    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    dc.SetPen(Constants::kBorderBlue);
    dc.SetBrush(Constants::kBorderBlue);
    dc.SetTextForeground(Constants::kIntelYellow);
    dc.SetFont(text_font_);
    dc.DrawRectangle(0, 0, GetSize().x, GetSize().y);

    int button_y = (GetSize().y - normal_.GetSize().y) * 0.5;

    switch(state_)
    {
    case MOUSE_OVER:
        dc.DrawBitmap(mouse_over_, 0, button_y, true);
        dc.SetTextForeground(Constants::kOverText);
        break;
    case MOUSE_EXIT:
        dc.DrawBitmap(normal_, 0, button_y, true);
        dc.SetTextForeground(Constants::kIntelYellow);
        break;
    case MOUSE_DOWN:
        dc.DrawBitmap(mouse_down_, 0, button_y, true);
        dc.SetTextForeground(Constants::kPressText);
        break;
    case MOUSE_UP:
        dc.DrawBitmap(mouse_over_, 0, button_y, true);
        dc.SetTextForeground(Constants::kOverText);
        break;
    }

    wxSize text_size = dc.GetTextExtent(help_text_);
    int text_y = (GetSize().y - text_size.y) * 0.5;

    dc.DrawText(help_text_, wxPoint(normal_.GetSize().x + Constants::kHelpTextPadding, text_y));


}
