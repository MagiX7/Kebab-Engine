#pragma once

#include <windows.h>
#include <stdio.h>

#define LOG_CONSOLE(format, ...) LogConsole(__FILE__, __LINE__, format, __VA_ARGS__);

void LogConsole(const char file[], int line, const char* format, ...);

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != NULL )			  \
		{							\
	  delete[] x;				\
	  x = NULL;					\
		}							\
							  \
	}

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

// Configuration -----------
//#define SCREEN_WIDTH 1024
//#define SCREEN_HEIGHT 768
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Kebab Engine"