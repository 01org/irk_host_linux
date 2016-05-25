#ifndef LINUX_FALLBACK_H_
#define LINUX_FALLBACK_H_

#include <fcntl.h>
#include <linux/uinput.h>

#include "ButtonState.h"

class LinuxFallback
{    
public:
    virtual ~LinuxFallback();
    
    static void Setup();
    static void Cleanup();
    
    static LinuxFallback *GetInstance();
    void SendMouseMove(bool relative, float x, float y);
    void SendMouseScroll(float x, float y);
    void SendMouseButtonEvent(unsigned short button, ButtonState state);
    void SendKeyEvent(unsigned int keycode, ButtonState state);
    void SendUnicharEvent(wchar_t character);

private:
    //DISALLOW_COPY_AND_ASSIGN(LinuxFallback);
    
    LinuxFallback();
    
    int KeyCodeFromHexDigit(char hex_digit);

    static LinuxFallback *instance_;
    
    bool is_setup_;
    int device_fd_;
    struct uinput_user_dev device_structure_;
    struct input_event event_;
};

#endif // LINUX_FALLBACK_H_
