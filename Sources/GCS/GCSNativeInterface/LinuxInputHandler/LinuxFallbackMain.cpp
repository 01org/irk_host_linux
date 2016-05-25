#include "LinuxFallback.h"

#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdarg.h>

int fd = -1;

void GCSLogOutputString(const std::wstring &string)
{
#ifdef _DEBUG
    std::wofstream log_file("~/InputManagerdebuglog.txt", std::ios_base::out | std::ios_base::app);
    if(!log_file.bad())
    {
        log_file << string;
        log_file.close();
    }
#endif
}

// Function use for debugging purpose
void GCSOutputDebugString(const wchar_t *sFormat, ... )
{
#ifdef _DEBUG
    va_list argptr;      
    va_start( argptr, sFormat ); 
    wchar_t buffer[ 2000 ];
    vswprintf(buffer, 2000, sFormat, argptr);
    GCSLogOutputString(buffer);
#endif
}

void GCSStartLog()
{
#ifdef _DEBUG
    std::wofstream log_file("~/InputManagerdebuglog.txt", std::ios_base::out | std::ios_base::trunc);
   log_file.close();
#endif
}

bool ReadMouseMoveMessage(bool &relative, float &x, float &y)
{
    if(read(fd, &relative, sizeof(relative)) <= 0)
    {
        return false;
    }
    if(read(fd, &x, sizeof(x)) <= 0)
    {
        return false;
    }
    if(read(fd, &y, sizeof(y)) <= 0)
    {
        return false;
    }
    return true;
}

bool ReadMouseScrollMessage(float &x, float &y)
{
    if(read(fd, &x, sizeof(x)) <= 0)
    {
        return false;
    }
    if(read(fd, &y, sizeof(y)) <= 0)
    {
        return false;
    }
    return true;
}

bool ReadMouseButtonMessage(unsigned short &button, ButtonState &state)
{
    if(read(fd, &button, sizeof(button)) <= 0)
    {
        return false;
    }
    if(read(fd, &state, sizeof(state)) <= 0)
    {
        return false;
    }
    return true;
}

bool ReadKeyMessage(unsigned short &keycode, ButtonState &state)
{
    if(read(fd, &keycode, sizeof(keycode)) <= 0)
    {
        return false;
    }
    if(read(fd, &state, sizeof(state)) <= 0)
    {
        return false;
    }
    return true;
}

bool ReadUnicharMessage(wchar_t &character)
{
    if(read(fd, &character, sizeof(wchar_t)) <= 0)
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    GCSStartLog();
    
    if(argc != 2)
    {
        GCSOutputDebugString(L"Invalid number of arguments: %d\n", argc - 1);
        return 2;
    }
    else
    {
        char *end;
        fd = strtol(argv[1], &end, 10);
    }
    
    if(fd == -1)
    {
        GCSOutputDebugString(L"Invalid file descriptor: %d\n", fd);
        return 3;
    }
    
    //Simply listens for messages on stdin
    //There are 7 possible messages: (function, (format received))
    //SendMouseMove, [M,relative(bool),x(float),y(float)]
    //SendMouseScroll, [S,x(float),y(float)]
    //SendMouseButtonEvent, [B,button(unsigned short),state(ButtonState)]
    //SendKeyEvent, (K,keycode(unsigned short), state(ButtonState)]
    //SendUnicharEvent, (U,character(wchar_t))
    //Heartbeat, [H]
    //Quit, [Q]
    
    bool done = false;
    fd_set read_sockets;
    
    LinuxFallback::Setup();
    
    while(!done)
    {
        FD_ZERO(&read_sockets);
        FD_SET(fd, &read_sockets);
        
        struct timeval tv;
        tv.tv_sec = 15;
        tv.tv_usec = 0;

        int result = select(fd+1, &read_sockets, NULL, NULL, &tv);
        
        if(result == 0)
        {
            GCSOutputDebugString(L"Read timeout\n");
            break; //Server hasn't sent anything to say it is alive, so assume it has been closed
        }

        char type;
        read(fd, &type, sizeof(type));
        
        switch(type)
        {
            case 'M':
            {
                bool relative;
                float x;
                float y;
                ReadMouseMoveMessage(relative, x, y);
                LinuxFallback::GetInstance()->SendMouseMove(relative, x, y);
                break;
            }
            case 'S':
            {
                float x;
                float y;
                ReadMouseScrollMessage(x, y);
                LinuxFallback::GetInstance()->SendMouseScroll(x, y);
                break;
            }
            case 'B':
            {
                unsigned short button;
                ButtonState state;
                ReadMouseButtonMessage(button, state);
                LinuxFallback::GetInstance()->SendMouseButtonEvent(button, state);
                break;
            }
            case 'K':
            {
                unsigned short keycode;
                ButtonState state;
                ReadKeyMessage(keycode, state);
                LinuxFallback::GetInstance()->SendKeyEvent(keycode, state);
                break;
            }
            case 'U':
            {
                wchar_t character;
                ReadUnicharMessage(character);
                LinuxFallback::GetInstance()->SendUnicharEvent(character);
                break;
            }
            case 'Q':
            {
                done = true;
                break;
            }
            case 'H':
            {
                break;
            }
            default:
                break;
        };
    }
    
    LinuxFallback::Cleanup();
}
