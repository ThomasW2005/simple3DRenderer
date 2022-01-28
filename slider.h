#pragma once

#include "functions.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

struct Slider
{
	enum class state
	{
		PASSIVE,
		HOVER,
		SLIDING
	};
	TTF_Font* font;
	state currentState = state::PASSIVE;
	SDL_Rect rect;
	SDL_Rect rectShowing;
	SDL_Renderer* renderer;
	SDL_Color bgPassive = { 255, 255, 255, 255 };
	SDL_Color bgHover = { 255, 255, 255, 255 };
	SDL_Color fgPassive = { 255, 255, 255, 255 };
	SDL_Color fgHover = { 255, 255, 255, 255 };
	SDL_Color fgClick = { 255, 255, 255, 255 };
	SDL_Color textColor = { 255, 255, 255, 255 };
	std::string name;
	float value, vs, ve;
	Slider(int xPos,
		int yPos,
		int height,
		int width,
		float valStart,
		float valEnd,
		SDL_Renderer* sdlRenderer,
		TTF_Font* _font,
		std::string _name = "",
		SDL_Color _textColor = { 0, 0, 0 },
		SDL_Color sdlBgPassive = { 200, 200, 200 },
		SDL_Color sdlBgHover = { 123, 129, 133 },
		SDL_Color sdlFgPassive = { 0, 255, 0 },
		SDL_Color sdlFgHover = { 0, 200, 0 },
		SDL_Color sdlFgClick = { 0, 170, 0 }
	);
	void show();
	void tick(SDL_Event* event);
	void setValue(float value);
	void setPosition(int x, int y);
};

struct SilderManager
{
	std::vector<Slider*> instances;

	void tick(SDL_Event* e);
	void show();
	void add(Slider* s);
};