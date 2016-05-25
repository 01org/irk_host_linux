#include "Localization.h"
#include "../../GCSNativeInterface/macros.h"
#include <wx/app.h> 
#include "main.h"

wxDECLARE_APP(GCSApp);

Localization* Localization::instance_ = NULL;
wxLocale* Localization::locale_ = NULL;
std::map<int, wxString> Localization::enumerated_strings_;
pugi::xml_document Localization::doc_;
wxString Localization::language_code_;

wxString Localization::GetString(LocalizedString::LocalizedString string)
{
    wxString requested_string;

    if(InitXMLDoc())
    {

        // get the string from the XML
        requested_string = doc_.child(language_code_).child(enumerated_strings_[string]).attribute(L"text").as_string();

        // replace any sub-string macros
        if(requested_string.Contains("%product_name_long_server%"))
        {
            requested_string.Replace("%product_name_long_server%",
                                     doc_.child(language_code_).child(enumerated_strings_[LocalizedString::product_name_long_server]).attribute(L"text").as_string());
        }

        if(requested_string.Contains("%product_name_long_client%"))
        {
            requested_string.Replace("%product_name_long_client%",
                                     doc_.child(language_code_).child(enumerated_strings_[LocalizedString::product_name_long_client]).attribute(L"text").as_string());
        }

        if(requested_string.Contains("%product_name_short_server%"))
        {
            requested_string.Replace("%product_name_short_server%",
                                     doc_.child(language_code_).child(enumerated_strings_[LocalizedString::product_name_short_server]).attribute(L"text").as_string());
        }

        if(requested_string.Contains("%product_name_short_client%"))
        {
            requested_string.Replace("%product_name_short_client%",
                                     doc_.child(language_code_).child(enumerated_strings_[LocalizedString::product_name_short_client]).attribute(L"text").as_string());
        }
    }

	if (requested_string == "")
	{
		//The file is bad, so let the user know and exit
            wxString error_string(L"There is a problem with your localization files. Please reinstall Intel Remote Keyboard.");
		wxGetApp().ExitOnceSetup(error_string);
	}

    return requested_string;
}

wxString Localization::GetLanguageCode()
{
    return language_code_;
}

Localization* Localization::GetInstance()
{
    if(!instance_)
    {
        instance_ = new Localization;
        locale_ = new wxLocale();
        locale_->Init();
        InitEnumStrings();
        wxString canonical_name = locale_->GetLanguageCanonicalName(locale_->GetLanguage());
        InitLanguageCode(canonical_name);
    }
    return instance_;
}

void Localization::Destroy()
{
    enumerated_strings_.clear();
    SAFE_DELETE(instance_);
    SAFE_DELETE(locale_);
}

Localization::Localization()
{
}

