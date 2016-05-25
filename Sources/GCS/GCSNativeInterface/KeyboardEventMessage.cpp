#include "StdAfx.h"
#include "KeyboardEventMessage.h"
#include <linux/input.h>
#include <unistd.h>

KeyboardEventMessage::KeyboardEventMessage() 
    : InputMessage(InputMessage::kKeyboardEventMessage)
    , virtual_key_(0)
    , data_(0)
    , button_state_(BUTTON_STATE_DOWN)
{
}

//virtual 
KeyboardEventMessage::~KeyboardEventMessage()
{
}

//virtual 
void KeyboardEventMessage::ReadMessage()
{
    virtual_key_ = ReadShort();
    data_ = ReadShort();
    button_state_ = static_cast<ButtonState>(ReadByte());
}

void KeyboardEventMessage::SendKeyboardEvent(int input_handler_fd) const
{
    if(input_handler_fd == -1)
    {
        
        return;
    }
    
    char type = 'K';
    unsigned short keycode = KeycodeFromVirtualKey(virtual_key_ & 0x00ff);
    
    if(keycode != KEY_RESERVED)
    {
        write(input_handler_fd, &type, sizeof(type));
        write(input_handler_fd, &keycode, sizeof(keycode));
        write(input_handler_fd, &button_state_, sizeof(button_state_));
    }
}

unsigned short KeyboardEventMessage::GetKey() const
{
    return virtual_key_;
}

unsigned short KeyboardEventMessage::GetData() const
{
    return data_;
}

ButtonState KeyboardEventMessage::GetState() const
{
    return button_state_;
}

