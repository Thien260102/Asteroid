#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <iostream>

// this class handle animation for game, get image from source
class Animation
{
private:
	SDL_Surface* image;           // load image from source
	SDL_Texture* texture;         // solve picture and display on screen
	float frame;                  // current frame
	float speed;				  // speed of load new frame
	std::vector<SDL_Rect> frames; // contain all frame
	bool finish;

public:
	SDL_Rect destination_rect;
	Animation();
	Animation(std::string source, int x, int y, int w, int h, SDL_Renderer* renderer, int n, float speed);
	void setting(std::string source, int x, int y, int w, int h, SDL_Renderer* renderer, int n, float speed);
	void update();
	void draw(SDL_Renderer* renderer, int x, int y, int w, int h, float angle, SDL_RendererFlip flip);
	void draw(SDL_Renderer* renderer, int x, int y, int w, int h);
	bool animation_finish();
	~Animation();
};