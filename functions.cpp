#include "functions.h"

std::vector<std::string> getModels(std::string dir)
{
	std::vector<std::string> models;
	for (const auto& entry : std::filesystem::directory_iterator(dir))
	{
		std::string ext = ((std::filesystem::path)entry.path()).string();
		std::string file = ext;
		ext.erase(0, ext.find("."));
		file.erase(0, dir.length() + 1);
		if (ext == ".obj")
			models.push_back(file);
	}
	return models;
}

//std::string strCtoCpp(const char* _string)
//{
//	std::string str(_string);
//	return str;
//}

SDL_Color HSVtoRGB(vec3d HSV)
{
	float h = HSV.x;
	if (h >= 359.9999f)
		h = 0.0f;
	float s = HSV.y;
	float v = HSV.z;
	float r, g, b;
	int i;
	float f, p, q, t;
	if (s == 0) {
		return{ 0, 0, 0 };
	}
	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:
		r = v;
		g = p;
		b = q;
		break;
	}
	return { (unsigned char)(255 * r),(unsigned char)(255 * g),(unsigned char)(255 * b) };
}

void drawFPS(int fps, TTF_Font* font, SDL_Renderer* renderer)
{
	SDL_Color alienGreen = { 0, 255, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, ("fps: " + std::to_string(fps)).c_str(), alienGreen);
	SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect messageRect = { WIDTH - surfaceMessage->w - textHeight / 2, textHeight / 2, surfaceMessage->w, surfaceMessage->h };
	SDL_RenderCopy(renderer, textureMessage, NULL, &messageRect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(textureMessage);
}

void SDL_RenderDrawText(SDL_Renderer* renderer, std::string message, TTF_Font* font, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color alienGreen = { r, g, b };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), alienGreen);
	SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect messageRect = { x, y, surfaceMessage->w, surfaceMessage->h };
	SDL_RenderCopy(renderer, textureMessage, NULL, &messageRect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(textureMessage);
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}