#pragma once

#include "functions.h"
#include "matrixMath.h"
#include <SDL.h>

void SDL_RenderDrawPointFloat(SDL_Renderer* renderer, float x, float y);
void SDL_RenderDrawLineFloat(SDL_Renderer* renderer, float x, float y, float x1, float y1);
void SDL_RenderDrawCircle(SDL_Renderer* renderer, float x, float y, int radius);
void SDL_RenderFillCircle(SDL_Renderer* renderer, float x, float y, int radius);
void SDL_RenderDrawTriangle(SDL_Renderer* renderer, triangle triang);
void SDL_RenderFillTriangle(SDL_Renderer* renderer, triangle triang);
void SDL_RenderFillTriangleFloat(SDL_Renderer* renderer, triangle triang);