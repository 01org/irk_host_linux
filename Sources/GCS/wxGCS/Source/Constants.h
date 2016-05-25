#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <wx/wx.h>

namespace Constants
{
    enum IDs
    {
        kOptionsButton,
        kOptionsLabel,
        kDevicesButton,
        kServerTimer,
        kDeviceList,
        kSystemTrayOpen,
        kSystemTrayDoNotDisturb,
        kSystemTrayExit,
        kSystemTrayAbout,
        kOptionsFriendlyNameText,
        kOptionsRunAtStartupButton,
        kOptionsDoNotDisturbButton,
        kUpdateTimer
    };

    static const int        kWindowHeight           = 519;
    static const int        kWindowWidth            = 310;

    static const int        kHelpButtonPositionX    = 12;
    static const int        kHelpButtonPositionY    = 464;

    static const int        kAboutScreenSizeX       = 600;
    static const int        kAboutScreenSizeY       = 830;
    static const int        kAboutScreenPadding     = 6;

    static const int        kMainIconX              = 12;
    static const int        kMainIconY              = 8;
    static const int        kMainIconOpenY          = 11;
    static const int        kMainIconCloseY         = 24;
    static const int        kAppNameX               = 150;
    static const int        kAppNameY               = 45;

    static const int        kIntelLogoPositionX     = 25;
    static const int        kIntelLogoPositionY     = 15;
    static const int        kDevicesPosition        = 70;
    static const int        kOptionsClosePosition   = 385;
    static const int        kOptionsOpenPosition    = 119;
    static const int        kDevicesPanelOpenSizeY  = 315;
    static const int        kDevicesPanelCloseSizeY = 65;
    static const int        kOptionsPanelOpenSizeY  = 314;
    static const int        kOptionsPanelCloseSizeY = 66;
    static const int        kOptionsCheckboxPadding = 15;
    static const int        kOptionsWrapThreshold   = 30;

    static const int        kTitleLabelX            = 15;
    static const int        kTitleLabelXRightPadding = 5;
    static const int        kTitleLabelOpenY        = -6;
    static const int        kTitleLabelCloseY       = 10;
    static const int        kTitleLabelOpenYRTL     = 4;
    static const int        kTitleLabelCloseYRTL    = 18;

    static const int        kTitleLabelYOffset      = 6;

    static const int        kOptionsFriendlyNameX   = 15;
    static const int        kOptionsFriendlyNameY   = 65;

    static const int        kOptionsDividerBuffer   = 10;
    static const int        kDeviceDividerBuffer    = 2;
    static const int        kOptionsCheckOffset     = -3;

    static const int        kHelpButtonPadding      = 3;
    static const int        kHelpTextPadding        = 5;
    static const int        kHelpTextPositionY      = -12;

    static const int        kDeviceListEntryX       = 5;

    static const int        kDeviceEntryTitlePosX   = 25;
    static const int        kDeviceEntryDeviceX     = 12;
    static const int        kDeviceEntryDeviceY     = 3;
    static const int        kDeviceEntryLabelX      = 55;
    static const int        kDeviceEntryLabelY      = 10;
    static const int        kDeviceEntryStatusX     = 258;
    static const int        kDeviceEntryStatusY     = 10;

    static const int        kPopupPositionOffsetX   = 20;
    static const int        kPopupPositionOffsetY   = 10;
    static const int        kPopupDataPositionX     = 9;
    static const int        kPopupDataPositionY     = 13;
    static const int        kPopupDataDeltaY        = 17;
    static const int        kPopupBelowOffsetY      = 37;
    static const int        kPopupButtonPositionX   = 9;
    static const int        kPopupButtonPositionY   = 56;
    static const int        kPopupButtonIconX       = 10;
    static const int        kPopupButtonIconY       = 7;
    static const int        kPopupForgetPositionX   = 53;
    static const int        kPopupForgetPositionY   = 2;

    static const wxString   kFontIntelClearRg       = wxT("IntelClear_Rg");
    static const wxString   kFontIntelClearIt       = wxT("IntelClear_Lt");
    static const wxString   kFontIntelClearLt       = wxT("IntelClear_Lt");
    static const wxString   kFontIntelClearLtIt     = wxT("IntelClear_LtIt");
    static const wxString   kFontIntelClearBd       = wxT("IntelClear_Bd");
    static const wxString   kFontIntelClearBdIt     = wxT("IntelClear_BdIt");
    static const wxString   kFontIntelClearProBd    = wxT("IntelClearPro_Bd");

    // font FACENAMES can be different from their file names
    static const wxString   kFontIntelClearName     = wxT("Intel Clear");
    static const wxString   kFontIntelClearProName  = wxT("Intel Clear Pro");

    static const int        kPasscodeLength         = 6;

    static const int        kAuthTitleX             = 85;
    static const int        kAuthTitleY             = 6;
    static const int        kAuthSubtitleY          = 65;
    static const int        kAuthPasscodeY          = 672;
    static const int        kAuthButtonTextY        = 13;
    static const int        kAuthButtonBuffer       = 26;
    static const int        kAuthCheckX             = 197;
    static const int        kAuthCheckY             = 172;
    static const int        kAuthAppNameX           = 775;

    static const int kTitleBarSize = 0;
    static const wxString kPathSeparator = wxT("/");

    static const int        kFontSizeTitle          = 28;
    static const int        kFontSizeAboutTitle     = 24;
    static const int        kFontSizeText           = 10;
    static const int        kFontSizeOptionsText    = 8;

    static const int        kFontSizeFriendlyName   = 16;
    static const int        kFontSizePopup          = 12;
    static const int        kFontSizePasscode       = 32;
    static const int        kFontSizeTitleRTL       = 26;

    static const int        kStandardPPI            = 96;

    static const wxColour   kIntelBlue              = wxColour(0, 66, 128, 255);
    static const wxColour   kIntelLightBlue         = wxColour(0, 125, 197, 255);
    static const wxColour   kIntelYellow            = wxColour(225, 229, 94, 255);
    static const wxColour   kBorderBlue             = wxColour(1, 62, 107, 255);
    static const wxColour   kPressText              = wxColour(0, 125, 197, 255);
    static const wxColour   kOverText               = wxColour(1, 174, 239, 255);

    class Helpers
    {
    public:
        static const float GetPPIRatio()
        {
            wxScreenDC screen_device_context;
            int current_ppi = screen_device_context.GetPPI().x;
            return static_cast<float>(kStandardPPI) / static_cast<float>(current_ppi);
        }
    };
}
#endif //CONSTANTS_H_
