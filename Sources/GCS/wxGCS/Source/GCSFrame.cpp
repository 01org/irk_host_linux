#include "GCSFrame.h"
#include "Constants.h"

#include <vector>
#include "../../GCSNativeInterface/Server.h"
#include "DeviceList.h"
#include "Localization.h"

#include <fstream>

namespace
{
    static const int kFullGUIDStringLength = 39;
    static const int kGUIDStringLength = kFullGUIDStringLength - 2; //FullGUID has braces, which are removed for the string we use
}

BEGIN_EVENT_TABLE(GCSFrame, wxFrame)
     EVT_PAINT(GCSFrame::OnPaint)
     EVT_TIMER(Constants::kServerTimer, GCSFrame::OnRefreshTimer)
     EVT_CLOSE(GCSFrame::OnClose)
     EVT_ACTIVATE(GCSFrame::OnAltTab)
END_EVENT_TABLE()

GCSFrame::GCSFrame(const wxString &title)
    : wxFrame(NULL,
              wxID_ANY,
              title,
              wxDefaultPosition,
              wxSize(Constants::kWindowWidth + (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X)),
              Constants::kWindowHeight + Constants::kTitleBarSize),
              wxCAPTION | wxCLOSE_BOX | wxFRAME_NO_TASKBAR)
    , indicator_(NULL)
    , handlers_(NULL)
    , widgets_(NULL)
    , server_(NULL)
    , refresh_timer_(NULL)
    , device_list_reference_(NULL)
    , app_name_(Localization::GetInstance()->GetString(LocalizedString::product_name_long_server))
    , title_font_(wxFontInfo().FaceName(Constants::kFontIntelClearName).Encoding(wxFONTENCODING_SYSTEM))
{
    wxScreenDC screen_device_context;
    int current_ppi = screen_device_context.GetPPI().x;
    float ppi_ratio = static_cast<float>(current_ppi) / static_cast<float>(Constants::kStandardPPI); // in this case we're looking for the reciprocal for increasing the frame size dependant on the accessibility features

    if(ppi_ratio != 1.0f)
    {
        SetSize(GetSize().x, Constants::kWindowHeight + Constants::kTitleBarSize * ppi_ratio);
    }

    title_font_.SetWeight(wxFONTWEIGHT_NORMAL);
    title_font_.SetPointSize(Constants::kFontSizeText * Constants::Helpers::GetPPIRatio());

    Centre();
}

void GCSFrame::Init()
{
    InitSystemTrayIcon();
    
    //start the server TODO: check if any other initialization is required
    server_ = new Server(SERVER_TYPE_DESKTOP);
    GUID store_id;
    GUID server_id;

    GetStoreGUID(&store_id);
    GetServerGUID(&server_id);
    server_->Init(store_id, server_id);

    refresh_timer_ = new wxTimer(this, Constants::kServerTimer);
    refresh_timer_->Start(10); // fire off every 10ms
}

Server* GCSFrame::GetServer()
{
    return server_;
}

DeviceList* GCSFrame::GetDeviceList()
{
    return device_list_reference_;
}

void GCSFrame::SetDeviceList(DeviceList *device_list)
{
    device_list_reference_ = device_list;
}

void GCSFrame::Exit()
{
    Cleanup();
    Hide();
    Close();
    Destroy();
    wxExit();
}

void GCSFrame::Cleanup()
{
    if(refresh_timer_ != NULL)
    {
        refresh_timer_->Stop();
        SAFE_DELETE(refresh_timer_);
    }

    if(server_ != NULL)
    {
        server_->Uninit();
        SAFE_DELETE(server_);
    }
    
    if(widgets_ != NULL)
    {
        for(int i = 0; i < widgets_->size(); ++i)
        {
            g_signal_handler_disconnect(widgets_->at(i), handlers_->at(i));
        }
        SAFE_DELETE(widgets_);
    }
    SAFE_DELETE(handlers_);
}

void GCSFrame::UpdateSystemTrayIcon(IconDisplayStatus icon_display_status)
{
    switch (icon_display_status)
    {
    case ON:
        app_indicator_set_icon_full(indicator_, "GCSTray-32x32", "Remote Keyboard");
        break;
    case DISCONNECTED:
        app_indicator_set_icon_full(indicator_, "GCSTrayDisc-32x32", "Remote Keyboard");
        break;
    case ON_ERROR:
        app_indicator_set_icon_full(indicator_, "GCSTrayErr-32x32", "Remote Keyboard");
        break;
    case OFF:
        app_indicator_set_icon_full(indicator_, "GCSTrayOff-32x32", "Remote Keyboard");
        break;
    case OFF_ERROR:
        app_indicator_set_icon_full(indicator_, "GCSTrayOffErr-32x32", "Remote Keyboard");
        break;
    }
}

void GCSFrame::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC paint_dc(this);
    paint_dc.DrawBitmap(wxBitmap(wxImage("Resources/Background.png")), 0, 0);
    paint_dc.SetTextForeground(Constants::kIntelYellow);
    paint_dc.SetFont(title_font_);
    wxSize app_name_size = paint_dc.GetTextExtent(app_name_);
    paint_dc.DrawText(app_name_, wxPoint(Constants::kWindowWidth - paint_dc.GetTextExtent(app_name_).GetX() - Constants::kTitleLabelXRightPadding - (wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_X) * 2), Constants::kAppNameY));
}

