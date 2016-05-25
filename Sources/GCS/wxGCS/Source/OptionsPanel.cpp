#include "OptionsPanel.h"
#include "Constants.h"


#include <iosfwd>
#include "GCSCheckBox.h"
#include "../../GCSNativeInterface/macros.h"
#include "OptionsPanelMainButton.h"
#include <sys/stat.h>
#include "Localization.h"
#include <wx/app.h> 
#include "main.h"
#include "OptionsPanelMainButton.h"

wxDECLARE_APP(GCSApp);

wxBEGIN_EVENT_TABLE(OptionsPanel, wxPanel)
    EVT_TEXT(Constants::kOptionsFriendlyNameText, OptionsPanel::OnFriendlyNameText)
    EVT_CHECKBOX(Constants::kOptionsRunAtStartupButton, OptionsPanel::OnRunAtStartupButton)
    EVT_CHECKBOX(Constants::kOptionsDoNotDisturbButton, OptionsPanel::OnDoNotDisturbButton)
    EVT_PAINT(OptionsPanel::OnPaint)
wxEND_EVENT_TABLE()

OptionsPanel::OptionsPanel(wxFrame *parent, int x, int y, int width, int height, wxBitmap main_options_button_image)
    : wxPanel(parent)
    , is_options_open_(false)
    , main_options_button_(NULL)
    , options_title_label_(Localization::GetInstance()->GetString(LocalizedString::options_tab_text))
    , title_font_(wxFontInfo().FaceName(Constants::kFontIntelClearProName).Encoding(wxFONTENCODING_SYSTEM))
    , computer_name_text_entry_(NULL)
    , divider_a_(NULL)
    , run_at_startup_checkbox_(NULL)
    , run_at_startup_label_(NULL)
    , divider_b_(NULL)
    , do_not_disturb_checkbox_(NULL)
    , do_not_disturb_label_(NULL)
    , doc_()
    , doc_name_()
{
    SetPosition(wxPoint(x,y));
    SetSize(wxSize(width,height));

    SetMainOptionsButton(main_options_button_image);
    
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    options_background_on_.LoadFile(is_rtl_language ? "Resources/reverse_UI/OptionsBG_ON.png" : "Resources/OptionsBG_ON.png", wxBITMAP_TYPE_PNG);
    options_background_off_.LoadFile(is_rtl_language ? "Resources/reverse_UI/OptionsBG_OFF.png" : "Resources/OptionsBG_OFF.png", wxBITMAP_TYPE_PNG);

    wxBitmap divider_bitmap;
    divider_bitmap.LoadFile("Resources/DividingLine.png", wxBITMAP_TYPE_PNG);

    // fonts
    title_font_.SetPointSize(Constants::kFontSizeTitle * Constants::Helpers::GetPPIRatio() * 100);

    wxFont friendly_name_font(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM));
    friendly_name_font.SetPointSize(Constants::kFontSizeFriendlyName * Constants::Helpers::GetPPIRatio());
    wxFont text_font(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM));
    text_font.SetPointSize(Constants::kFontSizeText * Constants::Helpers::GetPPIRatio());
    wxFont options_text_font(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM));
    options_text_font.SetPointSize(Constants::kFontSizeOptionsText * Constants::Helpers::GetPPIRatio());

    // friendly computer name
    wxString friendly_name_text = Localization::GetInstance()->GetString(LocalizedString::friendly_name_text);
    friendly_name_text.MakeCapitalized();
    computer_name_label_ = new wxStaticText(this, wxID_ANY, friendly_name_text, wxPoint(Constants::kOptionsFriendlyNameX, Constants::kOptionsFriendlyNameY));
    computer_name_label_->SetFont(friendly_name_font);
    computer_name_label_->SetForegroundColour(Constants::kIntelBlue);
    computer_name_text_entry_ = new wxTextCtrl(this, Constants::kOptionsFriendlyNameText, wxEmptyString, wxPoint(computer_name_label_->GetPosition().x, computer_name_label_->GetPosition().y  + computer_name_label_->GetSize().y + Constants::kOptionsDividerBuffer));
    computer_name_text_entry_->SetSize(wxSize(divider_bitmap.GetSize().x - (divider_bitmap.GetSize().x / 4) , computer_name_text_entry_->GetSize().y));
    computer_name_text_entry_->SetFont(text_font);

    // divider a
    divider_a_ = new wxStaticBitmap(this, wxID_ANY, divider_bitmap,
                                    wxPoint(0, computer_name_text_entry_->GetPosition().y + computer_name_label_->GetSize().y + Constants::kOptionsDividerBuffer));
    // run at startup
    run_at_startup_checkbox_ = new GCSCheckBox(this, Constants::kOptionsRunAtStartupButton, wxPoint(Constants::kOptionsCheckboxPadding, divider_a_->GetPosition().y + Constants::kOptionsDividerBuffer + Constants::kOptionsCheckOffset));
    run_at_startup_label_ = new wxStaticText(this, wxID_ANY, Localization::GetInstance()->GetString(LocalizedString::startup_checkbox_text), wxPoint(run_at_startup_checkbox_->GetPosition().x + run_at_startup_checkbox_->GetSize().x + Constants::kOptionsDividerBuffer, run_at_startup_checkbox_->GetPosition().y - Constants::kOptionsCheckOffset));
    run_at_startup_label_->Wrap(Constants::kWindowWidth - run_at_startup_label_->GetPosition().x - Constants::kOptionsWrapThreshold);
    run_at_startup_label_->SetFont(options_text_font);
    run_at_startup_label_->SetForegroundColour(Constants::kIntelBlue);

    //Center checkbox on text
    run_at_startup_checkbox_->SetPosition(wxPoint(run_at_startup_checkbox_->GetPosition().x, run_at_startup_label_->GetPosition().y + (run_at_startup_label_->GetSize().y * 0.5) - (run_at_startup_checkbox_->GetSize().y * 0.5)));
    

    // divider b
    divider_b_ = new wxStaticBitmap(this, wxID_ANY, divider_bitmap,
                                    wxPoint(0, run_at_startup_label_->GetPosition().y + run_at_startup_label_->GetSize().y + Constants::kOptionsDividerBuffer));

    // do not disturb
    do_not_disturb_checkbox_ = new GCSCheckBox(this, Constants::kOptionsDoNotDisturbButton, wxPoint(Constants::kOptionsCheckboxPadding, divider_b_->GetPosition().y + Constants::kOptionsDividerBuffer + Constants::kOptionsCheckOffset));
    do_not_disturb_label_ = new wxStaticText(this, wxID_ANY, Localization::GetInstance()->GetString(LocalizedString::do_not_disturb_checkbox_text), wxPoint(do_not_disturb_checkbox_->GetPosition().x + do_not_disturb_checkbox_->GetSize().x + Constants::kOptionsDividerBuffer, do_not_disturb_checkbox_->GetPosition().y - Constants::kOptionsCheckOffset));
    do_not_disturb_label_->Wrap(Constants::kWindowWidth - do_not_disturb_label_->GetPosition().x - Constants::kOptionsWrapThreshold);
    do_not_disturb_label_->SetFont(options_text_font);
    do_not_disturb_label_->SetForegroundColour(Constants::kIntelBlue);

    //Center checkbox on text
    do_not_disturb_checkbox_->SetPosition(wxPoint(do_not_disturb_checkbox_->GetPosition().x, do_not_disturb_label_->GetPosition().y + (do_not_disturb_label_->GetSize().y * 0.5) - (do_not_disturb_checkbox_->GetSize().y * 0.5)));

    SetBackgroundColour(parent->GetBackgroundColour());

    InitOptionsWithXML();
}

