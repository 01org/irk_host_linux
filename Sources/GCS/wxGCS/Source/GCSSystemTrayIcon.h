#ifndef GCS_SYSTEM_TRAY_ICON_H_
#define GCS_SYSTEM_TRAY_ICON_H_

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <app-indicator.h>
#include <gtk/gtk.h>

class AboutScreen;

class GCSSystemTrayIcon : public wxTaskBarIcon
{
public:
    GCSSystemTrayIcon(wxFrame *parent);
    virtual ~GCSSystemTrayIcon();

    static void OnMenuOpen(GtkWidget *widget, gpointer data);
    static void OnMenuDoNotDisturb(GtkWidget *widget, gpointer data);
    static void OnMenuExit(GtkWidget *widget, gpointer data);
    static void OnMenuAbout(GtkWidget *widget, gpointer data);

    //static void HandleLeftButton();
    static void HandleMenuOpen(wxFrame *frame);
    //static void HandleMenuDoNotDisturb();
    //static void HandleMenuUpdateDoNotDisturb();
    static void HandleMenuExit(wxFrame *frame);

    virtual wxMenu *CreatePopupMenu();

private:
    wxFrame *main_frame_;
    static AboutScreen *about_screen_;
};

#endif // GCS_SYSTEM_TRAY_ICON_H_
