#include "Entity.h"

Entity::Entity(){}

Entity::Entity(Animation& animation, std::string name, float x, float y, float angle)
{
	life = true;
	this->name = name;
	this->animation = animation;
	this->animation.destination_rect.x = x;
	this->animation.destination_rect.y = y;
	this->angle = angle;

	
}

void Entity::update(const Uint8 *keyboard_state_array){}

void Entity::update(){}

void Entity::draw(SDL_Renderer* renderer)
{
	animation.draw(renderer, animation.destination_rect.x, animation.destination_rect.y, 
		animation.destination_rect.w, animation.destination_rect.h);
}

bool Entity::operator==(Entity& eSecond)
{
	this->setCollision();
	eSecond.setCollision();
	
	return SDL_HasIntersection(&this->rect_collision, &eSecond.rect_collision);
}

bool Entity::isLife()
{
	if (name == "Explosion" || name == "Explosion_Ship")
		return !animation.animation_finish();
	return life;
}

void Entity::setCollision()
{
	if (name == "Rock")
	{
		rect_collision.x = animation.destination_rect.x + 13;
		rect_collision.y = animation.destination_rect.y + 10;
		rect_collision.w = animation.destination_rect.w - 26;
		rect_collision.h = animation.destination_rect.h - 20;
	}
	else if (name == "Small_Rock")
	{
		rect_collision.x = animation.destination_rect.x + 20;
		rect_collision.y = animation.destination_rect.y + 23;
		rect_collision.w = animation.destination_rect.w - 40;
		rect_collision.h = animation.destination_rect.h - 46;
	}
	else
		rect_collision = this->animation.destination_rect;

}

void Entity::setLife(bool life)
{
	this->life = life;
}

std::string Entity::get_name()
{
	return name;
}

float Entity::Get_x()
{
	return animation.destination_rect.x;
}
float Entity::Get_y()
{
	return animation.destination_rect.y;
}
float Entity::Get_angle()
{
	return angle;
}

Entity::~Entity(){}