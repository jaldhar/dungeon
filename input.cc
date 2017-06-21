// Input -- keyboard controller (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <map>
using namespace std;

#include "ncurses.h"

#include "command.h"
#include "input.h"
#include "world.h"


using COMMANDMAP = map<int, COMMAND>;

namespace {
    struct InputImpl {
        InputImpl();
        ~InputImpl()=default;

        void badInput();

        COMMANDMAP _commandKeys;
    } impl;

    InputImpl::InputImpl() : _commandKeys {
        { 'Q',          COMMAND::QUIT},
        { 0x12,         COMMAND::REFRESH}, // CTRL-R key. Handled in Render
        { KEY_RESIZE,   COMMAND::RESIZE},  // SIGWINCH. Handled in Render
    } {
    }

    void InputImpl::badInput() {
        beep();
    }
}

DISPLAY Input::update() {
    COMMAND command{COMMAND::BAD};
    int c;

    c = getch();
    auto it = impl._commandKeys.find(c);
    if (it != impl._commandKeys.end()) {
        command = it->second;
    }

    switch(command) {
        case COMMAND::REFRESH:
            return DISPLAY::REFRESH;
        case COMMAND::RESIZE:
            return DISPLAY::RESIZE;
        case COMMAND::BAD:
            impl.badInput();
            break;
        case COMMAND::QUIT:
            return DISPLAY::END;
        case COMMAND::IGNORE:
        default:
            break;
    }
    return DISPLAY::RENDER;
}