unsigned int KeyboardEventMessage::KeycodeFromVirtualKey(unsigned short virtual_key)
{
    if(virtual_key >= 255)
    {
        return 0;
    }
    
    //maps array index (virtual key) to keycode
    static unsigned int keycode_for_virtual_key[255] = {
        KEY_RESERVED,
        BTN_LEFT,       //VK_LBUTTON
        BTN_RIGHT,      //VK_RBUTTON
        KEY_CANCEL,     //VK_CANCEL
        BTN_MIDDLE,     //VK_MBUTTON
        BTN_SIDE,       //VK_XBUTTON1
        BTN_EXTRA,      //VK_XBUTTON2
        KEY_RESERVED,   //UNDEFINED
        KEY_BACKSPACE,  //VK_BACK
        KEY_TAB,        //VK_TAB
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_CLEAR,      //VK_CLEAR
        KEY_ENTER,      //VK_RETURN
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_LEFTSHIFT,  //VK_SHIFT
        KEY_LEFTCTRL,   //VK_CONTROL
        KEY_LEFTALT,    //VK_MENU
        KEY_PAUSE,      //VK_PAUSE
        KEY_CAPSLOCK,   //VK_CAPITAL
        KEY_HANGUEL,    //VK_HANGUEL
        KEY_RESERVED,   //UNDEFINED
KEY_RESERVED,   //VK_JUNJA
KEY_RESERVED,   //VK_FINAL
        KEY_HANJA,   //VK_HANJA
        KEY_RESERVED,   //UNDEFINED
        KEY_ESC,        //VK_ESCAPE
KEY_RESERVED,   //VK_CONVERT
KEY_RESERVED,   //VK_NONCONVERT
KEY_RESERVED,   //VK_ACCEPT
KEY_RESERVED,   //VK_MODECHANGE
        KEY_SPACE,      //VK_SPACE
        KEY_PAGEUP,     //VK_PRIOR
        KEY_PAGEDOWN,   //VK_NEXT
        KEY_END,        //VK_END
        KEY_HOME,       //VK_HOME
        KEY_LEFT,       //VK_LEFT
        KEY_UP,         //VK_UP
        KEY_RIGHT,      //VK_RIGHT
        KEY_DOWN,       //VK_DOWN
        KEY_SELECT,     //VK_SELECT
        KEY_PRINT,      //VK_PRINT
KEY_RESERVED,   //VK_EXECUTE
        KEY_SYSRQ,      //VK_SNAPSHOT
        KEY_INSERT,     //VK_INSERT
        KEY_DELETE,     //VK_DELETE
        KEY_HELP,       //VK_HELP
        
        KEY_0,          //0
        KEY_1,          //1
        KEY_2,          //2
        KEY_3,          //3
        KEY_4,          //4
        KEY_5,          //5
        KEY_6,          //6
        KEY_7,          //7
        KEY_8,          //8
        KEY_9,          //9
        KEY_RESERVED,   //UNDEFINED (0x3A)
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED (0x40)
        KEY_A,          //A
        KEY_B,          //B
        KEY_C,          //C
        KEY_D,          //D
        KEY_E,          //E
        KEY_F,          //F
        KEY_G,          //G
        KEY_H,          //H
        KEY_I,          //I
        KEY_J,          //J
        KEY_K,          //K
        KEY_L,          //L
        KEY_M,          //M
        KEY_N,          //N
        KEY_O,          //O
        KEY_P,          //P
        KEY_Q,          //Q
        KEY_R,          //R
        KEY_S,          //S
        KEY_T,          //T
        KEY_U,          //U
        KEY_V,          //V
        KEY_W,          //W
        KEY_X,          //X
        KEY_Y,          //Y
        KEY_Z,          //Z
        KEY_LEFTMETA,   //VK_LWIN
        KEY_RIGHTMETA,  //VK_RWIN
KEY_RESERVED,   //APPS
        KEY_RESERVED,   //UNDEFINED
        KEY_SLEEP,      //VK_SLEEP
        KEY_KP0,        //VK_NUMPAD0
        KEY_KP1,        //VK_NUMPAD1 
        KEY_KP2,        //VK_NUMPAD2
        KEY_KP3,        //VK_NUMPAD3
        KEY_KP4,        //VK_NUMPAD4
        KEY_KP5,        //VK_NUMPAD5
        KEY_KP6,        //VK_NUMPAD6
        KEY_KP7,        //VK_NUMPAD7
        KEY_KP8,        //VK_NUMPAD8
        KEY_KP9,        //VK_NUMPAD9
        KEY_KPASTERISK, //VK_MULTIPLY
        KEY_KPPLUS,     //VK_ADD
        KEY_KPCOMMA,    //VK_SEPARATOR
        KEY_KPMINUS,    //VK_SUBTRACT
        KEY_KPDOT,      //VK_DECIMAL
        KEY_KPSLASH,    //VK_DIVIDE
        KEY_F1,         //VK_F1
        KEY_F2,         //VK_F2
        KEY_F3,         //VK_F3
        KEY_F4,         //VK_F4
        KEY_F5,         //VK_F5
        KEY_F6,         //VK_F6
        KEY_F7,         //VK_F7
        KEY_F8,         //VK_F8
        KEY_F9,         //VK_F9
        KEY_F10,         //VK_F10
        KEY_F11,         //VK_F11
        KEY_F12,         //VK_F12
        KEY_F13,         //VK_F13
        KEY_F14,         //VK_F14
        KEY_F15,         //VK_F15
        KEY_F16,         //VK_F16
        KEY_F17,         //VK_F17
        KEY_F18,         //VK_F18
        KEY_F19,         //VK_F19
        KEY_F20,         //VK_F20
        KEY_F21,         //VK_F21
        KEY_F22,         //VK_F22
        KEY_F23,         //VK_F23
        KEY_F24,         //VK_F24
        KEY_RESERVED,   //UNDEFINED (0x88)
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED (0x8F)
        KEY_NUMLOCK,    //VK_NUMLOCK
        KEY_SCROLLLOCK, //VK_SCROLL
        KEY_RESERVED,   //UNDEFINED (0x92)
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED (0x96)
        KEY_RESERVED,   //UNDEFINED (0x97)
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED (0x9F)
        KEY_LEFTSHIFT,  //VK_LSHIFT
        KEY_RIGHTSHIFT, //VK_RSHIFT
        KEY_LEFTCTRL,   //VK_LCONTROL
        KEY_RIGHTCTRL,  //VK_RCONTROL
        KEY_LEFTALT,    //VK_LMENU
        KEY_RIGHTALT,   //VK_RMENU
        KEY_BACK,       //VK_BROWSER_BACK
        KEY_FORWARD,    //VK_BROWSER_FORWARD
        KEY_REFRESH,    //VK_BROWSER_REFRESH
        KEY_STOP,       //VK_BROWSER_STOP
        KEY_SEARCH,     //VK_BROWSER_SEARCH
        KEY_FAVORITES,  //VK_BROWSER_FAVORITES
        KEY_HOMEPAGE,   //VK_BROWSER_HOME
        KEY_MUTE,       //VK_VOLUME_MUTE
        KEY_VOLUMEDOWN, //VK_VOLUME_DOWN
        KEY_VOLUMEUP,   //VK_VOLUME_UP
        KEY_NEXTSONG,   //VK_MEDIA_NEXT_TRACK
        KEY_PREVIOUSSONG,//VK_MEDIA_PREVIOUS_TRACK
        KEY_STOPCD,     //VK_MEDIA_STOP
        KEY_PLAYPAUSE,  //VK_MEDIA_PLAY_PAUSE
        KEY_MAIL,       //VK_LAUNCH_MAIL
        KEY_MEDIA,      //VK_LAUNCH_MEDIA_SELECT
        KEY_PROG1,      //VK_LAUNCH_APP1
        KEY_PROG2,      //VK_LAUNCH_APP2
        KEY_RESERVED,   //UNDEFINED
        KEY_RESERVED,   //UNDEFINED
        KEY_SEMICOLON,  //VK_OEM_1
        KEY_EQUAL,      //VK_OEM_PLUS
        KEY_COMMA,      //VK_OEM_COMMA
        KEY_MINUS,      //VK_OEM_MINUS
        KEY_DOT,        //VK_OEM_PERIOD
        KEY_SLASH,      //VK_OEM_2
        KEY_GRAVE,      //VK_OEM_3
        //23 RESERVED KEYS (0xC1-D7)
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        KEY_RESERVED,   //RESERVED
        //3 UNASSIGNED KEYS
        KEY_RESERVED,   //UNASSIGNED (0xD8)
        KEY_RESERVED,   //UNASSIGNED
        KEY_RESERVED,   //UNASSIGNED (0xDA)
        KEY_LEFTBRACE,  //VK_OEM_4
        KEY_BACKSLASH,  //VK_OEM_5
        KEY_RIGHTBRACE, //VK_OEM_6
        KEY_APOSTROPHE, //VK_OEM_7
KEY_RESERVED,   //VK_OEM_8
        KEY_RESERVED,   //RESERVED
        KEY_102ND,      //VK_OEM_102
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
KEY_RESERVED,   //VK_PROCESSKEY
        KEY_RESERVED,   //OEM SPECIFIC
KEY_RESERVED,   //VK_PACKET
        KEY_RESERVED,   //UNASSIGNED
        //13 OEM SPECIFIC KEYS (0xE9-F5)
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
        KEY_RESERVED,   //OEM SPECIFIC
KEY_RESERVED,   //VK_ATTN
KEY_RESERVED,   //VK_CRSEL
KEY_RESERVED,   //VK_EXSEL
KEY_RESERVED,   //VK_EREOF
        KEY_PLAY,   //VK_PLAY
        KEY_ZOOM,   //VK_ZOOM
KEY_RESERVED,   //VK_NONAME
KEY_RESERVED,   //VK_PA1
        KEY_CLEAR   //VK_CLEAR
    };
    
    return keycode_for_virtual_key[virtual_key];
}
