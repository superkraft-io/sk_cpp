#pragma once


//Stringify macro
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


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
#define SK_FRAMEWORK_SUPERKRAFT 1
#define SK_FRAMEWORK_iPlug2 2
#define SK_FRAMEWORK_JUCE 3

#define SK_FRAMEWORK SK_FRAMEWORK_SUPERKRAFT //Default

#if defined(IPLUG_VERSION)
	#define SK_FRAMEWORK SK_FRAMEWORK_iPlug2
#endif

#if defined(JUCE_MODULE_AVAILABLE_juce_core)
	#define SK_FRAMEWORK SK_FRAMEWORK_JUCE
#endif


//Define which mode
#if defined DEBUG || defined _DEBUG
	#define SK_MODE_DEBUG
	#define SK_MODE "runtime"
#else
	#define SK_MODE_RELEASE
	#define SK_MODE "runtime"
#endif


//Define CPU architecture
#if defined(__x86_64__) || defined(_M_X64)
	#define SK_CPU_ARCH x64
#elif defined(__i386__) || defined(_M_IX86)
	#define SK_CPU_ARCH x86
#elif defined(__arm__) || defined(_M_ARM)
	#define SK_CPU_ARCH arm
#elif defined(__aarch64__) || defined(_M_ARM64)
	#define SK_CPU_ARCH arm64
#elif defined(__ia64__) || defined(_M_IA64)
	#define SK_CPU_ARCH ia64
#elif defined(__mips__) || defined(__mips)
	#if defined(__LP64__) || defined(_LP64)
		#define SK_CPU_ARCH mips64
	#else
		#define SK_CPU_ARCH mips
	#endif
#elif defined(__mipsel__) || defined(__mips_le)
	#if defined(__LP64__) || defined(_LP64)
		#define SK_CPU_ARCH mips64el
	#else
		#define SK_CPU_ARCH mipsel
	#endif
#elif defined(__powerpc__) || defined(__powerpc64__)
	#if defined(__PPC64__)
		#define SK_CPU_ARCH ppc64
	#else
		#define SK_CPU_ARCH ppc
	#endif
#elif defined(__s390__) || defined(__s390x__)
	#if defined(__s390x__)
		#define SK_CPU_ARCH s390x
	#else
		#define SK_CPU_ARCH s390
	#endif
#elif defined(__riscv)
	#if __riscv_xlen == 64
		#define SK_CPU_ARCH riscv64
	#else
		#define SK_CPU_ARCH riscv32
	#endif
#else
	#define SK_CPU_ARCH unknown
#endif

