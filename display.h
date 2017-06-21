// DISPLAY -- instructions for renderer (Enumeration)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <cstdint>

enum class DISPLAY : std::uint8_t {
    END = 0, REFRESH, RESIZE, RENDER
};

#endif