void Localization::InitEnumStrings()
{
    enumerated_strings_[LocalizedString::about_info]                                  = "about_info";
    enumerated_strings_[LocalizedString::about_title]                                 = "about_title";
    enumerated_strings_[LocalizedString::about_tool_strip_menu_text]                  = "about_tool_strip_menu_text";
    enumerated_strings_[LocalizedString::about_version]                               = "about_version";
    enumerated_strings_[LocalizedString::applications_tab_text]                       = "applications_tab_text";
    enumerated_strings_[LocalizedString::apply_changes_button_part1_text]             = "apply_changes_button_part1_text";
    enumerated_strings_[LocalizedString::apply_changes_button_part2_text]             = "apply_changes_button_part2_text";
    enumerated_strings_[LocalizedString::authentication_text]                         = "authentication_text";
    enumerated_strings_[LocalizedString::cancel_button_text]                          = "cancel_button_text";
    enumerated_strings_[LocalizedString::device_tab_text]                             = "device_tab_text";
    enumerated_strings_[LocalizedString::do_not_disturb_button_text]                  = "do_not_disturb_button_text";
    enumerated_strings_[LocalizedString::do_not_disturb_checkbox_text]                = "do_not_disturb_checkbox_text";
    enumerated_strings_[LocalizedString::do_not_disturb_notification_balloon_text]    = "do_not_disturb_notification_balloon_text";
    enumerated_strings_[LocalizedString::do_not_disturb_set_balloon_text]             = "do_not_disturb_set_balloon_text";
    enumerated_strings_[LocalizedString::do_not_disturb_tool_strip_menu_text]         = "do_not_disturb_tool_strip_menu_text";
    enumerated_strings_[LocalizedString::exit_tool_strip_menu_text]                   = "exit_tool_strip_menu_text";
    enumerated_strings_[LocalizedString::first_launch_bubble_text]                    = "first_launch_bubble_text";
    enumerated_strings_[LocalizedString::forget_all_button_part1_text]                = "forget_all_button_part1_text";
    enumerated_strings_[LocalizedString::forget_all_button_part2_text]                = "forget_all_button_part2_text";
    enumerated_strings_[LocalizedString::forget_button_text]                          = "forget_button_text";
    enumerated_strings_[LocalizedString::friendly_name_text]                          = "friendly_name_text";
    enumerated_strings_[LocalizedString::help_text]                                   = "help_text";
    enumerated_strings_[LocalizedString::html_page]                                   = "html_page";
    enumerated_strings_[LocalizedString::intel_text]                                  = "intel_text";
    enumerated_strings_[LocalizedString::no_devices_part1_text]                       = "no_devices_part1_text";
    enumerated_strings_[LocalizedString::no_devices_part2_text]                       = "no_devices_part2_text";
    enumerated_strings_[LocalizedString::notify_adjust_settings]                      = "notify_adjust_settings";
    enumerated_strings_[LocalizedString::notify_disable_dnd]                          = "notify_disable_dnd";
    enumerated_strings_[LocalizedString::notify_manage_pairing_history]               = "notify_manage_pairing_history";
    enumerated_strings_[LocalizedString::notify_open_add_remove]                      = "notify_open_add_remove";
    enumerated_strings_[LocalizedString::notify_open_network]                         = "notify_open_network";
    enumerated_strings_[LocalizedString::notify_open_sensors_and_location]            = "notify_open_sensors_and_location";
    enumerated_strings_[LocalizedString::notify_show_applications]                    = "notify_show_applications";
    enumerated_strings_[LocalizedString::notify_state_dnd]                            = "notify_state_dnd";
    enumerated_strings_[LocalizedString::notify_state_no_error]                       = "notify_state_no_error";
    enumerated_strings_[LocalizedString::notify_state_no_network]                     = "notify_state_no_network";
    enumerated_strings_[LocalizedString::notify_state_paired]                         = "notify_state_paired";
    enumerated_strings_[LocalizedString::notify_state_sensor_disabled]                = "notify_state_sensor_disabled";
    enumerated_strings_[LocalizedString::notify_state_uoip_error]                     = "notify_state_uoip_error";
    enumerated_strings_[LocalizedString::options_confirmation_text]                   = "options_confirmation_text";
    enumerated_strings_[LocalizedString::options_tab_text]                            = "options_tab_text";
    enumerated_strings_[LocalizedString::options_window_title_text]                   = "options_window_title_text";
    enumerated_strings_[LocalizedString::pinned_start_menu_company_text]              = "pinned_start_menu_company_text";
    enumerated_strings_[LocalizedString::pinned_taskbar_company_text]                 = "pinned_taskbar_company_text";
    enumerated_strings_[LocalizedString::product_name_long_client]                    = "product_name_long_client";
    enumerated_strings_[LocalizedString::product_name_long_server]                    = "product_name_long_server";
    enumerated_strings_[LocalizedString::product_name_short_client]                   = "product_name_short_client";
    enumerated_strings_[LocalizedString::product_name_short_server]                   = "product_name_short_server";
    enumerated_strings_[LocalizedString::restore_tool_strip_menu_text]                = "restore_tool_strip_menu_text";
    enumerated_strings_[LocalizedString::scan_image_text]                             = "scan_image_text";
    enumerated_strings_[LocalizedString::sensor_disable_bubble_text]                  = "sensor_disable_bubble_text";
    enumerated_strings_[LocalizedString::startmenu_checkbox_text]                     = "startmenu_checkbox_text";
    enumerated_strings_[LocalizedString::startup_checkbox_text]                       = "startup_checkbox_text";
    enumerated_strings_[LocalizedString::taskbar_checkbox_text]                       = "taskbar_checkbox_text";
    enumerated_strings_[LocalizedString::tool_tip_apple]                              = "tool_tip_apple";
    enumerated_strings_[LocalizedString::tool_tip_make_text]                          = "tool_tip_make_text";
    enumerated_strings_[LocalizedString::tool_tip_model_text]                         = "tool_tip_model_text";
    enumerated_strings_[LocalizedString::tool_tip_os_text]                            = "tool_tip_os_text";
    enumerated_strings_[LocalizedString::tool_tip_screen_text]                        = "tool_tip_screen_text";
    enumerated_strings_[LocalizedString::tool_tip_wifi_supported_text]                = "tool_tip_wifi_supported_text";
    enumerated_strings_[LocalizedString::tool_tip_wifi_text]                          = "tool_tip_wifi_text";
    enumerated_strings_[LocalizedString::tool_tip_wifi_unsupported_text]              = "tool_tip_wifi_unsupported_text";
    enumerated_strings_[LocalizedString::uoip_failed_bubble_text]                     = "uoip_failed_bubble_text";
}

