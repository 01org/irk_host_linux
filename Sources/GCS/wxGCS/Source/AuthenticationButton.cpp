#include "AuthenticationButton.h"
#include "AuthenticationPopup.h"
#include "Constants.h"
#include <wx/app.h> 
#include "main.h"

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(AuthenticationButton, wxPanel)
    EVT_ENTER_WINDOW(AuthenticationButton::OnMouseOver)
    EVT_LEAVE_WINDOW(AuthenticationButton::OnMouseExit)
    EVT_LEFT_DOWN(AuthenticationButton::OnMouseDown)
    EVT_LEFT_UP(AuthenticationButton::OnMouseUp)
    EVT_PAINT(AuthenticationButton::OnPaint)
END_EVENT_TABLE()

AuthenticationButton::AuthenticationButton(wxWindow *parent,
                                           wxWindowID id,
                                           wxBitmap normal,
                                           wxBitmap over,
                                           wxBitmap down,
                                           const wxBitmap &label,
                                           const wxString &button_text)
    : wxPanel(parent)
    , normal_(normal)
    , mouse_over_(over)
    , mouse_down_(down)
    , button_text_(button_text)
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , state_(MOUSE_EXIT)
{
    SetSize(normal_.GetSize());
}

// virtual
AuthenticationButton::~AuthenticationButton()
{
}

void AuthenticationButton::PaintNow()
{
    //wxPaintDC dc(this);
    //Render(dc);
}

void AuthenticationButton::OnMouseOver(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_OVER;
    Update();
}

void AuthenticationButton::OnMouseExit(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_EXIT;
    Update();
}

void AuthenticationButton::OnMouseDown(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_DOWN;
    Update();
}

void AuthenticationButton::OnMouseUp(wxMouseEvent &event)
{
    AuthenticationPopup *popup_parent = dynamic_cast<AuthenticationPopup*>(GetParent());

    if(popup_parent)
    {
        popup_parent->HandleButtonPress(GetId());
    }

    state_ = MOUSE_UP;
    Update();
    event.Skip();
}

void AuthenticationButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void AuthenticationButton::Render(wxDC &dc)
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    PrepareDC(dc);

    dc.SetFont(text_font_);
    dc.SetTextForeground(*wxWHITE);

    switch(state_)
    {
    case MOUSE_OVER:
        dc.DrawBitmap(mouse_over_, 0, 0);
        break;
    case MOUSE_EXIT:
        dc.DrawBitmap(normal_, 0, 0);
        dc.SetTextForeground(Constants::kIntelBlue);
        break;
    case MOUSE_DOWN:
        dc.DrawBitmap(mouse_down_, 0, 0);
        break;
    case MOUSE_UP:
        dc.DrawBitmap(mouse_over_, 0, 0);
        break;
    }
   
    int max_text_width = normal_.GetSize().x - normal_.GetSize().y - 6;
    wxSize text_size = dc.GetTextExtent(button_text_);
    while(text_size.x > max_text_width)
    {
        text_font_.SetPointSize(text_font_.GetPointSize() - 1);
        dc.SetFont(text_font_);
        text_size = dc.GetTextExtent(button_text_);
    }

    int left_offset = is_rtl_language ? 0 : GetSize().y;

    //Center text horizontally in the space we have
    int target_x = left_offset + 3 + ((max_text_width - text_size.x) * 0.5);
    dc.DrawText(button_text_, target_x, Constants::kAuthButtonTextY);
}
