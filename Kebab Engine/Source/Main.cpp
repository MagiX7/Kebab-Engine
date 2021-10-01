#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL.h"

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
	LOG("Starting '%s'...", TITLE);
	int mainReturn = EXIT_FAILURE;
	MainStates state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			app = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (app->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
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

			LOG("-------------- Application CleanUp --------------");
			if (app->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				mainReturn = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	RELEASE(app);
	LOG("Exiting game '%s'...\n", TITLE);
	return mainReturn;
}