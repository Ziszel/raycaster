//
// Created by calin on 15/03/23.
//
#pragma once
#include <vector>
#include "../include/raylib-cpp.hpp"
#include <iostream>


class Map {
private:
    std::vector<int> map_data = {
            // 17x10 grid representing the map. 1 = wall, 0 = space. 1 grid = 64px
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    raylib::Vector2 block_size = {64, 64};
    float map_size_x = 10;
    float map_size_y = 12;
public:
    Map();
    void Draw();

    friend class Player;
};
