
#pragma once

#include <string>
      
typedef struct _GUID 
{
   unsigned long Data1;
   unsigned short Data2;
   unsigned short Data3;
   unsigned char Data4[8];
} GUID;

extern GUID GUID_NULL;

inline bool operator==(const GUID &lhs, const GUID &rhs) 
{
    return  lhs.Data1 == rhs.Data1 &&
            lhs.Data2 == rhs.Data2 &&
            lhs.Data3 == rhs.Data3 &&
            lhs.Data4[0] == rhs.Data4[0] &&
            lhs.Data4[1] == rhs.Data4[1] &&
            lhs.Data4[2] == rhs.Data4[2] &&
            lhs.Data4[3] == rhs.Data4[3] &&
            lhs.Data4[4] == rhs.Data4[4] &&
            lhs.Data4[5] == rhs.Data4[5] &&
            lhs.Data4[6] == rhs.Data4[6] &&
            lhs.Data4[7] == rhs.Data4[7];
}

void UuidCreate(GUID *guid);
GUID GUIDFromString(const std::wstring &value);
std::wstring StringFromGUID(const GUID &ID);
    
#define IsEqualGUID(lhs, rhs) (lhs == rhs)
