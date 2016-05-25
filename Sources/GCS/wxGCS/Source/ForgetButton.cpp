#include "ForgetButton.h"
#include "Constants.h"
#include "ForgetDevicePopup.h"
#include "Localization.h"
#include <wx/mousestate.h>

BEGIN_EVENT_TABLE(ForgetButton, wxPanel)
    EVT_ENTER_WINDOW(ForgetButton::OnMouseOver)
    EVT_LEAVE_WINDOW(ForgetButton::OnMouseExit)
    EVT_LEFT_DOWN(ForgetButton::OnMouseDown)
    EVT_LEFT_UP(ForgetButton::OnMouseUp)
    EVT_PAINT(ForgetButton::OnPaint)
END_EVENT_TABLE()

ForgetButton::ForgetButton(wxWindow *parent, const wxBitmap &label)
    : wxPanel(parent)
    , normal_()
    , mouse_over_()
    , mouse_down_()
    , close_icon_()
    , forget_(Localization::GetInstance()->GetString(LocalizedString::forget_button_text))
    , text_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
    , state_(MOUSE_EXIT)
{
    normal_.LoadFile("Resources/ForgetButton_OFF.png", wxBITMAP_TYPE_PNG);
    mouse_over_.LoadFile("Resources/ForgetButton_OVER.png", wxBITMAP_TYPE_PNG);
    mouse_down_.LoadFile("Resources/ForgetButton_PRESS.png", wxBITMAP_TYPE_PNG);
    close_icon_.LoadFile("Resources/IconForgetX.png", wxBITMAP_TYPE_PNG);

    SetSize(normal_.GetSize());
}

// virtual
ForgetButton::~ForgetButton()
{
}

void ForgetButton::OnMouseOver(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_OVER;
    Refresh();
}

void ForgetButton::OnMouseExit(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_EXIT;
    Refresh();
}

void ForgetButton::OnMouseDown(wxMouseEvent &WXUNUSED(event))
{
    state_ = MOUSE_DOWN;
    Refresh();
}

void ForgetButton::OnMouseUp(wxMouseEvent &event)
{
    ForgetDevicePopup *popup_parent = dynamic_cast<ForgetDevicePopup*>(GetParent());

    if(popup_parent != NULL)
    {
        popup_parent->OnForgetButtonClicked();
    }

    state_ = MOUSE_UP;
    Refresh();
    event.Skip();
}

void ForgetButton::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
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
        break;
    case MOUSE_DOWN:
        dc.DrawBitmap(mouse_down_, 0, 0);
        break;
    case MOUSE_UP:
        dc.DrawBitmap(mouse_over_, 0, 0);
        break;
    }

    dc.DrawBitmap(close_icon_, Constants::kPopupButtonIconX, Constants::kPopupButtonIconY);
    dc.DrawText(forget_, Constants::kPopupForgetPositionX, Constants::kPopupForgetPositionY);
}
