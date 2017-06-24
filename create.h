// Create -- Sets up the dungeon (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _CREATE_H_
#define _CREATE_H_

struct World;

struct Create {
    Create() {};
    ~Create()=default;

    void init(World& world);
};

#endif