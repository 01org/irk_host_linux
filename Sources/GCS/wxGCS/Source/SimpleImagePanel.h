#ifndef WX_IMAGE_PANEL_H_
#define WX_IMAGE_PANEL_H_

#include <wx/wx.h>

class SimpleImagePanel : public wxPanel
{

public:
    SimpleImagePanel(wxFrame *parent, wxString file, wxBitmapType format);
    virtual ~SimpleImagePanel();
    void PaintNow();
    wxBitmap GetImage();
    void SetImage(wxString file, wxBitmapType format);

protected:
    wxBitmap image_;

private:
    void PaintEvent(wxPaintEvent & evt);
    void Render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};

#endif //WX_IMAGE_PANEL_H_