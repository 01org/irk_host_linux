#ifndef UTILITIES_H_
#define UTILITIES_H_

namespace Utilities
{
    std::vector<std::wstring> &SplitString(const std::wstring &s, wchar_t delim, std::vector<std::wstring> &elems);
    bool FileExists(const wchar_t *filename);
    void EnumerateWakeableMacAddresses(std::vector<std::wstring> *mac_addresses);
}

#endif // UTILITIES_H_
