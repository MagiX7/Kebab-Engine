#include "Application.h"

#include "Globals.h"

#include "SDL.h"

#include "Optick/src/optick.h"

#include <stdlib.h>

enum MainStates
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* app = NULL;

int main(int argc, char ** argv)
{
	LOG_CONSOLE("Starting '%s'...", TITLE);
	int mainReturn = EXIT_FAILURE;
	MainStates state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG_CONSOLE("-------------- Application Creation --------------");
			app = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG_CONSOLE("-------------- Application Init --------------");
			if (app->Init() == false)
			{
				LOG_CONSOLE("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG_CONSOLE("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			bool updateReturn = app->Update();

			if (updateReturn == false)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG_CONSOLE("-------------- Application CleanUp --------------");
			if (app->CleanUp() == false)
			{
				LOG_CONSOLE("Application CleanUp exits with ERROR");
			}
			else
				mainReturn = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete(app);
	app = nullptr;
	LOG_CONSOLE("Exiting game '%s'...\n", TITLE);
	return mainReturn;
}