// virtual
OptionsPanel::~OptionsPanel()
{
}

void OptionsPanel::InitOptionsWithXML()
{
    doc_name_ = L"/var/lib/RemoteKeyboard/Options.xml";
    const int dir_err = mkdir("/var/lib/RemoteKeyboard/", S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH | S_IXOTH);

    if(!doc_.load_file(doc_name_.c_str()))
    {
        pugi::xml_node node = doc_.append_child(L"Options");
        
        std::wstring name;
        GCSFrame *gcs_frame = dynamic_cast<GCSFrame*>(GetParent());
        DeviceList *device_list = gcs_frame->GetDeviceList();
        if(device_list != NULL)
        {
            device_list->GetServerName(name);
        }
        
        doc_.child(L"Options").append_attribute(L"FriendlyName") = name.c_str();
        doc_.child(L"Options").append_attribute(L"bRunOnStartup") = L"true";
        doc_.child(L"Options").append_attribute(L"bDNDMode") = L"false";
        doc_.save_file(doc_name_.c_str());
    }

    wxString target_friendly_name = doc_.child(L"Options").attribute(L"FriendlyName").as_string();
    computer_name_text_entry_->SetValue(target_friendly_name);
    run_at_startup_checkbox_->SetValue(doc_.child(L"Options").attribute(L"bRunOnStartup").as_bool());
    do_not_disturb_checkbox_->SetValue(doc_.child(L"Options").attribute(L"bDNDMode").as_bool());

    doc_.save_file(doc_name_.c_str());
}

