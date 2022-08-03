#include "Animation.h"

Animation::Animation(){}

Animation::Animation(std::string source, int x, int y, int w, int h, SDL_Renderer* renderer, int n, float speed)
{
	finish = false;
	frame = 0;
	this->speed = speed;

	image = IMG_Load(source.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, image);

	destination_rect.w = w;
	destination_rect.h = h;
	
	for (int i = 0; i < n; i++)
	{
		SDL_Rect temp;
		temp.x = x + w * i;
		temp.y = y;
		temp.w = w;
		temp.h = h;
		frames.push_back(temp);
	}
}

void Animation::setting(std::string source, int x, int y, int w, int h, SDL_Renderer* renderer, int n, float speed)
{
	finish = false;
	frame = 0;
	this->speed = speed;

	image = IMG_Load(source.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, image);

	destination_rect.w = w;
	destination_rect.h = h;

	for (int i = 0; i < n; i++)
	{
		SDL_Rect temp;
		temp.x = x + w * i;
		temp.y = y;
		temp.w = w;
		temp.h = h;
		frames.push_back(temp);
	}
}

void Animation::update()
{
	int n = frames.size();
	frame += speed;
	if (frame >= n)
	{
		finish = true;
		frame -= n;
	}
	if (n == 1)
		frame = 0;
}
void Animation::draw(SDL_Renderer* renderer, int x, int y, int w, int h, float angle, SDL_RendererFlip flip)
{
	update();

	SDL_SetRenderTarget(renderer, this->texture);
	SDL_RenderCopyEx(renderer, this->texture, &frames[frame], &destination_rect, -angle, NULL, flip);
	SDL_SetRenderTarget(renderer, NULL);
	//SDL_RenderCopy(renderer, texture, &frames[frame], &destination_rect);
}

void Animation::draw(SDL_Renderer* renderer, int x, int y, int w, int h)
{
	update();
	destination_rect.x = x;
	destination_rect.y = y;
	destination_rect.w = w;
	destination_rect.h = h;
	SDL_RenderCopy(renderer, texture, &frames[frame], &destination_rect);
}

bool Animation::animation_finish()
{
	return finish;
}

Animation::~Animation()
{
	SDL_DestroyTexture(texture);
}