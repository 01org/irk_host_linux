#include "GCSCheckBox.h"
#include "OptionsPanel.h"
#include <wx/app.h>
#include "main.h"

wxDECLARE_APP(GCSApp);

BEGIN_EVENT_TABLE(GCSCheckBox, wxPanel)
    EVT_PAINT(GCSCheckBox::OnPaint)
END_EVENT_TABLE()

GCSCheckBox::GCSCheckBox(wxWindow *parent, wxWindowID id, wxPoint position)
    : wxPanel(parent)
    , checkbox_selected_()
    , checkbox_unselected_()
    , state_(false)
    , id_(id)
{
    SetPosition(position);
    SetId(id_);
    
    checkbox_selected_.LoadFile("Resources/Checkbox_ON.png", wxBITMAP_TYPE_PNG);
    checkbox_unselected_.LoadFile("Resources/Checkbox_OFF.png", wxBITMAP_TYPE_PNG);

    // set size based on the properties of the image we're using
    SetSize(checkbox_selected_.GetSize());
    
    //Setup click event handlers
    Bind(wxEVT_LEFT_DOWN, &GCSCheckBox::OnMouseDown, this, id_);
    Bind(wxEVT_LEFT_UP, &GCSCheckBox::OnMouseUp, this, id_);
}

// virtual
GCSCheckBox::~GCSCheckBox()
{

}

bool GCSCheckBox::GetValue()
{
    return state_;
}

void GCSCheckBox::SetValue(bool new_state_)
{
    state_ = new_state_;
    Refresh();
    Update();
}

void GCSCheckBox::OnMouseDown(wxMouseEvent &event)
{
    Refresh();
    Update();
}

void GCSCheckBox::OnMouseUp(wxMouseEvent &event)
{
    OptionsPanel *panel_parent = dynamic_cast<OptionsPanel*>(GetParent());
    
    //toggle state
    SetValue(!state_);

    if(panel_parent != NULL)
    {
        wxCommandEvent event(wxEVT_CHECKBOX, id_);
        event.SetEventObject(this);
        panel_parent->ProcessWindowEvent(event);
    }
    
    event.Skip();
}

void GCSCheckBox::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    //wxLogWarning("Checkbox OnPaint");
    wxPaintDC dc(this);
    PrepareDC(dc);

    if(state_)
    {
        dc.DrawBitmap(checkbox_selected_, 0, 0, true);
    }
    else
    {
        dc.DrawBitmap(checkbox_unselected_, 0, 0, true);
    }
}