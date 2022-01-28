#pragma once

#include "matrixMath.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <filesystem>

std::vector<std::string> getModels(std::string dir);
void SDL_RenderDrawText(SDL_Renderer* renderer, std::string message, TTF_Font* font, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void drawFPS(int fps, TTF_Font* font, SDL_Renderer* renderer);
SDL_Color HSVtoRGB(vec3d HSV);
/// <summary>
/// Converts a C-style String to a std::string
/// </summary>
//std::string strCtoCpp(const char* _string);
float map(float x, float in_min, float in_max, float out_min, float out_max);
