#ifndef ABOUT_SCREEN_H_
#define ABOUT_SCREEN_H_

#include <wx/wx.h>
#include "../../GCSNativeInterface/macros.h"

class AboutScreen : public wxFrame
{
public:
    AboutScreen();

protected:
    void OnClose(wxCloseEvent &event);

private:
    DISALLOW_COPY_AND_ASSIGN(AboutScreen);

    int DetermineFontSizeToFitWrappedText(wxString string, int max_width, wxStaticText& text_widget);
    wxString GetLongestLine(wxString string, wxStaticText& text_widget);

    wxStaticText            *title_;
    wxStaticText            *version_info_;
    wxStaticText            *product_description_;
    wxTextCtrl              *licence_info_;

    DECLARE_EVENT_TABLE()
};

#endif //ABOUT_SCREEN_H_