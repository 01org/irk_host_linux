#include "AboutScreen.h"
#include "Constants.h"
#include <wx/textfile.h>
#include "../Resource Files/Version.h"
#include "Localization.h"
#include <vector>

BEGIN_EVENT_TABLE(AboutScreen, wxFrame)  
     EVT_CLOSE(AboutScreen::OnClose)
END_EVENT_TABLE()

AboutScreen::AboutScreen()
    : wxFrame(NULL,
              wxID_ANY,
              Localization::GetInstance()->GetString(LocalizedString::about_title),
              wxDefaultPosition,
              wxSize(Constants::kAboutScreenSizeX, Constants::kAboutScreenSizeY),
              wxCAPTION | wxCLOSE_BOX | wxFRAME_NO_TASKBAR)
    , title_(NULL)
    , version_info_(NULL)
    , product_description_(NULL)
    , licence_info_(NULL)
{
    SetBackgroundColour(wxColour(* wxWHITE));

    wxFont title_font(wxFontInfo().FaceName(Constants::kFontIntelClearProName).Encoding(wxFONTENCODING_SYSTEM));
    title_font.SetPointSize(Constants::kFontSizeAboutTitle * Constants::Helpers::GetPPIRatio());
    wxFont text_font(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM));
    text_font.SetPointSize(Constants::kFontSizeText * Constants::Helpers::GetPPIRatio());

    wxString title = Localization::GetInstance()->GetString(LocalizedString::about_title) + " " + Localization::GetInstance()->GetString(LocalizedString::product_name_long_server);
    wxString version = Localization::GetInstance()->GetString(LocalizedString::about_version);
    wxString text = Localization::GetInstance()->GetString(LocalizedString::about_info);

    title_ = new wxStaticText(this, wxID_ANY, title, wxPoint(Constants::kAboutScreenPadding, Constants::kAboutScreenPadding));
    title_->SetFont(title_font);
    title_->SetForegroundColour(Constants::kIntelBlue);

    // Adjust the Title fonts size smaller until it all fits on one line.
    int max_text_width = Constants::kAboutScreenSizeX - (2 * Constants::kAboutScreenPadding) - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2);
    wxSize text_size = title_->GetTextExtent(title);
    while (text_size.x > max_text_width)
    {
        title_font.SetPointSize(title_font.GetPointSize() - 1);
        title_->SetFont(title_font);
        text_size = title_->GetTextExtent(title);
    }

    wxString version_info_str = VER_PRODUCTVERSION_STR;

    version_info_ = new wxStaticText(this,
                                     wxID_ANY,
                                     version + version_info_str,
                                     wxPoint(title_->GetPosition().x, title_->GetPosition().y + title_->GetSize().y + Constants::kAboutScreenPadding));
    version_info_->Wrap(GetSize().x - version_info_->GetPosition().x - (2 * Constants::kAboutScreenPadding));

    product_description_ = new wxStaticText(this,
                                            wxID_ANY,
                                            text,
                                            wxPoint(version_info_->GetPosition().x, version_info_->GetPosition().y + version_info_->GetSize().y + Constants::kAboutScreenPadding));
    product_description_->SetFont(text_font);
    product_description_->Wrap(GetSize().x - product_description_->GetPosition().x - (2 * (Constants::kAboutScreenPadding)) - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2));

    max_text_width = Constants::kAboutScreenSizeX - (2 * Constants::kAboutScreenPadding) - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2);
    int font_size = DetermineFontSizeToFitWrappedText(product_description_->GetLabel(), max_text_width, *product_description_);

    //wxString file_path = wxString("Resources/") + Localization::GetInstance()->GetLanguageCode() + "/README.txt";
    wxString file_path = "README.txt";
    wxString licence_text;
    wxTextFile text_file; 
    text_file.Open(file_path);

    wxSize min_text_ctrl_size(100, 60);

    licence_info_ = new wxTextCtrl(this,
                                   wxID_ANY,
                                   wxEmptyString,
                                   wxPoint(product_description_->GetPosition().x, product_description_->GetPosition().y + product_description_->GetSize().y + Constants::kAboutScreenPadding),
                                   min_text_ctrl_size,
                                   wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY);

    licence_info_->SetSize(Constants::kAboutScreenSizeX - licence_info_->GetPosition().x - (2 * Constants::kAboutScreenPadding) - (wxSystemSettings::GetMetric(wxSYS_VSCROLL_X) * 0.5),
                           Constants::kAboutScreenSizeY - licence_info_->GetPosition().y - (2 * Constants::kAboutScreenPadding) - (wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y) * 2));

    (*licence_info_) << text_file.GetFirstLine() + "\n";

    while(!text_file.Eof())
    {
        (*licence_info_) << "\n";
        (*licence_info_) << text_file.GetNextLine();
    }

    Raise();
    Refresh();
}

void AboutScreen::OnClose(wxCloseEvent &event)
{
    Raise();
    Show(false);
}

int AboutScreen::DetermineFontSizeToFitWrappedText(wxString string, int max_width, wxStaticText& text_widget)
{
    wxString longestLine = GetLongestLine(string, text_widget);

    wxFont font = text_widget.GetFont();
    wxSize text_extent = text_widget.GetTextExtent(longestLine);
    wxSize widgetSize = text_widget.GetSize();
    while (text_extent.x > max_width)
    {
        font.SetPointSize(font.GetPointSize() - 1);
        text_widget.SetFont(font);
        text_widget.SetLabelText(string);
        text_widget.Wrap(GetSize().x - product_description_->GetPosition().x - (2 * (Constants::kAboutScreenPadding)) - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2));

        longestLine = GetLongestLine(string, text_widget);
        text_extent = text_widget.GetTextExtent(longestLine);
    }

    return font.GetPointSize();
}

wxString AboutScreen::GetLongestLine(wxString string, wxStaticText& text_widget)
{
    int startIndex = 0;
    int endIndex = 0;
    int longestStringIndex = 0;
    int max = 0;
    std::vector<wxString> splitStrings;

    while ((endIndex = string.find('\n', startIndex)) < string.size())
    {
        wxString value = string.substr(startIndex, endIndex - startIndex);
        splitStrings.push_back(value);
        startIndex = endIndex + 1;
    }
    if (startIndex < string.size())
    {
        wxString value = string.substr(startIndex);
        splitStrings.push_back(value);
    }

    for (int i = 0; i < splitStrings.size(); i++)
    {
        if (text_widget.GetTextExtent(splitStrings[i]).x >= max)
        {
            max = text_widget.GetTextExtent(splitStrings[i]).x;
            longestStringIndex = i;
        }
    }

    return splitStrings[longestStringIndex];
}
