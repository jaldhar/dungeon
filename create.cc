// Create -- Sets up the dungeon (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>
using namespace std;

#include "create.h"
#include "tile.h"
#include "world.h"

static const int ROWS = 3;
static const int COLS = 3;

namespace {
    struct Position {
        int row;
        int col;
    };

    struct CreateImpl {
        CreateImpl() {};
        ~CreateImpl()=default;

        void makeConnections(World& world);
        void makeGrid(World& world);
        void makeHorizontalCorridor(World& world, int row, int col1, int col2);
        void makeTwistyCorridor(World& world, int ax, int ay, int bx, int by);
        void makeVerticalCorridor(World& world, int row1, int row2, int col);

        int _sectorHeight;
        int _sectorWidth;
        vector<vector<int>> _neighbors;
        vector<Position> _centers;
    } impl;

    void CreateImpl::makeConnections(World& world) {
        const int numCenters = ROWS * COLS;

        // Initially all centers are unconnected;
        bool connected[numCenters]{false};
        vector<pair<int, int>> connections;

        // utility function to normalize entries in the connections list so that
        // the smaller of the pair is first.  This helps us avoid "mirror"
        // entries such as [1,3] and [3,1].
        auto sorted_pair = [](int a, int b) {
            return (a < b) ? make_pair(a,b) : make_pair(b,a);
        };

        uniform_int_distribution<int> randCenter(0, numCenters - 1);

        // Pick a random unconnected center to begin with.
        int centerFrom = randCenter(world.random());
        int lastCenter = -1;

        // until all centers are connected atleast once...
        while (any_of(begin(connected), end(connected), [](bool i){
            return !i;
        })) {
            // try connecting it to a random neighbor.
            uniform_int_distribution<int> randNeighbor(0,
                _neighbors[centerFrom].size() - 1);
            int centerTo = _neighbors[centerFrom][randNeighbor(world.random())];

            // Avoid backtracking.
            if (centerTo == lastCenter) {
                continue;
            }

            // it's a successful connection...
            connected[centerFrom] = true;

            // But only include it in the connections list if the two centers
            // aren't already connected.
            auto candidate = sorted_pair(centerFrom, centerTo);
            if(find_if(begin(connections), end(connections), [&candidate]
            (pair<int, int>& connection){ return candidate == connection; }) == 
            end(connections)) {
                connections.push_back(candidate);
            }

            // Now repeat the process with the next center.
            lastCenter = centerFrom;
            centerFrom = centerTo;
        }

        // Make corridors in all the connections
        for (auto& connection: connections) {
            makeTwistyCorridor(world, _centers[connection.first].row, 
                _centers[connection.first].col, _centers[connection.second].row,
                _centers[connection.second].col);
        }
    }

    void CreateImpl::makeGrid(World& world) {
        const int worldHeight = world.height();
        const int worldWidth = world.width();

        // Determine the size of each sector...
        _sectorHeight = (worldHeight - (worldHeight % ROWS)) / ROWS;
        _sectorWidth = (worldWidth - (worldWidth % COLS)) / COLS;

        const int heightOffset = (worldHeight % ROWS) / 2;
        const int widthOffset = (worldWidth % COLS) / 2;
        const int sectors = ROWS * COLS;

        // ...and which ones are neighbors.
        _neighbors.resize(sectors);
        for (int i = 0; i < sectors; i++) {
            if (i / ROWS != 0) {
                _neighbors[i].push_back(i - ROWS);
            }

            if (i / ROWS != ROWS - 1) {
                _neighbors[i].push_back(i + ROWS);
            }

            if (i % COLS != 0) {
                _neighbors[i].push_back(i - 1);
            }

            if (i % COLS != COLS - 1) {
                _neighbors[i].push_back(i + 1);
            }
        }

        // Pick a random spot somewhere in the middle of each sector
        _centers.resize(sectors);
        uniform_int_distribution<int> randRow(_sectorHeight / 4,
            _sectorHeight * 3 / 4);
        uniform_int_distribution<int> randCol(_sectorWidth / 4,
            _sectorWidth * 3 / 4);

        for (int i = 0; i < sectors; i++) {
            _centers[i].row = heightOffset + ((i / ROWS) * _sectorHeight ) +
                randRow(world.random());
            _centers[i].col = widthOffset + ((i % COLS) * _sectorWidth ) +
                randCol(world.random());
            // Normalize _center so it falls on even rows and cols.
            if (_centers[i].row % 2 == 1) {
                _centers[i].row--;
            }
            if (_centers[i].col % 2 == 1) {
                _centers[i].col--;
            }
        }
    }

