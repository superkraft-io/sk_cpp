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

#elif defined(__APPLE__)

static NSString* StringToNSString(const std::string& str){
    return [NSString stringWithUTF8String:str.c_str()];
}

#endif
