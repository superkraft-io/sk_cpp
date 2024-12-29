#pragma once

#include "json.hpp"

#include "../sk_common.hxx"


#if SK_OS == windows
    #include <windows.h>
    #include <VersionHelpers.h>
    #include <comdef.h>
    #include <Wbemidl.h>

    #pragma comment(lib, "wbemuuid.lib")

    typedef NTSTATUS(WINAPI* RtlGetVersionFunc)(RTL_OSVERSIONINFOEXW*);
#else
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <sys/sysctl.h>
    #include <mach/mach.h>

    struct MemoryInfo {
        uint64_t totalMemory; // in bytes
        uint64_t freeMemory;  // in bytes
        uint64_t usedMemory;  // in bytes
    };
#endif

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>



struct CPUInfo {
    std::string model;
    int speed; // in MHz
    long long user;
    long long nice;
    long long sys;
    long long idle;
    long long irq;
};


class SK_Machine {
public:

    static void init() {
        SK_Machine::cpuModel = SK_Machine::getCPUModel();
        SK_Machine::cpuSpeed = SK_Machine::getCPUSpeed();
        SK_Machine::staticInfo = SK_Machine::getStaticInfo();
    };

    static inline nlohmann::json staticInfo;

    static nlohmann::json respondError(std::string errorMsg){
        nlohmann::json json {
            {"error", errorMsg}
        };

        return json;
    }


    static bool isBigEndian() {
        uint32_t num = 1;
        uint8_t* bytePtr = reinterpret_cast<uint8_t*>(&num);
        if (bytePtr[0] == 1) return false;
        return true;
    }

    static std::string getCPUArch() {
        return TOSTRING(SK_CPU_ARCH);
    }

    static std::string getMachineType(){
        #if SK_OS == windows
            SYSTEM_INFO sysinfo;
            GetNativeSystemInfo(&sysinfo);

            switch (sysinfo.wProcessorArchitecture) {
                case PROCESSOR_ARCHITECTURE_AMD64:
                    return "x86_64";  // 64-bit AMD or Intel
                case PROCESSOR_ARCHITECTURE_INTEL:
                    return "i386";    // 32-bit Intel x86
                case PROCESSOR_ARCHITECTURE_ARM:
                    return "arm";     // ARM
                case PROCESSOR_ARCHITECTURE_ARM64:
                    return "arm64";   // ARM64
                case PROCESSOR_ARCHITECTURE_IA64:
                    return "ia64";    // Itanium
                case PROCESSOR_ARCHITECTURE_ALPHA:
                    return "alpha";   // Alpha
                case PROCESSOR_ARCHITECTURE_SHX:
                    return "shx";     // Hitachi SuperH
                case PROCESSOR_ARCHITECTURE_PPC:
                    return "ppc";     // PowerPC
                case PROCESSOR_ARCHITECTURE_MIPS:
                    return "mips";    // MIPS
                case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
                    return "wow64";   // 32-bit application running on 64-bit Windows
                case PROCESSOR_ARCHITECTURE_ALPHA64:
                    return "alpha64";    // Alpha 64-bit
                case PROCESSOR_ARCHITECTURE_MSIL:
                    return "msil";       // Microsoft Intermediate Language (MSIL)
                case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:
                    return "arm32_wow";  // ARM32 application running on 64-bit Windows
                case PROCESSOR_ARCHITECTURE_NEUTRAL:
                    return "neutral";    // ARM64 neutral (not supported in Windows)
                case PROCESSOR_ARCHITECTURE_IA32_ON_ARM64:
                    return "wow_arm64";  // WOW64 on ARM64 (not supported in Windows)
                default:
                    return "Unknown machine type";
            }
        #else
            struct utsname buffer;
            if (uname(&buffer) != 0) {
                return "Unknown machine type";
            }

            return buffer.machine;
        #endif
    }


