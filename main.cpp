#include <SDL.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <list>
#include <utility>
#include <iostream>
#include "Vector2.h"
#include "Bullet.h"
#include "Player.h"

using namespace std;

#undef main

#define DEG_TO_RAD(degrees) M_PI * (degrees) / 180
#define RAD_TO_DEG(radians) (180 * (radians) / M_PI)

constexpr int SCREEN_WIDTH = 1800;
constexpr int SCREEN_HEIGHT = 800;
constexpr int FIELD_WIDTH = 20;
constexpr int FIELD_HEIGHT = 20;
constexpr int MAX_DOTS = 1000;
constexpr int TEXTURE_SIZE = 64;
constexpr Uint32 FPS = 1000 / 60;

int arField[FIELD_HEIGHT][FIELD_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0},
};

int arBigField[800][800] = {};

SDL_Texture* texture = nullptr;

Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
	case 2:
		return *(reinterpret_cast<Uint16*>(p));
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			return p[0] << 16 | p[1] << 8 | p[2];
		}
		else
		{
			return p[0] | p[1] << 8 | p[2] << 16;
		}
	case 4:
		return *(reinterpret_cast<Uint32*>(p));
	default:
		return 0; 
	}
}

void drawField(SDL_Renderer* pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for(int y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(int x = 0; x < FIELD_WIDTH; ++x)
		{
			if(arField[y][x] == 2)
			{
				SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_Rect r = { x * 40, y * 40, 40, 40 };
				SDL_RenderFillRect(pRenderer, &r);
			}
			if(arField[y][x] == 1)
			{
				SDL_SetRenderDrawColor(pRenderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_Rect r = { x * 40, y * 40, 40, 40 };
				SDL_RenderFillRect(pRenderer, &r);
			}
		}
	}
	SDL_Rect r = { 800, 0, 1000, 400 };
	SDL_SetRenderDrawColor(pRenderer, 0, 191, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(pRenderer, &r);
	r.y = 400;
	SDL_SetRenderDrawColor(pRenderer, 0, 191, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(pRenderer, &r);
}

void rayCasting(SDL_Renderer* pRenderer, const Player& player)
{
	for(int x1 = 0; x1 < MAX_DOTS; ++x1)
	{
		double alpha = player.getAngle() - 30 + x1 * 60.0 / MAX_DOTS;
		const Vector2 dXdY = { cos(DEG_TO_RAD(alpha)), sin(DEG_TO_RAD(alpha)) };
		Vector2 rayCoord = player.getCoord();
		bool isWall = false;
		int offsetTextureX = 0;
		int offsetTextureY = 0;
		int offsetXInTexture = 0;
		while (true)
		{
			rayCoord += dXdY;
			if (rayCoord.x < 0 || rayCoord.x > 799 || rayCoord.y < 0 || rayCoord.y > 799)
			{
				rayCoord -= dXdY;
				break;
			}
			if(arBigField[static_cast<int>(rayCoord.y)][static_cast<int>(rayCoord.x)])
			{
				if (arField[static_cast<int>(rayCoord.y / 40)][static_cast<int>(rayCoord.x / 40)] == 1)
				{
					offsetTextureX = 0;
					offsetTextureY = 0;
				}
				else
				{
					offsetTextureX = 128;
					offsetTextureY = 128;
				}
				offsetXInTexture = arBigField[static_cast<int>(rayCoord.y)][static_cast<int>(rayCoord.x)];
				rayCoord -= dXdY;
				isWall = true;
				break;
			}
		}
		SDL_SetRenderDrawColor(pRenderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(pRenderer, player.getCoord().x, player.getCoord().y, rayCoord.x, rayCoord.y);
		if(isWall)
		{
			double distance = Vector2::distance(player.getCoord(), rayCoord);
			distance *= cos(DEG_TO_RAD(player.getAngle() - alpha));
			if(distance > 0)
			{
				int heightWall = 32 * SCREEN_HEIGHT / distance;
				int offsetYInTexture = 0;
				int heightTexture = 64;
				if(heightWall > SCREEN_HEIGHT)
				{
					heightWall = SCREEN_HEIGHT;
				}
				SDL_Rect r1 = { offsetTextureX + offsetXInTexture - 1, offsetTextureY + offsetYInTexture, 1, 64 };
				SDL_Rect r2 = { x1 + 800, (SCREEN_HEIGHT - heightWall) / 2, 1, heightWall };
				SDL_RenderCopy(pRenderer, texture, &r1, &r2);
			}
		}
	}
}

void drawBullets(SDL_Renderer* pRenderer, const std::list<Bullet>& lBullets, const Player& player)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for(auto& bullet : lBullets)
	{
		double distance = Vector2::distance(player.getCoord(), bullet.getCoord());
		Vector2 vecBulNorm((bullet.getCoord() - player.getCoord()) / distance);

		Vector2 vecPlayNorm(cos(DEG_TO_RAD(player.getAngle())), sin(DEG_TO_RAD(player.getAngle())));

		double resVecDot = vecPlayNorm.x * vecBulNorm.x + vecPlayNorm.y * vecBulNorm.y;


		double alpha = RAD_TO_DEG(acos(resVecDot));
		SDL_RenderDrawPoint(pRenderer, bullet.getCoord().x, bullet.getCoord().y);
		SDL_RenderDrawPoint(pRenderer, bullet.getCoord().x - 1, bullet.getCoord().y);
		SDL_RenderDrawPoint(pRenderer, bullet.getCoord().x + 1, bullet.getCoord().y);
		SDL_RenderDrawPoint(pRenderer, bullet.getCoord().x, bullet.getCoord().y - 1);
		SDL_RenderDrawPoint(pRenderer, bullet.getCoord().x, bullet.getCoord().y + 1);
		double normalY = vecBulNorm.x * vecPlayNorm.y - vecBulNorm.y * vecPlayNorm.x;

		if(alpha <= 30.0)
		{
			const double beta = normalY > 0 ? 60 - alpha : 60 + alpha;
			const int halfHeightBullet = 800 / distance;
			const int x1 = beta / 60.0 * MAX_DOTS;
			SDL_Rect r = { x1 + 300 - halfHeightBullet, 400 - halfHeightBullet, halfHeightBullet * 2, halfHeightBullet * 2 };
			SDL_RenderFillRect(pRenderer, &r);
		}
	}
}

void createBigField()
{
	for(int y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(int x = 0; x < FIELD_HEIGHT; ++x)
		{
			if (arField[y][x])
			{
				for (int i = 0; i < 40; ++i)
				{
					arBigField[y * 40 + 39][x * 40 + i] = (i / 40.0) * 64 + 1;
					arBigField[y * 40][x * 40 + 39 - i] = (i / 40.0) * 64 + 1;
					arBigField[y * 40 + 39 - i][x * 40] = (i / 40.0) * 64 + 1;
					arBigField[y * 40 + i][x * 40 + 39] = (i / 40.0) * 64 + 1;
				}
			}
		}
	}
}

int main()
{
	SDL_Window* pWindow = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	if ((pWindow = SDL_CreateWindow("Ray casting",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)) == nullptr)
	{
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	if ((pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE)) == nullptr)
	{
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	
	SDL_Surface* loadedSurface = SDL_LoadBMP("textures.bmp");
	if(loadedSurface == nullptr)
	{
		std::cout << "Unable to load image textures.bmp! SDL Error: " << SDL_GetError() << std::endl;
	}

	texture = SDL_CreateTextureFromSurface(pRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	std::list<Bullet> lBullets;

	createBigField();
	Player player;
	bool bIsRunning = true;
	while (bIsRunning) 
	{
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
			{
				bIsRunning = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					player.setMovingDirection(1);
					break;

				case SDLK_DOWN:
					player.setMovingDirection(-1);
					break;

				case SDLK_LEFT:
					player.setRotationDirection(-1);
					break;

				case SDLK_RIGHT:
					player.setRotationDirection(1);
					break;

				case SDLK_SPACE:
					lBullets.emplace_back(player);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_DOWN:
					player.setMovingDirection(0);
					break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
					player.setRotationDirection(0);
					break;
				}
			}
		}
		player.update();
		for(auto it = lBullets.begin(); it != lBullets.end();)
		{
			it->update();
			if(!it->getExist())
			{
				it = lBullets.erase(it);
			}
			else
			{
				++it;
			}
		}
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(pRenderer);
		drawField(pRenderer);
		rayCasting(pRenderer, player);
		drawBullets(pRenderer, lBullets, player);
		SDL_RenderPresent(pRenderer);
	}
	SDL_FreeSurface(textures);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}
