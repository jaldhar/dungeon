// Render -- Display routines (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _RENDER_H_
#define _RENDER_H_

#include <string>
#include "display.h"
struct World;

struct Render {
    Render() {};
    ~Render()=default;

    void init(std::string titleText);
    void update(DISPLAY display, World& world);
    void end();
};

#endif