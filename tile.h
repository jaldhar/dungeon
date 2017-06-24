// Tile -- a discrete space in the dungeon (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef _TILE_H_
#define _TILE_H_

#include "terrain.h"

struct Tile {
    Tile() : _visible{true}, _passable{false}, _terrain{TERRAIN::EMPTY} {};
    ~Tile()=default;

    bool    _visible;
    bool    _passable;
    TERRAIN _terrain;
};

#endif