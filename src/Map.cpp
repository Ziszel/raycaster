//
// Created by calin on 15/03/23.
//
#include "Map.h"

Map::Map() {

}

void Map::Draw() {
    for (int i = 0; i < map_size_y; ++i)
    {
        for (int j = 0; j < map_size_x; ++j)
        {
            if (map_data.at((i * 10) + j) == 1)
            {
                DrawRectangle(j * block_size.x,
                              i * block_size.y,
                              block_size.x - 1,
                              block_size.y - 1,
                              RAYWHITE);
            }
        }
    }
}
