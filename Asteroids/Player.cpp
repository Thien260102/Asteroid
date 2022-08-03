#include "Player.h"
#include <math.h>
#define PI 3.14159265

Player::Player(){}

Player::Player(Animation& animation, std::string name, float x, float y, float angle): Entity(animation, name, x, y, angle)
{
	
}
void Player::update(const Uint8* keyboard_state_array) // player move
{
    SDL_Rect object = animation.destination_rect;
    if (keyboard_state_array[SDL_SCANCODE_UP])
    {
        object.x -= dx;
        object.y -= dy;
    }
    if (keyboard_state_array[SDL_SCANCODE_DOWN])
    {
        object.x += dx;
        object.y += dy;
    }
    if (keyboard_state_array[SDL_SCANCODE_LEFT])
        angle += 3;

    if (keyboard_state_array[SDL_SCANCODE_RIGHT])
        angle -= 3;


    if ((int)angle % 360 == 0)
        angle = 0;

    dx = 5 * (float)sin(angle * PI / 180);
    dy = 5 * (float)cos(angle * PI / 180);

    if (object.x > 1200)
        object.x = 0;
    if (object.x < 0)
        object.x = 1200 - 40;
    if (object.y > 800)
        object.y = 0;
    if (object.y < 0)
        object.y = 800 - 40;

    animation.destination_rect = object;
    //animation.update();
}

void Player::draw(SDL_Renderer* renderer)
{
    animation.draw(renderer, animation.destination_rect.x, animation.destination_rect.y, 40, 40, angle, SDL_FLIP_NONE);
}

Player::~Player(){}