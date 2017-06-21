// Render -- Display routines (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <algorithm>
#include <clocale>
#include <csignal>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
using namespace std;

#include "ncurses.h"
// These ncurses macros name clash with c++ symbols on old versions of ncurses
#if NCURSES_MAJOR_VERSION < 5 || (NCURSES_MAJOR_VERSION == 5 && NCURSES_MINOR_VERSION < 9)
#undef box
#undef clear
#undef erase
#undef move
#undef refresh
#endif

#include "render.h"
#include "world.h"

namespace {

    struct RenderImpl {
        RenderImpl();
        ~RenderImpl()=default;

        void    drawTitle();
        void    refresh();
        void    resize();
        void    setTitleText(string titleText);
        void    setTitleWin(WINDOW*& win);

        static int  createTitleWin(WINDOW*, int);
        static void end(int);

        WINDOW*      _title;
        WINDOW*      _viewport;
        int          _lines;
        int          _cols;
        string       _titleText;
    } impl;

    RenderImpl::RenderImpl() : _title{nullptr}, _viewport{nullptr}, _lines{0},
    _cols{0}, _titleText() {
    }

    void RenderImpl::RenderTitle() {
        wclear(_title);
        const int len = _titleText.length();
        mvwaddstr(_title, 0, (impl._cols - len)/2, _titleText.c_str());
        wnoutrefresh(_title);
    }

    void RenderImpl::refresh() {
        redrawwin(_title);
        doupdate();
    }

    void RenderImpl::resize() {
        getmaxyx(stdscr, _lines, _cols);
        wbkgd(stdscr, ' ');

        wresize(_title, 1 , _cols);
        wbkgd(_title, ' ' | COLOR_PAIR(2));
    }

    void RenderImpl::setTitleText(string titleText) {
        _titleText = titleText;
    }

    void RenderImpl::setTitleWin(WINDOW*& win) {
        _title = win;
    }

    int RenderImpl::createTitleWin(WINDOW* win, int /* ncols */) {
        impl.setTitleWin(win);

        return 0;
    }

    void RenderImpl::end(int /* sig */) {
        curs_set(1);
        endwin();
        clear();
        exit(EXIT_SUCCESS);
    }
}

void Render::init(string titleText) {
    setlocale(LC_ALL, "POSIX");

    struct sigaction act;
    act.sa_handler = RenderImpl::end;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);

    impl.setTitleText(titleText);
    ripoffline(1, RenderImpl::createTitleWin);

    initscr();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    nodelay(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
    }

    curs_set(0);
    impl.resize();
    impl.drawTitle();
}

// Called every time the screen has to be redrawn.  There are two special
// cases:
// 1. If the refresh key (CTRL-R) has been pressed in which case the screen is
//    refreshed and redrawn.
// 2. If the screen is resized in which case the screen is redrawn.
// Otherwise the screen is redrawn.
void Render::update(DISPLAY display, World& world) {
        if (display == DISPLAY::REFRESH) {
            impl.refresh();
        } else if (display == DISPLAY::RESIZE) {
            impl.resize();
        } else {
            // render
        }
}

void Render::end() {
    impl.end(0);
}
