
#include <iostream>
#include "Application.hpp"
#include <SDL2/SDL_ttf.h>

int main()
{
	TTF_Init();
	Application app = Application();
	app.run();
	TTF_Quit();
	SDL_Quit();
	return (0);
}
