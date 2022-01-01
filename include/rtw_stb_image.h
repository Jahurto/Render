//
// Created by 程龙昊 on 2021/12/24.
//

#ifndef RAYTRACING_NEXTWEEK_RTW_STB_IMAGE_H
#define RAYTRACING_NEXTWEEK_RTW_STB_IMAGE_H
// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
    #pragma warning (pop)
#endif
#endif //RAYTRACING_NEXTWEEK_RTW_STB_IMAGE_H
