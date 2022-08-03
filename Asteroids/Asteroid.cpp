#include "Asteroid.h"

Asteroid::Asteroid(Animation& animation, std::string name, float x, float y, float angle) : Entity(animation, name, x, y, angle)
{
	
	dx = (int)x % 8 - 5;
	dy = (int)y % 8 - 5;
	if (dx <= 0)
		dx = 3;
	if (dy <= 0)
		dy = 3;
}

void Asteroid::update()  // window_width = 1200, window_height = 800
{
	SDL_Rect object = animation.destination_rect;
	object.x += dx;
	object.y += dy;
	object.x = object.x > 1200 ? 0 : (object.x < 0 ? 1200 : object.x);
	object.y = object.y > 800 ? 0 : (object.y < 0 ? 800 : object.y);

	animation.destination_rect = object;
	//animation.update();
}

void Asteroid::draw(SDL_Renderer* renderer)
{
	this->update();
	animation.draw(renderer, animation.destination_rect.x, animation.destination_rect.y, 64, 64);
}


Asteroid::~Asteroid(){}