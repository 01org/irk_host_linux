#include "GUID.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace
{
    const unsigned int kGUIDStringLength = 36;
}

GUID GUID_NULL = {0,0,0,{0,0,0,0,0,0,0,0}};

void UuidCreate(GUID *guid)
{
    std::wifstream storeUUID("/proc/sys/kernel/random/uuid", std::wifstream::in);
    std::wstring uuid;
    storeUUID.seekg(0, std::ios::end);
    uuid.reserve(storeUUID.tellg());
    storeUUID.seekg(0, std::ios::beg);
    uuid.assign(std::istreambuf_iterator<wchar_t>(storeUUID), std::istreambuf_iterator<wchar_t>());
    storeUUID.close(); 
    uuid.erase(uuid.end()-1);
    *guid = GUIDFromString(uuid);
}

GUID GUIDFromString(const std::wstring &value)
{
    GUID id;

    if(value.length() != kGUIDStringLength || //doesn't include the NULL terminatine character
        value.substr(8,1) != L"-" || value.substr(13,1) != L"-" || value.substr(18,1) != L"-" || value.substr(23,1) != L"-")
    {
        //Is not of the valid format hhhhhhhh-hhhh-hhhh-hhhh-hhhhhhhhhhhh, where h is a valid hex number
        return GUID_NULL;
    }
    
    wchar_t *next = NULL;
    id.Data1 = wcstoul(value.c_str(), &next, 16);
    id.Data2 = (unsigned short)wcstoul(value.substr(9,4).c_str(), &next, 16);
    id.Data3 = (unsigned short)wcstoul(value.substr(14,4).c_str(), &next, 16);
    id.Data4[0] = (unsigned char)wcstoul(value.substr(19,2).c_str(), &next, 16);
    id.Data4[1] = (unsigned char)wcstoul(value.substr(21,2).c_str(), &next, 16);
    
    id.Data4[2] = (unsigned char)wcstoul(value.substr(24,2).c_str(), &next, 16);
    id.Data4[3] = (unsigned char)wcstoul(value.substr(26,2).c_str(), &next, 16);
    id.Data4[4] = (unsigned char)wcstoul(value.substr(28,2).c_str(), &next, 16);
    id.Data4[5] = (unsigned char)wcstoul(value.substr(30,2).c_str(), &next, 16);
    id.Data4[6] = (unsigned char)wcstoul(value.substr(32,2).c_str(), &next, 16);
    id.Data4[7] = (unsigned char)wcstoul(value.substr(34,2).c_str(), &next, 16);

    return id;
}

std::wstring StringFromGUID(const GUID &ID)
{
    std::wstringstream guid_stream;
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data1;
    guid_stream << "-";
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data2;
    guid_stream << "-";
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data3;
    guid_stream << "-";
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[0];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[1];
    guid_stream << "-";
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[2];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[3];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[4];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[5];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[6];
    guid_stream << std::uppercase << std::setfill(L'0') << std::setw(2) << std::hex << ID.Data4[7];
    
    std::wstring guid_string;
    guid_stream >> guid_string;
    return guid_string;
}