    void CreateImpl::makeHorizontalCorridor(World& world, int row, int col1,
    int col2) {
        for (int i = col1; i <= col2; i++) {
            auto& t = world.tileAt(row, i);
            t._passable = true;
            t._terrain = TERRAIN::CORRIDOR;
        }
    }

    void CreateImpl::makeTwistyCorridor(World& world, int ay, int ax, int by,
    int bx) {

        // decide direction in which we will join segments
        uniform_int_distribution<int> randBool(0, 1);
        bool flip = randBool(world.random());

        int cx, cy, diffx, diffy;

        diffx = abs(ax - bx);
        diffy = abs(ay - by);
        // prevent divide by 0 errors when diffx, diffy are too small.
        if (diffx < 4) {
            diffx = 4;
        }
        if (diffy < 4) {
            diffy = 4;
        }

        uniform_int_distribution<int> randX(diffx / 4, diffx * 3 / 4);
        if (ax < bx) {
            cx = ax + randX(world.random());
        } else {
            cx = bx + randX(world.random());
        }

        uniform_int_distribution<int> randY(diffy / 4, diffy * 3 / 4);
        if (ay < by) {
            cy = ay + randY(world.random());
        } else {
            cy = by + randY(world.random());
        }

        if (ax < bx) {                      // a to west of b
            if (flip) {
                makeHorizontalCorridor(world, cy, ax, bx);
            } else {
                makeHorizontalCorridor(world, ay, ax, cx);
                makeHorizontalCorridor(world, by, cx, bx);
            }
        } else {                            // a to east of b
            if (flip) {
                makeHorizontalCorridor(world, cy, bx, ax);
            } else {
                makeHorizontalCorridor(world, ay, cx, ax);
                makeHorizontalCorridor(world, by, bx, cx);
            }
        }

        if (ay < by) {                      // a to north of b
            if (flip) {
                makeVerticalCorridor(world, ay, cy, ax);
                makeVerticalCorridor(world, cy, by, bx);
            } else {
                makeVerticalCorridor(world, ay, by, cx);
            }
        } else {                            // a to south of b
            if (flip) {
                makeVerticalCorridor(world, cy, ay, ax);
                makeVerticalCorridor(world, by, cy, bx);
            } else {
                makeVerticalCorridor(world, by, ay, cx);
            }
        }
    }

    void CreateImpl::makeVerticalCorridor(World& world, int row1, int row2,
    int col) {
        for (int i = row1; i <= row2; i++) {
            auto& t = world.tileAt(i, col);
            t._passable = true;
            t._terrain = TERRAIN::CORRIDOR;
        }
    }
}

void Create::init(World& world) {

    // 1 Divide the map into a grid (Rogue uses 3x3, but any size will work).
    //
    // 2. Give each grid a flag indicating if it's "connected" or not, and an
    //    array of which grid numbers it's connected to.
    impl.makeGrid(world);

    // 3. Pick a random room to start with, and mark it "connected".
    //
    // 4. While there are unconnected neighbor rooms, connect to one of them,
    // make that the current room, mark it "connected", and repeat.
    //
    // 5. While there are unconnected rooms, try to connect them to a random 
    // connected neighbor (if a room has no connected neighbors yet, just keep 
    // cycling, you'll fill out to it eventually).
    //
    // 6. All rooms are now connected at least once.

    // 7. Make 0 or more random connections to taste; I find rnd(grid_width)
    // random connections looks good.
    impl.makeConnections(world);
}
