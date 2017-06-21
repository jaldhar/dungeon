// COMMAND -- possible commands (Enumeration)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <cstdint>

enum class COMMAND : std::uint8_t {
    BAD = 0, IGNORE, QUIT, REFRESH, RESIZE
};

#endif