void GCSFrame::OnRefreshTimer(wxTimerEvent &WXUNUSED(event))
{
    if(server_ && device_list_reference_ != NULL)
    {
        server_->UpdateClientConnections();
        if(device_list_reference_->GetShouldShowAuthentication())
        {
            device_list_reference_->DisplayAuthenticationHandler();
            device_list_reference_->SetShouldShowAuthentication(false);
        }
        if(device_list_reference_->GetShouldHideAuthentication())
        {
            device_list_reference_->HideAuthenticationHandler();
            device_list_reference_->SetShouldHideAuthentication(false);
        }
        if(device_list_reference_->GetShouldCancelAuthentication())
        {
            device_list_reference_->OnCanceledAuthenticationHandler();
            device_list_reference_->SetShouldCancelAuthentication(false);
        }
        if(device_list_reference_->GetShouldRefreshDeviceList())
        {
            device_list_reference_->RefreshDeviceListHandler();
            device_list_reference_->SetShouldRefreshDeviceList(false);
        }

        if (server_->ShouldShow())
        {
            Show(true);
            server_->Show(true);
        }
        else if (server_->ShouldHide())
        {
            Show(false);
            server_->Show(false);
        }
    }
}

void GCSFrame::InitSystemTrayIcon()
{
    handlers_ = new std::vector<gulong>();
    widgets_ = new std::vector<gpointer>();
    GtkWidget *menu = NULL;

    indicator_ = app_indicator_new("wxGCS-appindicator", "wxGCS-AppIndicator", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    menu = gtk_menu_new();

    const int MAX_PATH = 260;
    const char *relative_path = "Resources/icons/";
    char buffer[MAX_PATH];
    char *absolute_path = realpath(relative_path, buffer);

    app_indicator_set_icon_theme_path(indicator_, absolute_path);
    app_indicator_set_icon_full(indicator_, "GCSTrayOff-32x32", "Remote Keyboard");

    GtkWidget *open_item = gtk_menu_item_new_with_label(Localization::GetInstance()->GetString(LocalizedString::restore_tool_strip_menu_text));
    GtkWidget *do_not_disturb_item = gtk_check_menu_item_new_with_label(Localization::GetInstance()->GetString(LocalizedString::do_not_disturb_tool_strip_menu_text));
    GtkWidget *close_item = gtk_menu_item_new_with_label(Localization::GetInstance()->GetString(LocalizedString::exit_tool_strip_menu_text));
    GtkWidget *about_item = gtk_menu_item_new_with_label(Localization::GetInstance()->GetString(LocalizedString::about_tool_strip_menu_text));

    gulong handle;
    handle = g_signal_connect(open_item, "activate", G_CALLBACK(GCSSystemTrayIcon::OnMenuOpen), this);
    handlers_->push_back(handle);
    widgets_->push_back(open_item);
    handle = g_signal_connect(do_not_disturb_item, "activate", G_CALLBACK(GCSSystemTrayIcon::OnMenuDoNotDisturb), this);
    handlers_->push_back(handle);
    widgets_->push_back(do_not_disturb_item);
    handle = g_signal_connect(close_item, "activate", G_CALLBACK(GCSSystemTrayIcon::OnMenuExit), this);
    handlers_->push_back(handle);
    widgets_->push_back(close_item);
    handle = g_signal_connect(about_item, "activate", G_CALLBACK(GCSSystemTrayIcon::OnMenuAbout), this);
    handlers_->push_back(handle);
    widgets_->push_back(about_item);

    gtk_menu_shell_append (GTK_MENU_SHELL(menu), open_item);
    gtk_menu_shell_append (GTK_MENU_SHELL(menu), do_not_disturb_item);
    gtk_menu_shell_append (GTK_MENU_SHELL(menu), close_item);
    gtk_menu_shell_append (GTK_MENU_SHELL(menu), about_item);

    gtk_widget_show(open_item);
    gtk_widget_show(do_not_disturb_item);
    gtk_widget_show(close_item);
    gtk_widget_show(about_item);

    app_indicator_set_status(indicator_, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu (indicator_, GTK_MENU (menu));
}

GUID GCSFrame::RetrieveGUID(const wchar_t *name)
{
    const int MAX_PATH = 260; //not defined in linux
    std::string path = "/var/lib/RemoteKeyboard/";
    std::string file_name = path;
    std::wstring wide_name = name;
    std::string name_string;
    name_string.assign(wide_name.begin(), wide_name.end());
    file_name += name_string;
    file_name += ".txt";
    const int dir_err = mkdir("/var/lib/RemoteKeyboard/", S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH | S_IXOTH);
    if (-1 == dir_err)
    {
    	//error check here
        //printf("Error creating directory!n");
        //exit(1);
    }
    
    GUID guid;

    FILE *file = fopen(file_name.c_str(), "rb");
    if(file != NULL)
    {
        fread(&guid, sizeof(GUID), 1, file);
        fclose(file);
    }
    else
    {
        //file must not exist, so generate a new GUID and store it
        UuidCreate(&guid);

        file = fopen(file_name.c_str(), "wb");
        if(file != NULL)
        {
            fwrite(&guid, sizeof(GUID), 1, file);
            fclose(file);
        }
    }

    return guid;
}

void GCSFrame::GetServerGUID(GUID *guid)
{
   *guid = RetrieveGUID(L"Server_ID");
}

void GCSFrame::GetStoreGUID(GUID *guid)
{
   *guid = RetrieveGUID(L"Store_ID");
}

void GCSFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    Show(false);
}

void GCSFrame::OnAltTab(wxActivateEvent &WXUNUSED(event))
{
    device_list_reference_->Refresh();
    Refresh();
}
