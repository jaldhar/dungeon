// dungeon.cc -- main routine
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <cstdlib>
#include <string>
using namespace std;

#include "create.h"
#include "display.h"
#include "input.h"
#include "render.h"
#include "world.h"

static const string TITLE("Dungeon");

int main() {
    Input  input;
    World  world;
    Render render;

    {
        Create create;
        create.init(world);
    }

    render.init(TITLE);
    render.update(DISPLAY::RENDER, world);

    DISPLAY display;
    while((display = input.update()) != DISPLAY::END) {
        render.update(display, world);
    };

    render.end();

    return EXIT_SUCCESS;
}