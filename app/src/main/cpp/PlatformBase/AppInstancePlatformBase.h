//
// Created by Matthew Good on 15/7/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPINSTANCEPLATFORMBASE_H
#define GRAPHICAL_TOOL_KIT_APPINSTANCEPLATFORMBASE_H

#include <MultiTouch/MultiTouch.h>

#if PLATFORM_WIN32 || PLATFORM_UNIVERSAL_WINDOWS
    #include "WindowsBase/AppInstanceWindowsBase.h"
    typedef AppInstanceWindowsBase AppInstancePlatformBase;
#elif PLATFORM_ANDROID
    #include "AndroidBase/AppInstanceAndroidBase.h"
    typedef AppInstanceAndroidBase AppInstancePlatformBase;
#elif PLATFORM_LINUX
    #include "LinuxBase/AppInstanceLinuxBase.h"
    typedef AppInstanceLinuxBase AppInstancePlatformBase;
#elif PLATFORM_MACOS
    #include "MacOSBase/AppInstanceMacOSBase.h"
    typedef AppInstanceMacOSBase AppInstancePlatformBase;
#elif PLATFORM_IOS
    #include "IOSBase/AppInstanceIOSBase.h"
    typedef AppInstanceIOSBase AppInstancePlatformBase;
#elif PLATFORM_TVOS
    #include "TVOSBase/AppInstanceTVOSBase.h"
    typedef AppInstanceTVOSBase AppInstancePlatformBase;
#else
    #error "Unknown platform"
#endif

#endif //GRAPHICAL_TOOL_KIT_APPINSTANCEPLATFORMBASE_H
