#pragma once

#define BEGIN_SK_NAMESPACE namespace SK {
#define END_SK_NAMESPACE }



#if defined(_WIN32) || defined(_WIN64)
	#define SK_OS windows
#elif defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define SK_OS ios
	#else
		#define SK_OS macos
	#endif
#elif defined(__linux__)
	#define SK_OS linux
#elif defined(__ANDROID__)
	#define SK_OS android
#else
	#define SK_OS unknown
#endif

