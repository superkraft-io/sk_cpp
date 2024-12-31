#pragma once

#include <chrono>

#include "../sk_common.hxx"
#include "sk_string.hxx"

#if SK_OS == windows
#elif SK_OS == macos || SK_OS == ios
#endif


BEGIN_SK_NAMESPACE


class SK_DateTime {
public:
    static SK_String formatTime(const std::chrono::system_clock::time_point& tp, const SK_String& format = "%Y-%m-%d %H:%M:%S.000Z") {
        std::time_t time = std::chrono::system_clock::to_time_t(tp); // Convert to time_t
        std::tm tm = *std::gmtime(&time);  // Convert to tm (UTC)

        std::ostringstream oss;
        oss << std::put_time(&tm, format.data.c_str());
        return SK_String(oss.str());
    }
};


END_SK_NAMESPACE