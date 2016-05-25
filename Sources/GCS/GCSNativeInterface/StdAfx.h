// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include <stdlib.h>
#include "macros.h"

// Include STL headers before strsafe.h (VS2010 problem with include orders, fixed in future releases)
#include <map>
#include <vector>
#include <string>

void GCSOutputDebugString(const wchar_t *sFormat, ...); 
void GCSStartLog(const wchar_t* path = L"~/debuglog.txt");

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef int64_t LONG64;

typedef int BOOL;
#define TRUE 1;
#define FALSE 0;

typedef int64_t LONGLONG;

typedef union _LARGE_INTEGER
{
    struct 
    {
        DWORD LowPart;
        long HighPart;
    };
    struct
    {
        DWORD LowPart;
        long HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

#define UNREFERENCED_PARAMETER(P) (P)

#define __stdcall

#define MAX_PATH 260

#define INVALID_SOCKET (~0)
