#pragma once

//Define namespace
#define BEGIN_SK_NAMESPACE namespace SK {
#define END_SK_NAMESPACE }


//Define which OS is being used
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



//Define which framework is being used
#define SK_FRAMEWORK Superkraft //Default

#if IPLUG_VERSION
	#define SK_FRAMEWORK iPlug2
#endif

#if JUCE_MODULE_AVAILABLE_juce_core
	#define SK_FRAMEWORK JUCE
#endif

#if defined DEBUG || defined _DEBUG
	#define SK_MODE_DEBUG
#else
	#define SK_MODE_RUNTIME
#endif