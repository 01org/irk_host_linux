#include "main.h"
#include "Constants.h"
#include "SimpleImagePanel.h"
#include "DeviceList.h"
#include <wx/arrstr.h>
#include <wx/splash.h>
#include <wchar.h>
#include "HelpButton.h"
#include "QRGenerator.h"
#include "../../GCSNativeInterface/UICallbacks.h"
#include "../../GCSNativeInterface/Server.h"
#include "Localization.h"
#include <wx/intl.h>
#include <pugixml.hpp>
#include <fstream>
#include <app-indicator.h>
#include <gtk/gtk.h>

IMPLEMENT_APP(GCSApp)
DECLARE_APP(GCSApp)

BEGIN_EVENT_TABLE(GCSApp, wxApp)
EVT_QUERY_END_SESSION(GCSApp::OnEndSession)
EVT_END_SESSION(GCSApp::OnEndSession)
END_EVENT_TABLE()

bool GCSApp::OnInit()
{
    //Initialize member variables as we don't have a constructor
    gcs_frame_ = NULL;
    options_panel_ = NULL;
    devices_panel_ = NULL;
    help_button_ = NULL;
    is_language_rtl_ = true;

    need_to_exit_ = false;
    exit_message_ = L"";

    wxInitAllImageHandlers();
    InitializePrivateFonts();

    // before we create any windows, check our locale
    RightToLeftLanguageCheck();

    // Create and show our frame. Everything is encapsulated inside this.
    gcs_frame_ = new GCSFrame(Localization::GetInstance()->GetString(LocalizedString::product_name_long_server));

    gcs_frame_->Init();
    gcs_frame_->SetBackgroundColour(wxColour(255, 255, 255, 255));
    
    // Devices
    wxBitmap devices_button_image;
    
    devices_button_image.LoadFile((is_language_rtl_) ? "Resources/reverse_UI//DevicesBG_ON.png" : "Resources/DevicesBG_ON.png", wxBITMAP_TYPE_PNG);
    devices_panel_ = new DevicesPanel(gcs_frame_,
                                      0,
                                      Constants::kDevicesPosition,
                                      devices_button_image.GetSize().x,
                                      devices_button_image.GetSize().y,
                                      devices_button_image);

    // Set up a reference to the device list inside the main frame (this is used for the server's callbacks)
    DeviceList* the_device_list = dynamic_cast<DeviceList*>(wxWindow::FindWindowById(Constants::kDeviceList));
    gcs_frame_->SetDeviceList(the_device_list);

    // Options
    wxBitmap options_button_image;//(100, 100);
    options_button_image.LoadFile((is_language_rtl_) ? "Resources/reverse_UI/OptionsBG_OFF.png" : "Resources/OptionsBG_OFF.png", wxBITMAP_TYPE_PNG);
    options_panel_ = new OptionsPanel(gcs_frame_,
                                      0,
                                      Constants::kOptionsClosePosition,
                                      options_button_image.GetSize().x,
                                      options_button_image.GetSize().y,
                                      options_button_image);

    // Hook up our main panel buttons
    Connect(Constants::kOptionsButton, wxEVT_LEFT_DOWN, wxCommandEventHandler(GCSApp::OnOptionsButtonClicked));
    Connect(Constants::kOptionsLabel, wxEVT_LEFT_DOWN, wxCommandEventHandler(GCSApp::OnOptionsButtonClicked));
    Connect(Constants::kDevicesButton, wxEVT_LEFT_DOWN, wxCommandEventHandler(GCSApp::OnOptionsButtonClicked));

    // initialize the UI callbacks
    UICallbacks::SetUIListener(devices_panel_->GetDeviceList());

    // create the help button
    wxBitmap help_button_off;
    help_button_off.LoadFile("Resources/IconHelp_OFF.png", wxBITMAP_TYPE_PNG);
    help_button_ = new HelpButton(gcs_frame_, help_button_off);
    wxPoint uncentered_position(Constants::kHelpButtonPositionX, Constants::kOptionsClosePosition + options_button_image.GetSize().y + 5);
    int help_area_height = Constants::kWindowHeight - uncentered_position.y - (Constants::kHelpButtonPadding * 2) - wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X);
    help_button_->SetPosition(uncentered_position);
    help_button_->SetSize(Constants::kWindowWidth - Constants::kHelpButtonPositionX, help_area_height);

    // create and show splash screen
    if(!need_to_exit_)
    {
        //Only show the splash screen if an error hasn't been encoutered yet
        wxBitmap splash_image;
        splash_image.LoadFile("Resources/SplashPage.png", wxBITMAP_TYPE_PNG);
        wxSplashScreen *splash = new wxSplashScreen(splash_image,
            wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
            2000,
            gcs_frame_,
            wxID_ANY,
            wxDefaultPosition,
            wxDefaultSize,
            wxSIMPLE_BORDER | wxSTAY_ON_TOP);
    }

    if(need_to_exit_)
    {
        gcs_frame_->Show();
        gcs_frame_->Raise();
        
        wxMessageDialog *error_dialog = new wxMessageDialog(gcs_frame_, exit_message_, L"Error", wxOK | wxCENTRE | wxSTAY_ON_TOP | wxICON_ERROR);
        error_dialog->ShowModal();
        SAFE_DELETE(error_dialog);
        gcs_frame_->Exit();
    }

    return true;
}

