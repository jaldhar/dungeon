// World -- Data model (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <array>
using namespace std;

#include "world.h"

static const int MAP_HEIGHT = 24;
static const int MAP_WIDTH = 80;

namespace {
    struct WorldImpl {
        WorldImpl();
        ~WorldImpl()=default;

        int _height;
        int _width;
        int _playerRow;
        int _playerCol;
        array<array<Tile, MAP_WIDTH>, MAP_HEIGHT> _map;
        mt19937 _rnd;
    } impl;

    WorldImpl::WorldImpl() : _playerRow{0}, _playerCol{0}, _map(),
    _rnd{random_device{}()} {}
}

int World::height() const {
    return MAP_HEIGHT;
}

int World::width() const {
    return MAP_WIDTH;
}

int World::playerRow() const {
    return impl._playerRow;
}

int World::playerCol() const {
    return impl._playerCol;
}

mt19937& World::random() {
    return impl._rnd;
}

Tile& World::tileAt(int row, int col) {
    return impl._map[row][col];
}
