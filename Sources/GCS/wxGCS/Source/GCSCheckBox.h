#include <wx/wx.h>

class GCSCheckBox : public wxPanel
{
public:
    GCSCheckBox(wxWindow *parent, wxWindowID id, wxPoint position);
    virtual ~GCSCheckBox();
    
    bool GetValue();
    void SetValue(bool new_state_);

private:
    void OnMouseDown(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    wxBitmap checkbox_selected_;
    wxBitmap checkbox_unselected_;
    bool state_;
    wxWindowID id_;

    DECLARE_EVENT_TABLE()
};