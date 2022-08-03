#pragma once
#include "SDL.h"
#include "Animation.h"

class Entity
{
protected:
	Animation animation;
	bool life;
	std::string name;
	float dx;
	float dy;
	float angle;
	SDL_Rect rect_collision; // to determine collision between two entity
public:
	Entity();
	Entity(Animation& animation, std::string name = NULL, float x = 0, float y = 0, float angle = 0);

	bool operator==(Entity& second);
	virtual void update(const Uint8 * keyboard_state_array);
	virtual void update();
	virtual void draw(SDL_Renderer* renderer);
	bool isLife();
	void setCollision();
	void setLife(bool life);
	std::string get_name();
	float Get_x();
	float Get_y();
	float Get_angle();
	virtual ~Entity();
};

