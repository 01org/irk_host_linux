#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include "GCSFrame.h"
#include "OptionsPanel.h"
#include "DevicesPanel.h"
#include <wx/wx.h>
#include <wx/vlbox.h>
#include "../../GCSNativeInterface/OutputMessage.h"
#include "../../GCSNativeInterface/MessageHandler.h"

class SimpleImagePanel;
class OptionsPanel;
class GCSFrame;
class HelpButton;
class PipeConnection;
class StreamPipeBuffer;
class PipeListenerThread;

class GCSApp : public wxApp, public MessageHandler
{
public:
    virtual bool OnInit();
    int OnExit();
    void OnEndSession(wxCloseEvent& event);
    bool IsLanguageRTL();

    void ExitOnceSetup(wxString &message);

    void ProcessMessage(InputMessage *input_message);

private:
    void OnOptionsButtonClicked(wxCommandEvent &event);
    void AdjustPanelSizes();
    wxString AssignPlatformDependantFontname(wxString font_name);
    void InitializePrivateFonts();
    void RightToLeftLanguageCheck();

    GCSFrame *gcs_frame_;

    OptionsPanel *options_panel_;
    DevicesPanel *devices_panel_;
    HelpButton   *help_button_;
    bool is_language_rtl_;

    bool need_to_exit_;
    wxString exit_message_;
    
    DECLARE_EVENT_TABLE()
};
