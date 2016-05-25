#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include <wx/wx.h>
#include <wx/intl.h>
#include <map>
#include <pugixml.hpp>

namespace LocalizedString
{
    enum LocalizedString
    {
        about_info,
        about_title,
        about_tool_strip_menu_text,
        about_version,
        applications_tab_text,
        apply_changes_button_part1_text,
        apply_changes_button_part2_text,
        authentication_text,
        cancel_button_text,
        device_tab_text,
        do_not_disturb_button_text,
        do_not_disturb_checkbox_text,
        do_not_disturb_notification_balloon_text,
        do_not_disturb_set_balloon_text,
        do_not_disturb_tool_strip_menu_text,
        exit_tool_strip_menu_text,
        first_launch_bubble_text,
        forget_all_button_part1_text,
        forget_all_button_part2_text,
        forget_button_text,
        friendly_name_text,
        help_text,
        html_page,
        intel_text,
        no_devices_part1_text,
        no_devices_part2_text,
        notify_adjust_settings,
        notify_disable_dnd,
        notify_manage_pairing_history,
        notify_open_add_remove,
        notify_open_network,
        notify_open_sensors_and_location,
        notify_show_applications,
        notify_state_dnd,
        notify_state_no_error,
        notify_state_no_network,
        notify_state_paired,
        notify_state_sensor_disabled,
        notify_state_uoip_error,
        options_confirmation_text,
        options_tab_text,
        options_window_title_text,
        pinned_start_menu_company_text,
        pinned_taskbar_company_text,
        product_name_long_client,
        product_name_long_server,
        product_name_short_client,
        product_name_short_server,
        restore_tool_strip_menu_text,
        scan_image_text,
        sensor_disable_bubble_text,
        startmenu_checkbox_text,
        startup_checkbox_text,
        taskbar_checkbox_text,
        tool_tip_apple,
        tool_tip_make_text,
        tool_tip_model_text,
        tool_tip_os_text,
        tool_tip_screen_text,
        tool_tip_wifi_supported_text,
        tool_tip_wifi_text,
        tool_tip_wifi_unsupported_text,
        uoip_failed_bubble_text,
        LOCALIZED_STRING_SIZE
    };
}


class Localization
{
public:
    wxString GetString(LocalizedString::LocalizedString string);
    wxString GetLanguageCode();
    static Localization* GetInstance();
    static void Destroy();
private:
    Localization();
    static void InitEnumStrings();
    static bool InitXMLDoc();
    static void InitLanguageCode(wxString canonical_name);
    static Localization* instance_;
    static wxLocale *locale_;
    static std::map<int, wxString> enumerated_strings_;
    static pugi::xml_document doc_;
    static wxString language_code_;
};
#endif //LOCALIZATION_H_