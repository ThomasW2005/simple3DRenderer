#include "slider.h"
#include "globals.h"
#include "functions.h"
#include "primitives.h"
#include "matrixMath.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char* argv[])
{
	if (argc > 1)
		strcpy_s(MODEL, argv[1]);
	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	std::string currentModel(MODEL);
	std::string drawModeName[3] = {
		"Filled & Shaded",
		"Wireframe",
		"Wirebox" };
	bool quit = false, fullScreen = false, rotX = false, rotY = false, rotZ = false, renderVertices = false, drawUI = true, randomizeColors = false, animateHue = true;
	float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f, fov = 90.0f, hue = 0.0f, moveSpeed = 2.0f, rotationSpeed = 0.25f, measuredFPS = 0, dt = 0.0f;
	int drawMode = 0, modelSelector = 0;
	vec3d rotatePoint(0.0f, 0.0f, 3.0f);
	SDL_Window* window = SDL_CreateWindow("Simple 3D .obj Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);
	// SDL_RENDERER_PRESENTVSYNC
	SDL_DisplayMode beforeMode;
	SDL_Event event;
	mesh modelMesh;
	std::vector<std::string> models = getModels("assets"); //get all .obj file in the assets directory and have them onhand for loading
	TTF_Font* consola = TTF_OpenFont("assets/cour.ttf", textHeight);
	TTF_Font* consolaFat = TTF_OpenFont("assets/cour.ttf", textHeight * 3);
	if (!consola)
	{
		consola = TTF_OpenFont("C:/Windows/Fonts/cour.ttf", textHeight);
		consolaFat = TTF_OpenFont("C:/Windows/Fonts/cour.ttf", textHeight * 3);
		if (!consola)
			SDL_ShowSimpleMessageBox(0, "Missing Fonts", "Looks like there are some fonts missing (cour.tff)", window);
	}

	if (quit = !modelMesh.LoadOBJ(currentModel))
		SDL_ShowSimpleMessageBox(0, "Error", "Error while loading model", window);

	modelMesh.move(0.0f, 0.0f, rotatePoint.z);
	modelMesh.color = HSVtoRGB({ hue, 1.0f, 1.0f });

	SilderManager manager;
	Slider XSlider(40, 40, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, consola, "X Angle: "); manager.add(&XSlider);
	Slider YSlider(40, 40, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, consola, "Y Angle: "); manager.add(&YSlider);
	Slider ZSlider(40, 40, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, consola, "Z Angle: "); manager.add(&ZSlider);
	Slider fovSlider(40, 40, textHeight - 4, 300, 60.0f, 120.0f, renderer, consola, "FOV: "); manager.add(&fovSlider);
	Slider HueSlider(40, 40, textHeight - 4, 300, 0.0f, 360, renderer, consola, "Hue: "); manager.add(&HueSlider);
	Slider brightnessSlider(40, 40, textHeight - 4, 300, 0.0f, 1.0f, renderer, consola, "Brightness: "); manager.add(&brightnessSlider);
	HueSlider.setValue(245.0f);
	brightnessSlider.setValue(1.0f);
	while (!quit)
	{
		Uint64 start = SDL_GetPerformanceCounter(); //timestamp for deltaTime & FPS

		while (SDL_PollEvent(&event)) //eventhandling
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				manager.tick(&event);
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					WIDTH = event.window.data1;
					HEIGHT = event.window.data2;
					AR = static_cast<float>(HEIGHT) / WIDTH;
					break;
				}
				break;
			case SDL_KEYDOWN:
				float speed = moveSpeed * dt;
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_w:
					modelMesh.move(0, 0, speed);
					rotatePoint.z += speed;
					break;
				case SDLK_s:
					modelMesh.move(0, 0, -speed);
					rotatePoint.z -= speed;
					break;
				case SDLK_a:
					modelMesh.move(-speed, 0, 0);
					rotatePoint.x -= speed;
					break;
				case SDLK_d:
					modelMesh.move(speed, 0, 0);
					rotatePoint.x += speed;
					break;
				case SDLK_UP:
					modelMesh.move(0, -speed, 0);
					rotatePoint.y -= speed;
					break;
				case SDLK_DOWN:
					modelMesh.move(0, speed, 0);
					rotatePoint.y += speed;
					break;
				case SDLK_LEFT:
					fovSlider.setValue(fovSlider.value -= 1.0f);
					break;
				case SDLK_RIGHT:
					fovSlider.setValue(fovSlider.value += 1.0f);
					break;
				case SDLK_x:
					rotX = !rotX;
					break;
				case SDLK_y:
					rotY = !rotY;
					break;
				case SDLK_z:
					rotZ = !rotZ;
					break;
				case SDLK_g:
					drawMode = ++drawMode % 3;
					break;
				case SDLK_f:
					renderVertices = !renderVertices;
					break;
				case SDLK_u:
					drawUI = !drawUI;
					break;
				case SDLK_c:
					randomizeColors = !randomizeColors;
				case SDLK_h:
					animateHue = !animateHue;
					break;
				case SDLK_j:
					if (!models.empty())
					{
						modelSelector = ++modelSelector % models.size();
						currentModel = "assets/" + models[modelSelector];
						if (quit = !modelMesh.LoadOBJ(currentModel))
							SDL_ShowSimpleMessageBox(0, "Error", "Error while loading model", window);
						modelMesh.move(rotatePoint.x, rotatePoint.y, rotatePoint.z);
					}
					break;
				case SDLK_r:
					if (quit = !modelMesh.LoadOBJ(currentModel))
						SDL_ShowSimpleMessageBox(0, "Error", "Error while loading model", window);
					rotatePoint.x = 0;
					rotatePoint.y = 0;
					rotatePoint.z = 3;
					modelMesh.move(0, 0, rotatePoint.z);
					models = getModels("assets");
					break;
				case SDLK_F11:
					fullScreen = !fullScreen;
					if (fullScreen)
					{
						SDL_DisplayMode mode;
						SDL_GetWindowDisplayMode(window, &beforeMode);
						SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);
						SDL_SetWindowSize(window, mode.w, mode.h);
						SDL_RenderPresent(renderer);
						SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
					}
					else
					{
						SDL_SetWindowFullscreen(window, 0);
						SDL_RenderPresent(renderer);
						SDL_SetWindowSize(window, beforeMode.w, beforeMode.h);
					}
					break;
				}
				break;
			}
		}

		if (rotX)
			angleX += rotationSpeed * dt;
		else
			angleX = XSlider.value;
		if (rotY)
			angleY += 2 * rotationSpeed * dt;
		else
			angleY = YSlider.value;
		if (rotZ)
			angleZ += 3 * rotationSpeed * dt;
		else
			angleZ = ZSlider.value;
		if (animateHue)
			HueSlider.setValue(std::fmod(hue + 20.0f * dt, 360.0f));

		hue = HueSlider.value;
		fov = fovSlider.value;
		modelMesh.color = HSVtoRGB({ hue, 1.0f, 1.0f });

		//rendering:
		//first, all triangles are rotated and projected, then accumulated in trianglesToRasterize.
		//After that, the triangles get sorted, so that the triangles which are nearest to the camera, will be drawn last, basic culling

		std::vector<triangle> trianglesToRasterize;

		for (const auto& i : modelMesh.tri)
		{
			triangle triRotated, triProjected;
			for (int x = 0; x < 3; x++) //rotation
			{
				vec3d j = i.p[x];
				j -= rotatePoint; //to rotate around any 3d point, offset, then rotate and offset back
				j = rotateX(j, angleX);
				j = rotateY(j, angleY);
				j = rotateZ(j, angleZ);
				j += rotatePoint;
				triRotated.p[x] = j;
			}
			vec3d normal = triRotated.normalVector();
			float brightness = 1.0f;
			if (drawMode == 0)
			{
				vec3d singleDirectionLight(0.0f, 0.0f, -1.0f); //a simple light, all rays coming from the same direction (not a point, but a plane)
				singleDirectionLight.normalize();
				normal.normalize();
				brightness = dotProduct(singleDirectionLight, normal); //We assume that the more a triangles normal vector aligns with the direction light, the more lit it will be
				brightness = std::clamp(brightness, 0.0f, 1.0f);
				brightness *= brightnessSlider.value;
			}																 //NOT FINISHED!!!
																			 //   |
			if (dotProduct(normal, triRotated.p[0]) < 0.0f || drawMode == 1) //we can only see triangles if their normal vector has a negative z component (irrelevant if wireframe mode is selected)
			{
				triProjected.color = { (unsigned char)(modelMesh.color.r * brightness), (unsigned char)(modelMesh.color.g * brightness), (unsigned char)(modelMesh.color.b * brightness), 255 };
				for (int j = 0; j < 3; j++)
					triProjected.p[j] = project(triRotated.p[j], fov);
				trianglesToRasterize.push_back(triProjected);
			}
		}

		std::sort(trianglesToRasterize.begin(), trianglesToRasterize.end(), [](triangle& t1, triangle& t2) // triangles get sorted using std::sort from <algorithm> (quicksort), lambda function to indiciate if two values should be swapped
			{
				float mid1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float mid2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return mid1 > mid2;
			});

		for (auto& triProjected : trianglesToRasterize) // trinagles get drawn according to selected mod
		{
			if (randomizeColors)
				triProjected.color = HSVtoRGB({ (float)(std::rand() % 360), 1.0f, 1.0f });
			SDL_SetRenderDrawColor(renderer, triProjected.color.r, triProjected.color.g, triProjected.color.b, triProjected.color.a);
			switch (drawMode)
			{
			case 0: //Normal, Filled and Shaded mode
				SDL_RenderFillTriangleFloat(renderer, triProjected);
				break;
			case 1: //Wireframe
				SDL_RenderDrawTriangle(renderer, triProjected);
				break;
			case 2: // "Wirebox", like wireframe, but you can't see through the model
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillTriangleFloat(renderer, triProjected);
				SDL_SetRenderDrawColor(renderer, triProjected.color.r, triProjected.color.g, triProjected.color.b, triProjected.color.a);
				SDL_RenderDrawTriangle(renderer, triProjected);
				break;
			}
			if (renderVertices) //optionally renders each vertex (not optimized yet)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				for (const auto& k : triProjected.p)
					SDL_RenderDrawCircle(renderer, map(k.x, -1.0f, 1.0f, 0, WIDTH), map(k.y, -1.0f, 1.0f, 0, HEIGHT), 4);
			}
		}

		if (drawUI)
		{
			manager.show();
			std::vector<std::string> textLeft = {
				"Simple 3D Rasterizer by Thomas Weichhart",
				" ",
				"X angle: " + std::to_string(angleX * (180 / M_PI)) + "10\xB0",
				"Y angle: " + std::to_string(angleY * (180 / M_PI)) + "10\xB0",
				"Z angle: " + std::to_string(angleZ * (180 / M_PI)) + "10\xB0",
				"camera FOV: " + std::to_string(fov),
				"vertices: " + std::to_string(modelMesh.tri.size()),
				"rotation point/center of cube:",
				"    x: " + std::to_string(rotatePoint.x),
				"    y: " + std::to_string(rotatePoint.y),
				"    z: " + std::to_string(rotatePoint.z),
				"current drawing mode: " + drawModeName[drawMode],
				"model name: " + currentModel };
			std::vector<std::string> textRight = {
				"Controls:",
				"W to move model back",
				"S to move model forward",
				"A to move model left",
				"D to move model right",
				"Up Arrow to move model up",
				"Down Arrow to move cube down",
				"Left/Right Arrow to change FOV",
				"F11 to toggle fullscreen",
				"g to cycle drawing modes",
				"f to toggle Vertices",
				"r to reload model",
				"u to toggle UI",
				"c to randomize colors",
				"j to cycle models",
				"h to animate hue",
				"X/Y/Z to toggle between auto rotation and manual rotation using sliders for X/Y/Z axis" };

			for (int i = 0; i < textLeft.size(); i++)
				SDL_RenderDrawText(renderer, textLeft[i], consola, textHeight / 2, textHeight / 2 + i * textHeight, 0, 255, 0);

			XSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 0) * textHeight + 2);
			YSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 1) * textHeight + 2);
			ZSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 2) * textHeight + 2);
			fovSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 3) * textHeight + 2);
			HueSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 4) * textHeight + 2);
			brightnessSlider.setPosition(textHeight / 2, textHeight / 2 + (textLeft.size() + 5) * textHeight + 2);

			for (int i = 0; i < textRight.size(); i++)
				SDL_RenderDrawText(renderer, textRight[i], consola, textHeight / 2, i * textHeight - textHeight / 2 + (HEIGHT - textHeight * textRight.size()), 0, 255, 0);
		}
		drawFPS(measuredFPS, consolaFat, renderer);

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		dt = ((SDL_GetPerformanceCounter() - start) / (float)SDL_GetPerformanceFrequency());
		measuredFPS = 1.0f / dt;
	}
	TTF_CloseFont(consola);
	TTF_CloseFont(consolaFat);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}