    static std::string getOSType() {
        // Determine the platform using preprocessor macros

        #if SK_OS == windows
        // Windows platform
            OSVERSIONINFOEX osvi;
            ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
            osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

            if (GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi))) {
                switch (osvi.dwPlatformId) {
                case VER_PLATFORM_WIN32_NT:
                    return "Windows_NT";
                case VER_PLATFORM_WIN32_WINDOWS:
                    return "Windows";
                case VER_PLATFORM_WIN32s:
                    return "Windows 3.x";
                default:
                    return "Unknown";
                }
            }
            else {
                return "Unknown";
            }

        #elif
        // macOS platform
            struct utsname buffer;
            if (uname(&buffer) == 0) {
                return buffer.sysname;
            }
            else {
                return "Unknown";
            }

        #else
        // Unsupported platform
            return "Unknown";
        #endif
    }

    static std::string getOSPlatform() {
        // Determine the platform using preprocessor macros
        #if defined(_WIN32)
            return "win32";
        #elif defined(__APPLE__)
            return "darwin";
        #elif defined(__linux__)
            return "linux";
        #elif defined(__FreeBSD__)
            return "freebsd";
        #elif defined(__OpenBSD__)
            return "openbsd";
        #elif defined(__NetBSD__)
            return "netbsd";
        #elif defined(__sun) && defined(__SVR4)
            return "sunos";
        #elif defined(_AIX)
            return "aix";
        #elif defined(__ANDROID__)
            return "android";
        #elif defined(__DragonFly__)
            return "dragonfly";
        #else
            return "unknown";
        #endif
    }


    static std::string getOSVersion(bool releaseVersion = true) {
        #if SK_OS == windows
            RTL_OSVERSIONINFOEXW osvi;
            ZeroMemory(&osvi, sizeof(RTL_OSVERSIONINFOEXW));
            osvi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);

            // Load ntdll.dll dynamically
            HMODULE hMod = GetModuleHandle("ntdll.dll");
            if (hMod != nullptr) {
                // Get address of RtlGetVersion function
                RtlGetVersionFunc pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(GetProcAddress(hMod, "RtlGetVersion"));
                if (pRtlGetVersion != nullptr) {
                    // Call RtlGetVersion to fill osvi structure
                    if (pRtlGetVersion(&osvi) == 0) { // Check for success directly
                        // Construct version string
                        std::string version = std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion) + "." + std::to_string(osvi.dwBuildNumber);
                        
                        if (releaseVersion == false){
                            version = "Windows " + std::to_string(osvi.dwMajorVersion);
                            if (osvi.wSuiteMask & VER_SUITE_PERSONAL) version += " Home";
                            else  version += " Pro";
                        }

                        return version;
                    }
                }
            }
            return "unknown";
        #else
            FILE* pipe = popen("/usr/bin/sw_vers -productVersion", "r");
            if (!pipe) return "unknown";

            char buffer[128];
            std::string result = "";
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != NULL)
                    result += buffer;
        }
            pclose(pipe);

            // Trim newline character if present
            if (!result.empty() && result[result.length() - 1] == '\n')
                result.erase(result.length() - 1);

            return result;
        #endif
    }


    static std::vector<CPUInfo>  getCPUInformation() {
        std::vector<CPUInfo> cpus;
        
        #if SK_OS == windows
            //THIS FUNCTION DOES NOT WORK IN WINDOWS BECAUSE APPARENTLY IT MNUST BE CALLED IN THE PROGRAIM main() FUNCTION
            //IN OTHER WORDS IT MUST BE CALLED AS SOON AS THE PROGRAM STARTS AND THUS THIS FUNCTION CANNOT BE CALLED
            //AT A LATER STAGE OF RUNTIME


            HRESULT hres;
            hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
            if (FAILED(hres)) {
                std::cerr << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
            }

            hres = CoInitializeSecurity(
                NULL,
                -1,                          // COM authentication
                NULL,                        // Authentication services
                NULL,                        // Reserved
                RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
                RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
                NULL,                        // Authentication info
                EOAC_NONE,                   // Additional capabilities 
                NULL                         // Reserved
            );

            if (FAILED(hres)) {
                std::cerr << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
                CoUninitialize();
                return cpus;
            }

            IWbemLocator* pLoc = NULL;
            hres = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID*)&pLoc);

            if (FAILED(hres)) {
                std::cerr << "Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres << std::endl;
                CoUninitialize();
                return cpus;
            }

            IWbemServices* pSvc = NULL;
            hres = pLoc->ConnectServer(
                _bstr_t(L"ROOT\\CIMV2"),
                NULL,
                NULL,
                0,
                NULL,
                0,
                0,
                &pSvc);

            if (FAILED(hres)) {
                std::cerr << "Could not connect. Error code = 0x" << std::hex << hres << std::endl;
                pLoc->Release();
                CoUninitialize();
                return cpus;
            }

            hres = CoSetProxyBlanket(
                pSvc,
                RPC_C_AUTHN_WINNT,
                RPC_C_AUTHZ_NONE,
                NULL,
                RPC_C_AUTHN_LEVEL_CALL,
                RPC_C_IMP_LEVEL_IMPERSONATE,
                NULL,
                EOAC_NONE
            );

            if (FAILED(hres)) {
                std::cerr << "Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
                pSvc->Release();
                pLoc->Release();
                CoUninitialize();
                return cpus;
            }

            IEnumWbemClassObject* pEnumerator = NULL;
            hres = pSvc->ExecQuery(
                bstr_t("WQL"),
                bstr_t("SELECT * FROM Win32_Processor"),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                NULL,
                &pEnumerator);

            if (FAILED(hres)) {
                std::cerr << "Query for CPU info failed. Error code = 0x" << std::hex << hres << std::endl;
                pSvc->Release();
                pLoc->Release();
                CoUninitialize();
                return cpus;
            }

            IWbemClassObject* pclsObj = NULL;
            ULONG uReturn = 0;

            while (pEnumerator) {
                HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                if (0 == uReturn) {
                    break;
                }

                VARIANT vtProp;

                CPUInfo cpu;
                hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
                if (SUCCEEDED(hr)) {
                    cpu.model = _bstr_t(vtProp.bstrVal);
                    VariantClear(&vtProp);
                }

                hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
                if (SUCCEEDED(hr)) {
                    cpu.speed = vtProp.intVal;
                    VariantClear(&vtProp);
                }

                // Simulated values for times as they are not readily available via WMI
                cpu.user = 0;
                cpu.nice = 0;
                cpu.sys = 0;
                cpu.idle = 0;
                cpu.irq = 0;

                cpus.push_back(cpu);
                pclsObj->Release();
            }

            pSvc->Release();
            pLoc->Release();
            pEnumerator->Release();
            CoUninitialize();

            return cpus;
        #else
            //code
        #endif
    }


    static std::string getHostname() {
        #if SK_OS == windows
            TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
            DWORD size = sizeof(buffer) / sizeof(buffer[0]);

            if (GetComputerNameEx(ComputerNameDnsHostname, buffer, &size)) {
                return std::string(buffer);
            }
            else {
                return "<unknown>";
            }

        #elif SK_OS == macos || SK_OS == ios
            char buffer[256];
            if (gethostname(buffer, sizeof(buffer)) == 0) {
                return std::string(buffer);
            }
            else {
                return "<unknown>";
            }
        #endif
    }

    static nlohmann::json getStaticInfo(){
        nlohmann::json json {
            {"EOL", "\\n"},
            {"endianess", (SK_Machine::isBigEndian() ? "BE" : "LE")},
            {"arch", SK_Machine::getCPUArch()},
            {"machine", SK_Machine::getMachineType()},
            {"platform", SK_Machine::getOSPlatform()},
            {"release", SK_Machine::getOSVersion()}, //OS build
            {"type", SK_Machine::getOSType()},
            {"version", SK_Machine::getOSVersion(false)},

            {"devNull", "/dev/null"},

            { "hostname", SK_Machine::getHostname()},

            { "homedir", SK::SK_Path_Utils::paths["home"] },
            { "tmpdir", SK::SK_Path_Utils::paths["temp"] }
        };

        #if SK_OS == android
        #elif SK_OS == macos || SK_OS == ios
        #elif SK_OS == linux
        #elif SK_OS == windows
            json["EOL"] = "\\r\\n";
            json["devNull"] = "\\\\.\\nul";
        #endif

           
                

        
        
        return json;
    }


    static inline std::string cpuModel;
    static std::string getCPUModel() {
        #ifdef SK_OS == windows
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return std::to_string(sysInfo.wProcessorArchitecture);

        #elif SK_OS == macos || SK_OS == ios
            char buffer[256];
            size_t size = sizeof(buffer);
            sysctlbyname("machdep.cpu.brand_string", buffer, &size, nullptr, 0);
            return std::string(buffer);

        #elif SK_OS == linux
            std::ifstream cpuInfoFile("/proc/cpuinfo");
            std::string line;
            while (std::getline(cpuInfoFile, line)) {
                if (line.find("model name") != std::string::npos) {
                    std::string::size_type pos = line.find(":");
                    return line.substr(pos + 1);
                }
            }
            return "unknown";

        #else
            return "unsupported";
        #endif
    }

    static inline int cpuSpeed;
    static int getCPUSpeed() {
        #if SK_OS == windows
            // Windows: Use the QueryPerformanceFrequency or other methods
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return 1;
        #elif SK_OS == macos || SK_OS == ios
            uint64_t cpuSpeed;
            size_t size = sizeof(cpuSpeed);
            if (sysctlbyname("hw.cpufrequency", &cpuSpeed, &size, nullptr, 0) == 0) {
                return stoi(std::to_string(cpuSpeed / 1000000));  // Convert to MHz
            }
            return "Unknown CPU speed";
        #elif SK_OS == linux || SK_OS == android
            // Linux: Read /proc/cpuinfo or use /sys to get CPU speed
            std::ifstream cpuInfoFile("/proc/cpuinfo");
            std::string line;
            while (std::getline(cpuInfoFile, line)) {
                if (line.find("cpu MHz") != std::string::npos) {
                    std::string::size_type pos = line.find(":");
                    return line.substr(pos + 1);
                }
            }

            // Try to get from /sys on Linux (can also work for Android)
            std::ifstream cpuSpeedFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
            if (cpuSpeedFile.is_open()) {
                std::string speed;
                std::getline(cpuSpeedFile, speed);
                return speed;  // CPU speed in kHz
            }

            return -1;
        #else
            return -2;
        #endif
    }

    static nlohmann::json getCPUInfo() {
        int coreCount = 0;

        #if SK_OS == windows
            // Windows: Use GetSystemInfo to get the number of CPU cores
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            coreCount = sysInfo.dwNumberOfProcessors;
        #elif SK_OS == macos || SK_OS == ios
            // macOS/iOS: Use sysctl to get the number of CPU cores
            int numCPU;
            size_t len = sizeof(numCPU);
            sysctlbyname("hw.ncpu", &numCPU, &len, nullptr, 0);
            coreCount = numCPU;

        #elif SK_OS == linux
            // Linux/Android: Use sysctl or read /proc/cpuinfo
            coreCount = std::thread::hardware_concurrency();  // Standard C++ way
        #endif

        nlohmann::json cpu {
            {"coreCount", coreCount}, //used by availableParallelism
        };


        //std::vector<CPUInfo> cpus = getCPUInformation();

        nlohmann::json cores = nlohmann::json();

        for (int i = 0; i < coreCount; i++) {
            cores.push_back({
                {"model", cpuModel},
                {"speed", cpuSpeed},
                {"times", {
                    {"user", 0},
                    {"nice", 0},
                    {"sys", 0},
                    {"idle", 0},
                    {"irq", 0},
                }}
            });
        }

        cpu["cores"] = cores;



        return cpu;//JSON2Resource(cpu);
    }

    static nlohmann::json getMemoryInfo() {
        #if SK_OS == windows
            MEMORYSTATUSEX memoryStatus;
            memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

            if (GlobalMemoryStatusEx(&memoryStatus)){
                
            } else {
                return respondError("Unable to get memory status");
            }

            nlohmann::json info {
                {"physical", {
                    {"free", memoryStatus.ullAvailPhys},
                    {"total", memoryStatus.ullTotalPhys},
                    {"used", memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys}
                }},

                {"page", {
                    {"free", memoryStatus.ullAvailPageFile},
                    {"total", memoryStatus.ullTotalPageFile},
                    {"used", memoryStatus.ullTotalPageFile - memoryStatus.ullAvailPageFile}
                }},

                {"virtual", {
                    {"free", memoryStatus.ullAvailVirtual},
                    {"total", memoryStatus.ullTotalVirtual},
                    {"used", memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual}
                }},

                {"extendedAvailable", (size_t)memoryStatus.ullAvailExtendedVirtual},
                {"usageInPercent",(size_t)memoryStatus.dwMemoryLoad}
            };
        
        #elif SK_OS == macos || SK_OS == ios
        
            MemoryInfo memInfo;

            // Get total memory
            int mib[2] = {CTL_HW, HW_MEMSIZE};
            uint64_t totalMemory;
            size_t size = sizeof(totalMemory);
            if (sysctl(mib, 2, &totalMemory, &size, NULL, 0) == 0) {
                memInfo.totalMemory = totalMemory;
            } else {
                std::cerr << "Failed to get total memory" << std::endl;
                memInfo.totalMemory = 0;
            }

            // Get free memory
            mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
            host_basic_info_data_t hostInfo;
            kern_return_t ret = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&hostInfo, &count);

            /*
            if (ret == KERN_SUCCESS) {
                 memInfo.freeMemory = hostInfo.memory_size - hostInfo.memory_used;
                 memInfo.usedMemory = hostInfo.memory_used;
            } else {
                std::cerr << "Failed to get memory statistics" << std::endl;
                 memInfo.freeMemory = 0;
                 memInfo.usedMemory = 0;
            }
             */
        
        
            nlohmann::json info {
                {"physical", {
                    {"free", 0},
                    {"total", 0},
                    {"used", 0}
                }},

                {"page", {
                    {"free", 0},
                    {"total", 0},
                    {"used", 0}
                }},

                {"virtual", {
                    {"free", 0},
                    {"total", 0},
                    {"used", 0}
                }},

                {"extendedAvailable", 0},
                {"usageInPercent", 0}
            };
        #endif
        
        return info;
    }

    static nlohmann::json getNetworkInfo() {
        nlohmann::json json {

        };

        return json;
    }

    static nlohmann::json getMachineTime() {
        #if SK_OS == windows
            double number = GetTickCount64();
        #elif SK_OS == macos || SK_OS == ios
            uint64_t number = 1;
        
            struct timeval boottime;
            size_t size = sizeof(boottime);

            // Get system boot time
            if (sysctlbyname("kern.boottime", &boottime, &size, NULL, 0) != 0) {
                int x = 0;
            } else {
                
                auto boot_ms = boottime.tv_sec * 1000 + boottime.tv_usec / 1000;
                
                // Current time
                auto now = std::chrono::system_clock::now();
                auto now_time = std::chrono::system_clock::to_time_t(now);
                auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                
                number = now_ms - boot_ms;
            }
        #endif
        
        double result = number / 1000.0;

        // Store the formatted result in a string variable
        std::ostringstream uptime_three_decimals;
        uptime_three_decimals << std::fixed << std::setprecision(3) << result;
        std::string uptime_three_decimals_str = uptime_three_decimals.str();

        nlohmann::json json {
            {"uptime", stof(uptime_three_decimals_str)}
        };

        return json;//JSON2Resource(info);
    }

    static nlohmann::json getTemplate() {
        nlohmann::json info = {
               
        };

        return info;//JSON2Resource(info);
    }
};
