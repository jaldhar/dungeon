// TERRAIN -- possible tile types (Enumeration)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <cstdint>

enum class TERRAIN : std::uint8_t {
    EMPTY = 0, CORRIDOR, FLOOR, C_WALL, H_WALL, V_WALL, UL_WALL, UR_WALL,
    LL_WALL, LR_WALL, TT_WALL, RT_WALL, BT_WALL, LT_WALL
};

#endif