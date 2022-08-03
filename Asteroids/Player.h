#pragma once
#include "Entity.h"
class Player :public Entity
{
public:
	Player();
	Player(Animation& animation, std::string name, float x, float y, float angle);
	void update(const Uint8* keyboard_state_array);
	void draw(SDL_Renderer* renderer);
	~Player();
};

