// World -- Data model (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _WORLD_H_
#define _WORLD_H_

#include "tile.h"

struct World {
    World() {};
    ~World()=default;

    int height() const;
    int width() const;
    int playerRow() const;
    int playerCol() const;
    Tile& tileAt(int row, int col);
};

#endif