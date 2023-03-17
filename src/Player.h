//
// Created by calin on 15/03/23.
//
#pragma once
#include <cmath>
#include <memory>
#include "../include/raylib-cpp.hpp"
#include "Map.h"

class Player
{
private:
    raylib::Vector2 position;
    raylib::Vector2 delta_position;
    raylib::Vector2 size;
    float angle;
    float speed = 250;

public:
    Player();
    Player(raylib::Vector2 position, raylib::Vector2 size, float angle);
    void Update(float& delta_time, std::unique_ptr<Map>& map);
    void Draw(float& delta_time);
    void DrawRays(std::unique_ptr<Map>& map);
    void CheckRays(std::unique_ptr<Map>& map);
    void Move(float& delta_time);
    float Dist(float& ax, float& ay, float& bx, float& by, float& ang);
};