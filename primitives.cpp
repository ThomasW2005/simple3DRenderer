#include "primitives.h"

void SDL_RenderFillTriangleFloat(SDL_Renderer* renderer, triangle triang)
{
	for (auto& i : triang.p)
	{
		i.x = map(i.x, -1.0f, 1.0f, 0, WIDTH);
		i.y = map(i.y, -1.0f, 1.0f, 0, HEIGHT);
	}
	SDL_RenderFillTriangle(renderer, triang);
}

void SDL_RenderFillTriangle(SDL_Renderer* renderer, triangle triang)
{
	//this function currently leaks memory ;( fix it (i dont think it does anymore but i dont wanna test it rn)

	Sint16 vx[3];
	Sint16 vy[3];

	vx[0] = triang.p[0].x;
	vx[1] = triang.p[1].x;
	vx[2] = triang.p[2].x;
	vy[0] = triang.p[0].y;
	vy[1] = triang.p[1].y;
	vy[2] = triang.p[2].y;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int gfxPrimitivesPolyInts[3];

	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < 3); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		}
		else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}

	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < 3); i++) {
			if (!i) {
				ind1 = 2;
				ind2 = 0;
			}
			else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else {
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2))) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), [](const void* a, const void* b) {return (*(const int*)a) - (*(const int*)b); });

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			SDL_RenderDrawLine(renderer, xa, y, xb, y);
		}
	}
}

void SDL_RenderDrawTriangle(SDL_Renderer* renderer, triangle triang)
{
	for (int i = 0; i < 3; i++)
		SDL_RenderDrawLineFloat(renderer, triang.p[i].x, triang.p[i].y, triang.p[(i + 1) % 3].x, triang.p[(i + 1) % 3].y);
}

void SDL_RenderDrawPointFloat(SDL_Renderer* renderer, float x, float y)
{
	SDL_RenderDrawPointF(renderer, map(x, -1.0f, 1.0f, 0, WIDTH), map(y, -1.0f, 1.0f, 0, HEIGHT));
}

void SDL_RenderDrawLineFloat(SDL_Renderer* renderer, float x, float y, float x1, float y1)
{
	SDL_RenderDrawLineF(renderer, map(x, -1.0f, 1.0f, 0, WIDTH), map(y, -1.0f, 1.0f, 0, HEIGHT), map(x1, -1.0f, 1.0f, 0, WIDTH), map(y1, -1.0f, 1.0f, 0, HEIGHT));
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, float x, float y, int radius)
{
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}

void SDL_RenderDrawCircle(SDL_Renderer* renderer, float x, float y, int radius)
{
	const int diameter = (radius * 2);

	float xpos = (radius - 1);
	float ypos = 0;
	float tx = 1;
	float ty = 1;
	float error = (tx - diameter);

	while (xpos >= ypos)
	{
		SDL_RenderDrawPoint(renderer, x + xpos, y - ypos);
		SDL_RenderDrawPoint(renderer, x + xpos, y + ypos);
		SDL_RenderDrawPoint(renderer, x - xpos, y - ypos);
		SDL_RenderDrawPoint(renderer, x - xpos, y + ypos);
		SDL_RenderDrawPoint(renderer, x + ypos, y - xpos);
		SDL_RenderDrawPoint(renderer, x + ypos, y + xpos);
		SDL_RenderDrawPoint(renderer, x - ypos, y - xpos);
		SDL_RenderDrawPoint(renderer, x - ypos, y + xpos);

		if (error <= 0)
		{
			++ypos;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--xpos;
			tx += 2;
			error += (tx - diameter);
		}
	}
}