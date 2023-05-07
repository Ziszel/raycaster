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
    int ray_num, map_x, map_y, map_pos, dof; // rayNum, mapX, mapY, mapPos, Depth of Field
    float ray_x, ray_y, ray_angle, x_offset, y_offset, disT; // distance of current ray
    float degree_in_radians = 0.0174533; // https://www.rapidtables.com/convert/number/degrees-to-radians.html
    int rays_to_cast = 60; // which will also give the FOV
    ray_angle = angle - degree_in_radians * 30; // offset the initial ray by 30 degrees from the current player angle.
    // Catch if the ray_angle goes over or below the lower bounds of the unit circle
    // If it does, reset it to either the minimum or maximum value
    if (ray_angle < 0)
    {
        ray_angle += 2 * PI;
    }
    if (ray_angle > 2 * PI)
    {
        ray_angle -= 2 * PI;
    }

    // check which line the ray hits a wall first (horizontal or vertical)
    for (ray_num = 0; ray_num < rays_to_cast; ++ray_num) {
        // --- horizontal lines ---
        dof = 0;
        float disV = 100000.0f; // max out the distance
        // used to calculate the vertical distance if a ray hits a wall, for now set to the player's position
        float vx = position.x;
        float vy = position.y;
        // https://www.math.net/tangent
        float n_tan = -tan(ray_angle);
        if (ray_angle > PI / 2 && ray_angle < 3 * PI / 2) // ray pointing left
        {
            // set the parameters to check for left / right ray distance respectively
            ray_x = ((((int) position.x / map->block_size.x) * map->block_size.x) - 0.0001); // get the 64th of its value (because each tile/block size IS 64)
            // // https://www.youtube.com/watch?v=gYRrGTC7GtA
            ray_y = (position.x - ray_x) * n_tan + position.y;
            x_offset = -64;
            y_offset = -x_offset * n_tan;
        }
        if (ray_angle < (PI / 2) || ray_angle > (3 * PI / 2)) // ray pointing right
        {
            ray_x = ((((int) position.x / 64) * 64) + 64);
            ray_y = (position.x - ray_x) * n_tan + position.y;
            x_offset = 64;
            y_offset = -x_offset * n_tan;
        }
        if (ray_angle == 0 || ray_angle == PI) // impossible for the ray to hit a vertical line (looking directly up or down)
        {
            ray_x = position.x;
            ray_y = position.y;
            dof = 8;
        }
        // check to see if the ray is hitting a wall, if so calculate the distance and store for drawing
        while (dof < 8) {
            map_x = (int) ray_x / 64;
            map_y = (int) ray_y / 64;
            map_pos = map_y * 10 + map_x; // get the current map position
            // get map position of mx and my and then check if it is a 1 and therefore a wall
            if (map_pos > 0 && map_pos < (12 * 10) && map->map_data.at(map_pos) == 1) {
                // set the v positions == to the ray
                vx = ray_x;
                vy = ray_y;
                // calculate the rays distance from the player
                disV = Dist(position.x, position.y, vx, vy, ray_angle);
                dof = 8;
            } else { // Because the map is an evenly spaced grid, apply the offset to check if the next grid space is a wall
                ray_x += x_offset;
                ray_y += y_offset;
                dof += 1;
            }
        }
        // --- vertical ---
        dof = 0;
        float disH = 100000.0f; // max out the distance
        // used to calculate the horizontal distance if a ray hits a wall, for now set to the player's position
        float hx = position.x;
        float hy = position.y;
        float a_tan = -1 / tan(ray_angle);
        if (ray_angle > PI) // ray pointing upwards
        {
            // set the parameters to check for up / down ray distance respectively
            ray_y = ((((int) position.y / 64) * 64) - 0.0001);
            ray_x = (position.y - ray_y) * a_tan + position.x;
            y_offset = -64;
            x_offset = -y_offset * a_tan;
        }
        if (ray_angle < PI) // ray pointing downwards
        {
            ray_y = ((((int) position.y / 64) * 64) + 64);
            ray_x = (position.y - ray_y) * a_tan + position.x;
            y_offset = 64;
            x_offset = -y_offset * a_tan;
        }
        if (ray_angle == PI || ray_angle == 0) // impossible for the ray to hit a horizontal line (looking directly left or right)
        {
            ray_x = position.x;
            ray_y = position.y;
            dof = 8;
        }
        // check to see if the ray is hitting a wall, if so calculate the distance and store for drawing
        while (dof < 8) {
            map_x = ray_x / 64;
            map_y = ray_y / 64;
            map_pos = map_y * 10 + map_x;
            // get map position of mx and my and then check if it is a 1 and therefore a wall
            if (map_pos > 0 && map_pos < (12 * 10) && map->map_data.at(map_pos) == 1) {
                hx = ray_x;
                hy = ray_y;
                // calculate the rays distance from the player
                disH = Dist(position.x, position.y, hx, hy, ray_angle);
                dof = 8;
            } else { // Because the map is an evenly spaced grid, apply the offset to check if the next grid space is a wall
                ray_x += x_offset;
                ray_y += y_offset;
                dof += 1;
            }
        }
        // hit a vertical wall first, so draw from that distance
        if (disV < disH) {
            ray_x = vx;
            ray_y = vy;
            disT = disV;
        }
        // hit a horizontal wall first, so draw from that distance
        if (disH < disV) {
            ray_x = hx;
            ray_y = hy;
            disT = disH;
        }
        // Draw 3D
        float current_angle = angle - ray_angle; // distance between the player's current angle and the ray angle
        // Limits to ensure ca does not go out of bounds, crashing the application
        if (current_angle < 0) { current_angle += 2 * PI; }
        if (current_angle > 2 * PI) { current_angle -= 2 * PI; }

        // multiplying the ray distance by the cosine of the new angle (ca) fixes the fisheye effect
        // https://gamedev.stackexchange.com/questions/97574/how-can-i-fix-the-fisheye-distortion-in-my-raycast-renderer#97580
        // http://www.playfuljs.com/a-first-person-engine-in-265-lines/
        disT = disT * cos(current_angle);
        // Line height
        float lineH = 48 * 1280 / disT; // 1280 is the size of the screen, but not accessible to the player class
        if (lineH > 1280) // cap it to the screen size to stop unnecessary draw calls
        {
            lineH = 1280;
        }

        // drawing from left to right for each part of the map
        for (int i = 0; i < map->map_size_x; ++i)
        {
            // draw from midpoint of the screen on the x to the end of the screen &
            // to from 0 to lineH on the y
            // For x: the ray * the size of the block + midpoint + iteration
            // For y: 0 -> lineH (the size of the wall calculated with distance from player)
            DrawLine(ray_num * 10 + 640 + i, 0,
                     ray_num * 10 + 640 + i, lineH, WHITE);
        }

        // Draw 2D
        DrawLine(position.x + size.x / 2, position.y + size.y / 2, ray_x, ray_y, GREEN);

        ray_angle += degree_in_radians; // increment the angle of the ray for drawing the next ray
        if (ray_angle < 0)
        {
            ray_angle += 2 * PI;
        }
        if (ray_angle > 2 * PI)
        {
            ray_angle -= 2 * PI;
        }
    }
}