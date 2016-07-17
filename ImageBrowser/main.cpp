
#include <ctime>

#include "SDL.h"
#include "Timer.h"
#include "Browser.h"
#include "log.h"

const int32_t SCREEN_FPS = 60;
const int32_t SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

int main(int32_t argc, char** argv)
{
	FILELog::ReportingLevel() = FILELog::FromString(argv[2] ? argv[2] : "DEBUG1");
	for (int32_t i = 0; i < argc; ++i)
	{
		FILE_LOG(logINFO) << "arg" << i << ": " << argv[i];
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
	int32_t countedFrames = 0;
	fpsTimer.Start();
	 
	while (!quit)
	{
		//Start loop timer
		loopTimer.Start();

		while (SDL_PollEvent(&e) != 0)
		{
			//quit app
			switch (e.type)
			{
				case SDL_QUIT:
				{
					quit = true;
				}
				break;

				case SDL_MOUSEBUTTONUP:
				{
					//Get mouse position
					int32_t x, y;
					SDL_GetMouseState(&x, &y);

					Browser::GetInstance().HandleInput(x, y,InputType::MOUSE_CLICK);
				}
				break;

				case SDL_MOUSEMOTION:
				{
					//Get mouse position
					int32_t x, y;
					SDL_GetMouseState(&x, &y);

					Browser::GetInstance().HandleInput(x, y, InputType::MOUSE_OVER);

					//cout << "Mouse x = " << x << " y = " << y << endl;
				}
				break;
				default:
					break;
			}
		}

		//Calculate fps
		//float avgFPS = countedFrames / (fpsTimer.GetTicks() / 1000.f);
		//std::cerr << avgFPS << std::endl;

		//Update browser state & render 
		Browser::GetInstance().Update();

		//Update frame counter
		++countedFrames;

		//If frame finished early
		int32_t frameTicks = loopTimer.GetTicks();
		if (frameTicks < SCREEN_TICK_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		}
	}

	Browser::GetInstance().Terminate();

	return 1;
}