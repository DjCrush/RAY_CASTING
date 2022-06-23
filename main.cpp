#include <SDL.h>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

#undef main

#define DEG_TO_RAD(degrees) M_PI * (degrees) / 180

constexpr int SCREEN_WIDTH = 1800;
constexpr int SCREEN_HEIGHT = 800;
constexpr int FIELD_WIDTH = 20;
constexpr int FIELD_HEIGHT = 20;

int arField[FIELD_HEIGHT][FIELD_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
};

int arBigField[800][800] = {};

void drawField(SDL_Renderer* pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for(int y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(int x = 0; x < FIELD_WIDTH; ++x)
		{
			if(arField[y][x])
			{
				SDL_Rect r = { x * 40, y * 40, 40, 40 };
				SDL_RenderFillRect(pRenderer, &r);
			}
		}
	}
}

struct Player
{
	double x = 180.0;
	double y = 200.0;
	int angle = 250;
	int rotationDirection = 0;
	bool movingForward = false;
	bool movingBackwards = false;

	void update()
	{
		const double dx = cos(DEG_TO_RAD(angle));
		const double dy = sin(DEG_TO_RAD(angle));
		if(movingForward)
		{
			x += dx;
			y += dy;
			if(arBigField[static_cast<int>(y)][static_cast<int>(x)] || x < 0 || x > 799)
			{
				x -= dx;
				y -= dy;
			}
		}
		else if (movingBackwards)
		{
			x -= dx;
			y -= dy;
			if (arBigField[static_cast<int>(y)][static_cast<int>(x)] || y < 0 || y > 799)
			{
				x += dx;
				y += dy;
			}
		}
		angle += rotationDirection;
		if (angle == -1)
		{
			angle += 359;
		}
		else if (angle == 360)
		{
			angle = 0;
		}
	}
};

void rayCasting(SDL_Renderer* pRenderer, const Player& player)
{
	double x1 = 0;
	for(int alpha = player.angle - 45; alpha <= player.angle + 45; ++ alpha)
	{
		const double dx = cos(DEG_TO_RAD(alpha)) * 2;
		const double dy = sin(DEG_TO_RAD(alpha)) * 2;
		double x = player.x;
		double y = player.y;
		bool isWall = false;
		while (true)
		{
			x += dx;
			y += dy;
			if (x < 0 || x > 799 || y < 0 || y > 799)
			{
				x -= dx;
				y -= dy;
				break;
			}
			if(arBigField[static_cast<int>(y)][static_cast<int>(x)])
			{
				x -= dx;
				y -= dy;
				isWall = true;
				break;
			}
		}
		SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(pRenderer, player.x, player.y, x, y);
		if(isWall)
		{
			double distance = sqrt((player.x - x) * (player.x - x) + (player.y - y) * (player.y - y));
			distance *= cos(DEG_TO_RAD(alpha - player.angle));
			if(distance > 0)
			{
				int halfHeightWall = 16 * SCREEN_HEIGHT / distance;
				SDL_SetRenderDrawColor(pRenderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(pRenderer, x1 + 800, SCREEN_HEIGHT / 2 - halfHeightWall, 
					x1 + 800, SCREEN_HEIGHT / 2 + halfHeightWall);
			}
		}
		x1 += (SCREEN_WIDTH - 800) / 90.0;
	}
}

void createBigField()
{
	for(int y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(int x = 0; x < FIELD_HEIGHT; ++x)
		{
			for(int y1 = 0; y1 < 40; ++y1)
			{
				for(int x1 = 0; x1 < 40; ++x1)
				{
					arBigField[y * 40 + y1][x * 40 + x1] = arField[y][x];
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
		return 1;
	if((pWindow = SDL_CreateWindow("Ray casting",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)) == nullptr)
		return 1;

	if((pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED)) == nullptr)
		return 1;
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
					player.movingForward = true;
					player.movingBackwards = false;
					break;

				case SDLK_DOWN:
					player.movingForward = false;
					player.movingBackwards = true;
					break;

				case SDLK_LEFT:
					player.rotationDirection = -1;
					break;

				case SDLK_RIGHT:
					player.rotationDirection = 1;
					break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					player.movingForward = false;
					break;
				case SDLK_DOWN:
					player.movingBackwards = false;
					break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
					player.rotationDirection = 0;
					break;
				}
			}
		}

		player.update();
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(pRenderer);
		drawField(pRenderer);
		rayCasting(pRenderer, player);
		SDL_RenderPresent(pRenderer);
	}

	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}
