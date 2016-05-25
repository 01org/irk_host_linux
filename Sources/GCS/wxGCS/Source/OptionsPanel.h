#ifndef OPTIONS_PANEL_H_
#define OPTIONS_PANEL_H_

#include <wx/wx.h>
#include <pugixml.hpp>

#define MAX_PATH 260

class GCSCheckBox;
class OptionsPanelMainButton;

class OptionsPanel : public wxPanel
{
public:
    OptionsPanel(wxFrame *parent, int x, int y, int width, int height, wxBitmap main_options_button_image);
    virtual ~OptionsPanel();

    void InitOptionsWithXML();
    void SetMainOptionsButton(wxBitmap main_options_button_image);
    void OptionsClicked();
    bool GetIsOptionsOpen() const;
    void OnFriendlyNameText(wxCommandEvent &event);
    void OnRunAtStartupButton(wxCommandEvent &event);
    void OnDoNotDisturbButton(wxCommandEvent &event);
    void OnPaint(wxPaintEvent &event);
    void UpdateServerDNDMode();
    void UpdateServerAutoStart();

private:
    bool                    is_options_open_;
    OptionsPanelMainButton  *main_options_button_;
    wxString                options_title_label_;
    wxFont                  title_font_;
    wxStaticText            *computer_name_label_;
    wxTextCtrl              *computer_name_text_entry_;
    wxStaticBitmap          *divider_a_;
    GCSCheckBox             *run_at_startup_checkbox_;
    wxStaticText            *run_at_startup_label_;
    wxStaticBitmap          *divider_b_;
    GCSCheckBox             *do_not_disturb_checkbox_;
    wxStaticText            *do_not_disturb_label_;
    pugi::xml_document      doc_;
    std::wstring            doc_name_;
    wxBitmap                options_background_on_;
    wxBitmap                options_background_off_;

    wxDECLARE_EVENT_TABLE();
};

#endif //WX_IMAGE_PANEL_H_