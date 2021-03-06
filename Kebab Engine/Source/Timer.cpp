// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - startedAt;
	}
	else
	{
		return stoppedAt - startedAt;
	}
}