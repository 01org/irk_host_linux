#ifndef GCS_FRAME_H_
#define GCS_FRAME_H_

#include <wx/wx.h>
#include "GCSSystemTrayIcon.h"
#include <sstream>
#include <app-indicator.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <vector>
#include "../../GCSNativeInterface/GUID.h"

 #define DebugOut(s)            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

class Server;
class DeviceList;

class GCSFrame : public wxFrame
{
public:
    enum IconDisplayStatus
    {
        ON,
        DISCONNECTED,
        ON_ERROR,
        OFF,
        OFF_ERROR
    };

    GCSFrame(const wxString &title);
    virtual void Init();
    Server *GetServer();
    DeviceList *GetDeviceList();
    void SetDeviceList(DeviceList *device_list);
    virtual void Exit();
    virtual void Cleanup();
    void UpdateSystemTrayIcon(IconDisplayStatus icon_display_status);

protected:
    virtual void OnRefreshTimer(wxTimerEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnClose(wxCloseEvent &event);
    void OnAltTab(wxActivateEvent &event);
    void InitSystemTrayIcon();
    
    AppIndicator *indicator_;
    std::vector<gulong> *handlers_;
    std::vector<gpointer> *widgets_;

    wxTimer             *refresh_timer_;
    DeviceList          *device_list_reference_;
    Server              *server_;
    wxString            app_name_;
    wxFont              title_font_;

private:
    GUID RetrieveGUID(const wchar_t *name);
    
    void GetServerGUID(GUID *guid);
    void GetStoreGUID(GUID *guid);


    DECLARE_EVENT_TABLE()
};

#endif //GCS_FRAME_H_
