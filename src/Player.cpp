//
// Created by calin on 15/03/23.
//
#include "Player.h"

Player::Player() {

}

Player::Player(raylib::Vector2 position, raylib::Vector2 size, float angle) {
    this->position = position;
    this->delta_position.x = cos(angle) * 5;
    this->delta_position.y = sin(angle) * 5;
    this->size = size;
    this->angle = angle;
}

void Player::Update(float& delta_time, std::unique_ptr<Map>& map) {
    Move(delta_time);
    DrawRays(map);
}

void Player::Draw(float& delta_time) {
    DrawRectangle(position.x, position.y, size.x, size.y, RED);
}

// https://gamedev.stackexchange.com/questions/165965/how-is-raycasting-implemented
// https://lodev.org/cgtutor/raycasting.html
// https://www.youtube.com/watch?v=gYRrGTC7GtA
void Player::DrawRays(std::unique_ptr<Map>& map) {
    CheckRays(map);
}

void Player::Move(float& delta_time) {
    if (IsKeyDown(KEY_A))
    {
        //position.x -= speed * delta_time;
        angle -= 0.1;
        if (angle < 0) { angle += M_PI * 2; }
        delta_position.x = cos(angle) * 5;
        delta_position.y = sin(angle) * 5;
    }
    else if (IsKeyDown(KEY_D))
    {
        //position.x += speed * delta_time;
        angle += 0.1;
        if (angle > M_PI * 2) { angle = 0; }
        delta_position.x = cos(angle) * 5;
        delta_position.y = sin(angle) * 5;
    }
    if (IsKeyDown(KEY_W))
    {
        //position.y -= speed * delta_time;
        position.x += delta_position.x;
        position.y += delta_position.y;
    }
    else if (IsKeyDown(KEY_S))
    {
        //position.y += speed * delta_time;
        position.x -= delta_position.x;
        position.y -= delta_position.y;
    }
}

float Player::Dist(float& ax, float& ay, float& bx, float& by, float& ang)
{
    // Pythagoras theorem to return the distance between
    return sqrt((bx - ax)*(bx - ax) + (by - ay) * (by - ay));
}

void Player::CheckRays(std::unique_ptr<Map>& map) {
    int r, mx, my, mp, dof; // rayNum, mapX, mapY, mapPos, Depth of Field
    float rx, ry, ra, xo, yo; // rayX, rayY, rayAngle, xOffset, yOffset
    float degree_in_radians = 0.0174533;
    int rays_to_cast = 60;
    ra = angle - degree_in_radians * 30; // offset the initial ray by 30 degrees.
    if (ra < 0)
    {
        ra += 2 * PI;
    }
    if (ra > 2 * PI)
    {
        ra -= 2 * PI;
    }

    // check which line the ray hits a wall first (horizontal or vertical)
    for (r = 0; r < rays_to_cast; ++r) {
        // --- horizontal lines ---
        dof = 0;
        float disV = 100000.0f;
        float vx = position.x;
        float vy = position.y;
        // https://www.math.net/tangent
        float n_tan = -tan(ra);
        if (ra > PI / 2 && ra < 3 * PI / 2) // ray pointing left
        {
            rx = ((((int) position.x / 64) * 64) - 0.0001); // get the 64th of its value
            ry = (position.x - rx) * n_tan + position.y;
            xo = -64;
            yo = -xo * n_tan;
        }
        if (ra < (PI / 2) || ra > (3 * PI / 2)) // ray pointing right
        {
            rx = ((((int) position.x / 64) * 64) + 64);
            ry = (position.x - rx) * n_tan + position.y;
            xo = 64;
            yo = -xo * n_tan;
        }
        if (ra == 0 || ra == PI) // impossible for the ray to hit a vertical line (looking directly up or down)
        {
            rx = position.x;
            ry = position.y;
            dof = 8;
        }
        while (dof < 8) {
            mx = (int) rx / 64;
            my = (int) ry / 64;
            mp = my * 10 + mx; // this is going negative
            // get map position of mx and my and then check if it is a 1 and therefore a wall
            std::cout << mp << "\n";
            if (mp > 0 && mp < (12 * 10) && map->map_data.at(mp) == 1) {
                vx = rx;
                vy = ry;
                // calculate the rays distance from the player
                disV = Dist(position.x, position.y, vx, vy, ra);
                dof = 8;
            } else { // Because the map is an evenly spaced grid, apply the offset to check if the next grid space is a wall
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        // --- horizontal ---
        dof = 0;
        float disH = 100000.0f;
        float hx = position.x;
        float hy = position.y;
        float a_tan = -1 / tan(ra);
        if (ra > PI) // ray pointing upwards
        {
            ry = ((((int) position.y / 64) * 64) - 0.0001);
            rx = (position.y - ry) * a_tan + position.x;
            yo = -64;
            xo = -yo * a_tan;
        }
        if (ra < PI) // ray pointing downwards
        {
            ry = ((((int) position.y / 64) * 64) + 64);
            rx = (position.y - ry) * a_tan + position.x;
            yo = 64;
            xo = -yo * a_tan;
        }
        if (ra == PI || ra == 0) // impossible for the ray to hit a horizontal line (looking directly left or right)
        {
            rx = position.x;
            ry = position.y;
            dof = 8;
        }
        while (dof < 8) {
            mx = rx / 64;
            my = ry / 64;
            mp = my * 10 + mx;
            // get map position of mx and my and then check if it is a 1 and therefore a wall
            std::cout << mp << "\n";
            if (mp > 0 && mp < (12 * 10) && map->map_data.at(mp) == 1) {
                hx = rx;
                hy = ry;
                // calculate the rays distance from the player
                disH = Dist(position.x, position.y, hx, hy, ra);
                dof = 8;
            } else { // Because the map is an evenly spaced grid, apply the offset to check if the next grid space is a wall
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        if (disV < disH) {
            rx = vx;
            ry = vy;
        }
        if (disH < disV) {
            rx = hx;
            ry = hy;
        }
        DrawLine(position.x + size.x / 2, position.y + size.y / 2, rx, ry, GREEN);
        ra += degree_in_radians; // increment the angle of the ray for drawing the next ray
        if (ra < 0)
        {
            ra += 2 * PI;
        }
        if (ra > 2 * PI)
        {
            ra -= 2 * PI;
        }
    }
}