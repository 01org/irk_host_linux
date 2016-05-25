#include "StdAfx.h"
#include "Utilities.h"
#include <sys/stat.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fstream>

//typedef int (WINAPI *AppVisibilityInit)();
//typedef void (WINAPI *AppVisibilityDeinit)();
//typedef int (WINAPI *AppVisibilityGetStartMenuVisibility)();
//typedef int (WINAPI *AppVisibilityGetAppVisibility)();

std::vector<std::wstring> &Utilities::SplitString(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems) {
    std::wstringstream ss(s);
    std::wstring item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

bool Utilities::FileExists(const wchar_t *filename)
{
    struct stat buffer;
    size_t length = wcsnlen(filename, 260);
    char *file = new char[length];
    wcstombs(file, filename, length);
    int ret = stat(file, &buffer);
    return (ret == 0);
}

void Utilities::EnumerateWakeableMacAddresses(std::vector<std::wstring> *mac_addresses)
{
    std::string base_directory = "/sys/class/net/";
    DIR *dp = opendir(base_directory.c_str());
    if(dp != NULL)
    {
        struct dirent *directory_entry = readdir(dp);
            
        while(directory_entry != NULL)
        {
            if(directory_entry->d_name[0] == '.')
            {
                directory_entry = readdir(dp);
                continue;
            }
                
            std::string filename = base_directory + directory_entry->d_name + "/address";
            std::wifstream file(filename.c_str());
                
            if(file.is_open())
            {
                std::wstring address;
                file.seekg(0, std::ios::end);
                address.reserve(file.tellg());
                file.seekg(0, std::ios::beg);
                address.assign(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>());
                file.close(); 
                address.erase(address.end()-1);
                if(address.length() > 0 && address != L"00:00:00:00:00:00")
                {
                    GCSOutputDebugString(L"found valid mac address: %ls\n", address.c_str());
                    mac_addresses->push_back(std::wstring(address.begin(), address.end()-1));
                }
            }
                
            directory_entry = readdir(dp);
        }
    }
}