bool Localization::InitXMLDoc()
{
    if(doc_.first_child().empty())
    {
        const char *localization_xml_path = "Resources/Localization/Localization.xml";
        pugi::xml_parse_result result = doc_.load_file(localization_xml_path);
        
        return result;
    }
    return true;
}

void Localization::InitLanguageCode(wxString canonical_name)
{
    if(canonical_name.StartsWith("ar"))
    {
        language_code_ = "ar";
    }
    else if (canonical_name.StartsWith("cs"))
    {
        language_code_ = "cs";
    }
    else if (canonical_name.StartsWith("da"))
    {
        language_code_ = "da";
    }
    else if(canonical_name.StartsWith("de"))
    {
        language_code_ = "de";
    }
    else if(canonical_name.StartsWith("en"))
    {
        language_code_ = "en";
    }
    else if(canonical_name.StartsWith("es"))
    {
        language_code_ = "es";
    }
    else if (canonical_name.StartsWith("fi"))
    {
        language_code_ = "fi";
    }
    else if(canonical_name.StartsWith("fr"))
    {
        language_code_ = "fr";
    }
    else if (canonical_name.StartsWith("he"))
    {
        language_code_ = "he";
    }
    else if(canonical_name.StartsWith("it"))
    {
        language_code_ = "it";
    }
    else if(canonical_name.StartsWith("ja"))
    {
        language_code_ = "ja";
    }
    else if(canonical_name.StartsWith("ko"))
    {
        language_code_ = "ko";
    }
    else if (canonical_name.StartsWith("nl"))
    {
        language_code_ = "nl";
    }
    else if (canonical_name.StartsWith("nb"))
    {
        language_code_ = "nb";
    }
    else if(canonical_name.StartsWith("pl"))
    {
        language_code_ = "pl";
    }
    else if(canonical_name.StartsWith("pt"))
    {
        language_code_ = "pt";
    }
    else if (canonical_name.StartsWith("ro"))
    {
        language_code_ = "ro";
    }
    else if(canonical_name.StartsWith("ru"))
    {
        language_code_ = "ru";
    }
    else if (canonical_name.StartsWith("sv"))
    {
        language_code_ = "sv";
    }
    else if (canonical_name.StartsWith("tr"))
    {
        language_code_ = "tr";
    }
    else if(canonical_name.StartsWith("zh") )
    {
        if(canonical_name.EndsWith("CN") || canonical_name.EndsWith("SG"))
        {
            language_code_ = "zh_CN"; // simplified
        }
        else
        {
            language_code_ = "zh_TW"; // traditional
        }
    }
    else
    {
        language_code_ = "en";
    }
}