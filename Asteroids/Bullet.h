#pragma once
#include "Entity.h"
class Bullet :public Entity
{
public:
	Bullet(Animation& animation, std::string name, float x, float y, float angle);
	void draw(SDL_Renderer* renderer);
	void update();
	~Bullet();
};