void OptionsPanel::SetMainOptionsButton(wxBitmap main_options_button_image)
{
    if(main_options_button_ == NULL)
    {
        wxBitmap empty = wxBitmap(0, 0);
        main_options_button_ = new OptionsPanelMainButton(this, Constants::kOptionsButton, empty, wxPoint(0,0), main_options_button_image.GetSize());
        main_options_button_->SetWindowStyle(wxBORDER_NONE);
        main_options_button_->SetBackgroundColour(*wxWHITE);
        SetSize(wxSize(main_options_button_image.GetSize()));
    }
}

void OptionsPanel::OptionsClicked()
{
    bool is_rtl_language = wxGetApp().IsLanguageRTL();
    
    if(!is_options_open_)
    {
        SetMainOptionsButton(options_background_on_);
        SetPosition(wxPoint(0, Constants::kOptionsOpenPosition));
        is_options_open_ = true;
    }
    else
    {
        SetMainOptionsButton(options_background_off_);
        SetPosition(wxPoint(0, Constants::kOptionsClosePosition));
        is_options_open_ = false;
    }

    Refresh();
}

bool OptionsPanel::GetIsOptionsOpen() const
{
    return is_options_open_;
}

void OptionsPanel::OnFriendlyNameText(wxCommandEvent &event)
{
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
		std::wstring friendly_name = computer_name_text_entry_->GetValue().ToStdWstring();
		doc_.child(L"Options").attribute(L"FriendlyName").set_value(friendly_name.c_str());
        doc_.save_file(doc_name_.c_str());

        GCSFrame *gcs_frame = dynamic_cast<GCSFrame*>(GetParent());
        DeviceList *device_list = gcs_frame->GetDeviceList();
        if(device_list != NULL)
        {
            device_list->SetServerName(friendly_name);
        }
    }
}

void OptionsPanel::OnRunAtStartupButton(wxCommandEvent &event)
{
    //wxLogWarning("Checkbox click");
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
        doc_.child(L"Options").attribute(L"bRunOnStartup").set_value(run_at_startup_checkbox_->GetValue());
        doc_.save_file(doc_name_.c_str());
        UpdateServerAutoStart();
    }
}

void OptionsPanel::OnDoNotDisturbButton(wxCommandEvent &event)
{
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
        doc_.child(L"Options").attribute(L"bDNDMode").set_value(do_not_disturb_checkbox_->GetValue());
        doc_.save_file(doc_name_.c_str());
        UpdateServerDNDMode();
    }
}

void OptionsPanel::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    doc_.reset();
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
        wxString target_friendly_name = doc_.child(L"Options").attribute(L"FriendlyName").as_string();
        computer_name_text_entry_->ChangeValue(target_friendly_name);
        run_at_startup_checkbox_->SetValue(doc_.child(L"Options").attribute(L"bRunOnStartup").as_bool());
        do_not_disturb_checkbox_->SetValue(doc_.child(L"Options").attribute(L"bDNDMode").as_bool());
    }
}

void OptionsPanel::UpdateServerDNDMode()
{
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
        GCSFrame *gcs_frame = dynamic_cast<GCSFrame*>(GetParent());
        gcs_frame->GetDeviceList()->SetDoNotDisturb(doc_.child(L"Options").attribute(L"bDNDMode").as_bool());
    }
}

void OptionsPanel::UpdateServerAutoStart()
{
    pugi::xml_parse_result result = doc_.load_file(doc_name_.c_str());
    if(result)
    {
        GCSFrame *gcs_frame = dynamic_cast<GCSFrame*>(GetParent());
        gcs_frame->GetDeviceList()->SetAutoStart(doc_.child(L"Options").attribute(L"bRunOnStartup").as_bool());
    }
}
