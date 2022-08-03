#include "Bullet.h"
#define PI 3.14159265

Bullet::Bullet(Animation& animation, std::string name, float x, float y, float angle) : Entity(animation, name, x, y, angle)
{
	dx = 10 * (float)sin(angle * PI / 180);
	dy = 10 * (float)cos(angle * PI / 180);

}

void Bullet::draw(SDL_Renderer* renderer)
{
	animation.draw(renderer, animation.destination_rect.x, animation.destination_rect.y, 32, 64, angle, SDL_FLIP_VERTICAL);
}

void Bullet::update()
{
	SDL_Rect object = animation.destination_rect;
	object.x += dx;
	object.y += dy;
	if (object.x > 1200 || object.x < 0
		|| object.y < 0 || object.y > 800)
		life = false;
	
	animation.destination_rect = object;
	//animation.update();
}


Bullet::~Bullet(){}