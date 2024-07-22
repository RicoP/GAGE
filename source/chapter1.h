#pragma once

#include "gage.h"

#ifdef GAGE_BUILD_DLL
#   if defined(GAGE_BUILD_DLL_ENGINE)
#       define GAGE_GAME_API __declspec(dllimport)
#   elif defined(GAGE_BUILD_DLL_GAME)
#       define GAGE_GAME_API extern "C" __declspec(dllexport)
#   else
#       error "UNKNOWN DLL type"
#   endif
#else
#define GAGE_GAME_API
#endif

GAGE_GAME_API void Chapter1();