#include "GCSSystemTrayIcon.h"
#include "Constants.h"
#include "GCSFrame.h"
#include <pugixml.hpp>
#include "Localization.h"
#include "AboutScreen.h"
#include <wx/app.h> 
#include "main.h"

wxDECLARE_APP(GCSApp);

AboutScreen *GCSSystemTrayIcon::about_screen_ = NULL;

GCSSystemTrayIcon::GCSSystemTrayIcon(wxFrame *parent)
    : wxTaskBarIcon()
    , main_frame_(NULL)
{
    main_frame_ = parent;
}

// virtual
GCSSystemTrayIcon::~GCSSystemTrayIcon()
{
}

void GCSSystemTrayIcon::OnMenuOpen(GtkWidget *widget, gpointer data)
{
    wxFrame *frame = dynamic_cast<wxFrame*>(wxGetApp().GetTopWindow());
    GCSSystemTrayIcon::HandleMenuOpen(frame);
}

void GCSSystemTrayIcon::OnMenuDoNotDisturb(GtkWidget *widget, gpointer data)
{
    wchar_t filename[MAX_PATH] = L"/var/lib/RemoteKeyboard/Options.xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    if(result)
    {
        gtk_check_menu_item_set_active(reinterpret_cast<GtkCheckMenuItem *>(widget), doc.child(L"Options").attribute(L"bDNDMode").as_bool());
    }
}

void GCSSystemTrayIcon::OnMenuExit(GtkWidget *widget, gpointer data)
{
    wxWindow *window = wxGetApp().GetTopWindow();
    wxFrame *frame = dynamic_cast<wxFrame*>(wxGetApp().GetTopWindow());
    GCSSystemTrayIcon::HandleMenuExit(frame);
}

void GCSSystemTrayIcon::OnMenuAbout(GtkWidget *widget, gpointer data)
{
    if(about_screen_ == NULL)
    {
        about_screen_ = new AboutScreen();
    }

    if(about_screen_ != NULL)
    {
        about_screen_->Centre();
        about_screen_->Show();
        about_screen_->Raise();
    }
}

void GCSSystemTrayIcon::HandleMenuOpen(wxFrame *frame)
{
    frame->Raise();
    frame->Show(true);
}

void GCSSystemTrayIcon::HandleMenuExit(wxFrame *frame)
{
    GCSFrame *gcs_frame = dynamic_cast<GCSFrame*>(frame);

    gcs_frame->Cleanup();
    gcs_frame->Hide();
    gcs_frame->Close();
    gcs_frame->Destroy();
    wxExit();
}

wxMenu *GCSSystemTrayIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu();
    menu->Append(Constants::kSystemTrayOpen, Localization::GetInstance()->GetString(LocalizedString::restore_tool_strip_menu_text));
    menu->AppendCheckItem(Constants::kSystemTrayDoNotDisturb, Localization::GetInstance()->GetString(LocalizedString::do_not_disturb_tool_strip_menu_text));
    menu->Append(Constants::kSystemTrayExit, Localization::GetInstance()->GetString(LocalizedString::exit_tool_strip_menu_text));
    menu->Append(Constants::kSystemTrayAbout, Localization::GetInstance()->GetString(LocalizedString::about_tool_strip_menu_text));

    return menu;
}
