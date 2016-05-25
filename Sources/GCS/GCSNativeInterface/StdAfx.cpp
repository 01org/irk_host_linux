// stdafx.cpp : source file that includes just the standard includes
// GCSNativeInterface.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "StdAfx.h"
#include <fstream>
#include <stdarg.h>
#include <sstream>
#include <iomanip>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void GCSLogOutputString(const std::wstring &string, const wchar_t* path)
{
#if defined( _DEBUG )
    std::wofstream log_file("~/debuglog.txt", std::ios_base::out | std::ios_base::app);
    if(!log_file.bad())
    {
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
    GCSLogOutputString(buffer, L"~/debuglog.txt");
#else
    UNREFERENCED_PARAMETER(sFormat);
#endif
}

void GCSStartLog(const wchar_t* path)
{
#ifdef _DEBUG
    std::wofstream log_file("~/debuglog.txt", std::ios_base::out | std::ios_base::trunc);
    log_file.close();
#else
    UNREFERENCED_PARAMETER(path);
#endif
}

void GCSLogBytes(const BYTE *bytes, unsigned int size)
{
    wchar_t *buffer = new wchar_t[1024];
    wchar_t *current = buffer;
    for (unsigned int i = 0; i < size && i < 3072 - 5; ++i)
    {
        current += swprintf(current, 3072, L"%02X ", bytes[i]);
    }
    current = L'\0';

    GCSOutputDebugString(L"%s\n", buffer);

    current = buffer;

    for (unsigned int i = 0; i < size && i < 3072 - 5; ++i)
    {
        current += swprintf(current, 3072, L"%c", bytes[i] == 0 ? '~' : bytes[i]);
    }
    current = L'\0';

    GCSOutputDebugString(L"%s\n", buffer);

    delete[] buffer;
}

std::wstring CurrentTimeString()
{
    return std::wstring(L"");
}
