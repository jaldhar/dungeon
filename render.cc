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
#include "tile.h"
#include "world.h"

using TILEMAP = map<TERRAIN, chtype>;

static const int TILEHEIGHT = 1;
static const int TILEWIDTH = 1;

namespace {

    struct RenderImpl {
        RenderImpl();
        ~RenderImpl()=default;

        void    drawTitle();
        void    renderTitle();
        void    renderViewport(World& world, int top, int left);
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
        TILEMAP      _tilemap;
    } impl;

    RenderImpl::RenderImpl() : _title{nullptr}, _viewport{nullptr}, _lines{0},
    _cols{0}, _titleText(), _tilemap() {
    }

    void RenderImpl::renderTitle() {
        wclear(_title);
        const int len = _titleText.length();
        mvwaddstr(_title, 0, (impl._cols - len)/2, _titleText.c_str());
        wnoutrefresh(_title);
    }

    void RenderImpl::renderViewport(World& world, int top, int left) {
        int worldHeight = world.height();
        int worldWidth = world.width();

        for (int row = top; row < impl._lines + top; row += TILEHEIGHT) {
            if (row < 0 || row >= worldHeight) {
                continue;
            }

            for (int col = left; col < _cols + left; col += TILEWIDTH) {
                if (col < 0 || col >= worldWidth) {
                    continue;
                }

                chtype c;
                Tile& t = world.tileAt(row, col);

                if (t._visible) {
                    c = impl._tilemap[t._terrain];
                    c |= COLOR_PAIR(2);
                } else {
                    continue;
                }

                mvwaddch(_viewport, row, col, c);
            }
        }

        wnoutrefresh(_viewport);
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

    impl._tilemap[TERRAIN::EMPTY]    = ' '; // use of ACS_* requires this goes
    impl._tilemap[TERRAIN::CORRIDOR] = '#'; // after call to initscr().
    impl._tilemap[TERRAIN::FLOOR]    = '.';
    impl._tilemap[TERRAIN::C_WALL]   = '+';
    impl._tilemap[TERRAIN::H_WALL]   = ACS_HLINE;
    impl._tilemap[TERRAIN::V_WALL]   = ACS_VLINE;
    impl._tilemap[TERRAIN::UL_WALL]  = ACS_ULCORNER;
    impl._tilemap[TERRAIN::UR_WALL]  = ACS_URCORNER;
    impl._tilemap[TERRAIN::LR_WALL]  = ACS_LRCORNER;
    impl._tilemap[TERRAIN::LL_WALL]  = ACS_LLCORNER;
    impl._tilemap[TERRAIN::TT_WALL]  = ACS_TTEE;
    impl._tilemap[TERRAIN::RT_WALL]  = ACS_RTEE;
    impl._tilemap[TERRAIN::BT_WALL]  = ACS_BTEE;
    impl._tilemap[TERRAIN::LT_WALL]  = ACS_LTEE;

    curs_set(0);
    impl.resize();
    impl.renderTitle();
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
            int playerCol = world.playerCol();
            int playerRow = world.playerRow();
            int top = playerRow - (impl._lines + 1) / 2;
            int left = playerCol - (impl._cols + 1) / 2;

            impl.renderViewport(world, top, left);
        }
}

void Render::end() {
    impl.end(0);
}
