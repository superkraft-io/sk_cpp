#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>


#include <string>
#include <iostream>

static LPCWSTR StringToLPCWSTR(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wchar_t* wide_str = new wchar_t[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide_str, size_needed);
    return wide_str;
}



static std::string wstringToString(const std::wstring& wstr)
{
    if (wstr.empty())
        return {};
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}

static std::wstring stringToWString(const std::string& str)
{
    // If the input string is empty, return an empty wstring
    if (str.empty())
    {
        return {};
    }

    // Convert std::string (UTF-8) to std::wstring (UTF-16 or UTF-32 depending on platform)
    return std::wstring(str.begin(), str.end());
}


static std::string jsonToDelimitedString(const nlohmann::json& jsonObject, const std::string& pairDelimiter = "; ", const std::string& keyValueSeparator = ": ")
{
    if (jsonObject.empty())
    {
        return "";
    }

    std::string result;
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
    {
        if (!result.empty())
        {
            result += pairDelimiter;
        }
        result += it.key() + keyValueSeparator + it.value().get<std::string>();
    }

    return result;
}

static wil::unique_cotaskmem_string stringToUniqueCoTaskMemString(const std::string& str)
{
    // Convert std::string to std::wstring
    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (wideCharCount == 0)
    {
        throw std::runtime_error("Failed to convert string to wide string");
    }

    // Allocate wide-character memory using CoTaskMemAlloc
    wchar_t* wideString = static_cast<wchar_t*>(CoTaskMemAlloc(wideCharCount * sizeof(wchar_t)));
    if (!wideString)
    {
        throw std::bad_alloc();
    }

    // Perform the conversion
    if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideString, wideCharCount) == 0)
    {
        CoTaskMemFree(wideString);
        throw std::runtime_error("Failed to convert string to wide string");
    }

    // Transfer ownership to wil::unique_cotaskmem_string
    return wil::unique_cotaskmem_string(wideString);
}
#elif defined(__APPLE__)

static NSString* StringToNSString(const std::string& str){
    return [NSString stringWithUTF8String:str.c_str()];
}

#endif
