#include <macros.h>
#include "LinuxFallback.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

extern void GCSOutputDebugString(const wchar_t *sFormat, ... );

LinuxFallback *LinuxFallback::instance_ = NULL;

LinuxFallback::LinuxFallback()
    : is_setup_(false)
    , device_fd_(-1)
    , device_structure_()
    , event_()
{
    device_fd_ = open("/dev/uinput", O_WRONLY | O_NDELAY);
    
    if(device_fd_ == -1)
    {
        return;
    }

    memset(&device_structure_, 0, sizeof(device_structure_));
    strncpy(device_structure_.name, "RemoteKeyboard Fallback", UINPUT_MAX_NAME_SIZE);
    
    device_structure_.id.version = 1;
    device_structure_.id.bustype = BUS_USB;
    
    // Setup the uinput device
    ioctl(device_fd_, UI_SET_EVBIT, EV_KEY);
    ioctl(device_fd_, UI_SET_EVBIT, EV_REL);
    ioctl(device_fd_, UI_SET_RELBIT, REL_X);
    ioctl(device_fd_, UI_SET_RELBIT, REL_Y);
    ioctl(device_fd_, UI_SET_RELBIT, REL_DIAL);
    
    for (int i = 0; i < 256; ++i) 
    {
        ioctl(device_fd_, UI_SET_KEYBIT, i);
    }
    
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_FORWARD);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_BACK);
   
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_4);
    ioctl(device_fd_, UI_SET_KEYBIT, BTN_5);
    
    /* Create input device into input sub-system */
    write(device_fd_, &device_structure_, sizeof(device_structure_));
    if (ioctl(device_fd_, UI_DEV_CREATE))
    {
        //Error creating device
        close(device_fd_);
        return;
    }
    is_setup_ = true;
}

// virtual 
LinuxFallback::~LinuxFallback()
{
    if(is_setup_)
    {
        ioctl(device_fd_, UI_DEV_DESTROY);
        close(device_fd_);
    }
}

void LinuxFallback::Setup()
{
    if(instance_ == NULL)
    {
        instance_ = new LinuxFallback();        
    }
}

void LinuxFallback::Cleanup()
{
    delete instance_;
    instance_ = NULL;
}

LinuxFallback *LinuxFallback::GetInstance()
{
    return instance_;
}

void LinuxFallback::SendMouseMove(bool relative, float x, float y)
{
    if(!is_setup_)
    {
        return;
    }
    
    memset(&event_, 0, sizeof(event_));
    gettimeofday(&event_.time, NULL);
    event_.type = relative ? EV_REL : EV_ABS;
    event_.code = relative ? REL_X : ABS_X;
    event_.value = (int)x;
    write(device_fd_, &event_, sizeof(event_));
    
    event_.type = relative ? EV_REL : EV_ABS;
    event_.code = relative ? REL_Y : ABS_Y;
    event_.value = (int)y;
    write(device_fd_, &event_, sizeof(event_));
    
    event_.type = EV_SYN;
    event_.code = SYN_REPORT;
    event_.value = 0;
    write(device_fd_, &event_, sizeof(event_));
}

void LinuxFallback::SendMouseScroll(float x, float y)
{
    if(!is_setup_)
    {
        return;
    }
    
    float delta = (x == 0.0f ? y : x);
    
    memset(&event_, 0, sizeof(event_));
    gettimeofday(&event_.time, NULL);
    event_.type = EV_REL;
    event_.code = REL_DIAL;
    event_.value = (int)delta;
    write(device_fd_, &event_, sizeof(event_));
    
    event_.type = EV_SYN;
    event_.code = SYN_REPORT;
    event_.value = 0;
    write(device_fd_, &event_, sizeof(event_));
}

void LinuxFallback::SendMouseButtonEvent(unsigned short button, ButtonState state)
{
    if(!is_setup_ || button == KEY_RESERVED)
    {
        return;
    }
    
    if(state != BUTTON_STATE_UP)
    {
    // Report BUTTON CLICK - PRESS event
        memset(&event_, 0, sizeof(event_));
        gettimeofday(&event_.time, NULL);
        event_.type = EV_KEY;
        event_.code = button;
        event_.value = 1;
        write(device_fd_, &event_, sizeof(event_));

        event_.type = EV_SYN;
        event_.code = SYN_REPORT;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));
    }
    
    if(state != BUTTON_STATE_DOWN)
    {
        // Report BUTTON CLICK - RELEASE event
        memset(&event_, 0, sizeof(event_));
        gettimeofday(&event_.time, NULL);
        event_.type = EV_KEY;
        event_.code = button;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));

        event_.type = EV_SYN;
        event_.code = SYN_REPORT;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));
    }
}

void LinuxFallback::SendKeyEvent(unsigned int keycode, ButtonState state)
{
    if(!is_setup_ || keycode == KEY_RESERVED)
    {
        return;
    }
    
    if(state != BUTTON_STATE_UP)
    {
        // Report BUTTON CLICK - PRESS event
        memset(&event_, 0, sizeof(event_));
        gettimeofday(&event_.time, NULL);
        event_.type = EV_KEY;
        event_.code = keycode;
        event_.value = 1;
        write(device_fd_, &event_, sizeof(event_));

        event_.type = EV_SYN;
        event_.code = SYN_REPORT;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));
    }
    
    if(state != BUTTON_STATE_DOWN)
    {
        // Report BUTTON CLICK - RELEASE event
        memset(&event_, 0, sizeof(event_));
        gettimeofday(&event_.time, NULL);
        event_.type = EV_KEY;
        event_.code = keycode;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));

        event_.type = EV_SYN;
        event_.code = SYN_REPORT;
        event_.value = 0;
        write(device_fd_, &event_, sizeof(event_));
    }
}

void LinuxFallback::SendUnicharEvent(wchar_t character)
{    
    // Send the hex value of the unicode character
    static const int kMaxHexStringLength = 32;
    char hex_string[kMaxHexStringLength] = {0};
    snprintf(hex_string, kMaxHexStringLength, "%04X", (int)character);

    if (strlen(hex_string) > 0)
    {
        // Ctrl+Shit+U
        SendKeyEvent(KEY_LEFTCTRL, BUTTON_STATE_DOWN);
        SendKeyEvent(KEY_LEFTSHIFT, BUTTON_STATE_DOWN);
        SendKeyEvent(KEY_U, BUTTON_STATE_CLICK);
        SendKeyEvent(KEY_LEFTSHIFT, BUTTON_STATE_UP);
        SendKeyEvent(KEY_LEFTCTRL, BUTTON_STATE_UP);

        // Send the hex digits for the character
        for (unsigned int i = 0; i < kMaxHexStringLength && hex_string[i] != '\0'; ++i)
        {
            SendKeyEvent(KeyCodeFromHexDigit(hex_string[i]), BUTTON_STATE_CLICK);
        }

        // Send an enter (or space) to complete the sequence
        SendKeyEvent(KEY_ENTER, BUTTON_STATE_CLICK);
    }
}

int LinuxFallback::KeyCodeFromHexDigit(char hex_digit)
{
    //Convert '0'-'9' into integer 0-9, and 'A'-'F' into integer 10-15
    int index = -1;
    if(hex_digit >= '0' && hex_digit <= '9')
    {
       index = hex_digit - '0'; 
    }
    else if(hex_digit >= 'A' && hex_digit <= 'F')
    {
        index = hex_digit - 'A' + 10;
    }
    
    if(index < 0 || index > 15)
    {
        return KEY_RESERVED;
    }
    
    static unsigned int keycode_for_hex_digit[16] = {
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F
    };
    
    return keycode_for_hex_digit[index];
}

