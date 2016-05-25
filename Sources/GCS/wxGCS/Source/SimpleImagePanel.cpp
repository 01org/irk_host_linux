#include "SimpleImagePanel.h"
#include <wx/dcbuffer.h>
#include "Constants.h"
 
BEGIN_EVENT_TABLE(SimpleImagePanel, wxPanel)
    EVT_PAINT(SimpleImagePanel::PaintEvent)
END_EVENT_TABLE()

SimpleImagePanel::SimpleImagePanel(wxFrame *parent, wxString file, wxBitmapType format)
    : wxPanel(parent)
{
    image_.LoadFile(file, format);
    SetSize(wxSize(image_.GetSize()));
}

// virtual
SimpleImagePanel::~SimpleImagePanel()
{
}

void SimpleImagePanel::PaintNow()
{
    wxClientDC dc(this);
    Render(dc);
}

wxBitmap SimpleImagePanel::GetImage()
{
    return image_;
}

void SimpleImagePanel::SetImage(wxString file, wxBitmapType format)
{
    image_.LoadFile(file, format);
    wxSize newSize = image_.GetSize();
    SetSize(newSize);
}

void SimpleImagePanel::PaintEvent(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    Render(dc);
}

void SimpleImagePanel::Render(wxDC &dc)
{
    dc.DrawBitmap(image_, 0, 0, false);
}
