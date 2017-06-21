// Input -- keyboard controller (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _INPUT_H_
#define _INPUT_H_

#include "display.h"

struct Input {
    Input() {};
    ~Input()=default;

    DISPLAY update();
};

#endif