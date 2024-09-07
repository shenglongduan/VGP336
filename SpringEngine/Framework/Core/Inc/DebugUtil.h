#pragma once

#include "TimeUtil.h"

using namespace SpringEngine::Core::TimeUtil;

#if defined(_DEBUG)
#define LOG(format, ...)\
    do{\
        char buffer[256];\
        int res = snprintf(buffer, std::size(buffer), "{%.3f}: "##format##"\n", GetTime(), __VA_ARGS__);\
        OutputDebugStringA(buffer);\
    }while(false)

#define ASSERT(condition, format, ...)\
    do{\
        if(!(condition))\
        {\
            LOG("ASSERT! %s(%d)\n"##format##, __FILE__, __LINE__, __VA_ARGS__);\
            DebugBreak();\
        }\
    }while(false)
#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...) do{(void)sizeof(condition);}while(false)
#endif
