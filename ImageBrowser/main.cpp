
#include <iostream>
#include "Browser.h"

#include <ctime>
#include "SDL.h"
#include "Timer.h"

using std::string;

using std::cout;
using std::endl;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
	{
		printf("arg%d: %s\n", i, argv[i]);
	}
	
	Browser::GetInstance().Initialize(argv[1]);

	//Quit event
	bool quit = false;

	//Event handler
	SDL_Event e;

	//The frames per second timer
	Timer fpsTimer;

	//The frames per loop timer
	Timer loopTimer;

	//Start counting frames per second
	int countedFrames = 0;
	fpsTimer.Start();
	 
	while (!quit)
	{
		//Start loop timer
		loopTimer.Start();

		while (SDL_PollEvent(&e) != 0)
		{
			//quit app
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else
				if (e.type == SDL_MOUSEBUTTONUP)
				{
					//Get mouse position
					int x, y;
					SDL_GetMouseState(&x, &y);

					Browser::GetInstance().HandleInput(x, y);
				}
		}

		//Calculate fps
		float avgFPS = countedFrames / (fpsTimer.GetTicks() / 1000.f);
		//std::cerr << avgFPS << std::endl;

		//Update browser state & render 
		Browser::GetInstance().Update();

		//Update frame counter
		++countedFrames;

		//If frame finished early
		int frameTicks = loopTimer.GetTicks();
		if (frameTicks < SCREEN_TICK_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		}
	}

	Browser::GetInstance().Terminate();

	return 1;
}