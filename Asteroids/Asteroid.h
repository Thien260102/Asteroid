#pragma once
#include "Entity.h"
class Asteroid :public Entity
{
public:
	Asteroid(Animation& animation, std::string name, float x, float y, float angle);

	void update();
	void draw(SDL_Renderer* renderer);
	~Asteroid();
};