void GCSApp::OnEndSession(wxCloseEvent &WXUNUSED(event))
{
}

bool GCSApp::IsLanguageRTL()
{
    return is_language_rtl_;
}

void GCSApp::ExitOnceSetup(wxString &message)
{
    need_to_exit_ = true;
    exit_message_ = message;
}

int GCSApp::OnExit()
{
    gcs_frame_->Hide();
    gcs_frame_->Cleanup();
    QRGenerator::Destroy();
    Localization::Destroy();

    return 0;
}

void GCSApp::OnOptionsButtonClicked(wxCommandEvent &WXUNUSED(event))
{
    options_panel_->OptionsClicked();
    devices_panel_->OptionsClicked(options_panel_->GetIsOptionsOpen());
    AdjustPanelSizes();
}

void GCSApp::AdjustPanelSizes() // may be hacky for now, but assures our panels don't overlap and have layering/mouse priority issues
{
    if(options_panel_->GetIsOptionsOpen())
    {
        devices_panel_->SetSize(wxSize(devices_panel_->GetSize().x, Constants::kDevicesPanelCloseSizeY));
        options_panel_->SetSize(wxSize(devices_panel_->GetSize().x, Constants::kOptionsPanelOpenSizeY));
    }
    else
    {
        devices_panel_->SetSize(wxSize(devices_panel_->GetSize().x, Constants::kDevicesPanelOpenSizeY));
        options_panel_->SetSize(wxSize(devices_panel_->GetSize().x, Constants::kOptionsPanelCloseSizeY));
    }

    options_panel_->Refresh();
    devices_panel_->Refresh();
}

wxString GCSApp::AssignPlatformDependantFontname(wxString font_name)
{
    wxString private_font;
    private_font.Append(wxT("Resources" + Constants::kPathSeparator + "fonts" + Constants::kPathSeparator + font_name + ".ttf"));

    return private_font;
}

void GCSApp::InitializePrivateFonts()
{
    wxArrayString *fonts = new wxArrayString();
    wxString intel_clear_font = AssignPlatformDependantFontname(Constants::kFontIntelClearRg);
    fonts->Add(intel_clear_font);

    wxString intel_clear_font_italic = AssignPlatformDependantFontname(Constants::kFontIntelClearIt);
    fonts->Add(intel_clear_font_italic);

    wxString intel_clear_font_light = AssignPlatformDependantFontname(Constants::kFontIntelClearLt);
    fonts->Add(intel_clear_font_light);

    wxString intel_clear_font_light_italic = AssignPlatformDependantFontname(Constants::kFontIntelClearLtIt);
    fonts->Add(intel_clear_font_light_italic);

    wxString intel_clear_font_bold = AssignPlatformDependantFontname(Constants::kFontIntelClearBd);
    fonts->Add(intel_clear_font_bold);

    wxString intel_clear_font_bold_italic = AssignPlatformDependantFontname(Constants::kFontIntelClearBdIt);
    fonts->Add(intel_clear_font_bold_italic);

    wxString intel_clear_font_pro = AssignPlatformDependantFontname(Constants::kFontIntelClearProBd);
    fonts->Add(intel_clear_font_pro);

    for(std::size_t i = 0; i < fonts->GetCount(); ++i)
    {
        bool can_activate_private_fonts = wxFont::AddPrivateFont(fonts->Item(i));
        
        if(!can_activate_private_fonts)
        {
            wxLogError("Initializing fonts adding failed for %s", fonts->Item(i));
            continue;
        }
        if (can_activate_private_fonts)
        {
            can_activate_private_fonts = wxFont::ActivatePrivateFonts();

            if(!can_activate_private_fonts)
            {
                wxLogError("Initializing fonts activating failed for %s", fonts->Item(i));
                continue;
            }
        }
    }

    SAFE_DELETE(fonts);
}

void GCSApp::RightToLeftLanguageCheck()
{
    is_language_rtl_ = false;

    wxLocale locale;
    locale.Init();
    wxString language_code = locale.GetCanonicalName();

    if(language_code.StartsWith("ar"))
    {
        is_language_rtl_ = true;
    }
}

void GCSApp::ProcessMessage(InputMessage* input_message)
{
}
