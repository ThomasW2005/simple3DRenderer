#include "Slider.h"

void Slider::setPosition(int x, int y)
{
	rectShowing.x = rect.x = x;
	rectShowing.y = rect.y = y;

}

void Slider::setValue(float valueSet)
{
	value = valueSet;
	rectShowing.w = (value - vs) * rect.w / (ve - vs);
}

void Slider::tick(SDL_Event* event)
{
	int xPos, yPos;
	SDL_GetMouseState(&xPos, &yPos);
	static bool isDown;
	if (event->type == SDL_MOUSEBUTTONDOWN)
		isDown = true;
	if (event->type == SDL_MOUSEBUTTONUP)
		isDown = false;

	if (xPos > rect.x - 1 && xPos < (rect.x + rect.w) + 1 && yPos > rect.y && yPos < (rect.y + rect.h))
	{
		currentState = state::HOVER;
		if (isDown)
		{
			currentState = state::SLIDING;
			value = ((xPos - rect.x) / (float)rect.w * (ve - vs)) + vs;
			this->setValue(value);
		}
	}
	else
		currentState = state::PASSIVE;
}

void Slider::show()
{
	SDL_Color bg_color, fg_color;
	switch (currentState)
	{
	case state::PASSIVE:
		bg_color = bgPassive;
		fg_color = fgPassive;
		break;
	case state::HOVER:
		bg_color = bgHover;
		fg_color = fgHover;
		break;
	case state::SLIDING:
		bg_color = bgHover;
		fg_color = fgClick;
		break;
	}
	SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, fg_color.r, fg_color.g, fg_color.b, fg_color.a);
	SDL_RenderFillRect(renderer, &rectShowing);
	if (!name.empty())
		SDL_RenderDrawText(renderer, name + std::to_string(value), font, rect.x, rect.y - 2, textColor.r, textColor.g, textColor.b);
}

Slider::Slider(int xPos, int yPos, int height, int width, float valStart, float valEnd, SDL_Renderer* sdlRenderer, TTF_Font* _font, std::string _name, SDL_Color _textColor, SDL_Color sdlBgPassive, SDL_Color sdlBgHover, SDL_Color sdlFgPassive, SDL_Color sdlFgHover, SDL_Color sdlFgClick)
{
	rect.h = height;
	rect.w = width;
	rect.x = xPos;
	rect.y = yPos;
	rectShowing.x = xPos;
	rectShowing.y = yPos;
	rectShowing.h = height;
	rectShowing.w = 0;
	vs = valStart;
	ve = valEnd;
	renderer = sdlRenderer;
	value = vs;

	bgPassive = sdlBgPassive;
	bgHover = sdlBgHover;
	fgPassive = sdlFgPassive;
	fgHover = sdlFgHover;
	fgClick = sdlFgClick;
	name = _name;
	textColor = _textColor;
	font = _font;
}

void SilderManager::tick(SDL_Event* e)
{
	for (const auto& x : instances)
		x->tick(e);
}

void SilderManager::show()
{
	for (const auto& x : instances)
		x->show();
}

void SilderManager::add(Slider* s)
{
	instances.push_back(s);
}
