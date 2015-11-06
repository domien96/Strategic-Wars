#ifndef CONTEXT_H
#define CONTEXT_H

#include "Vector2.h"
#include <math.h>
#include "constants.h"

class Context
{
public:
	Context(Vector2 _size) :
		size(_size),
		fps(FPS)
	{};
	~Context() {};

	/* Checks whether the debug_mode flag is active */
	bool IsDebugMode() { return debug_mode; };
	
	/* Sets the debug_mode flag */
	void SetDebugMode(bool _debug_mode) { debug_mode = _debug_mode; };

	/* Returns the screen total size in pixels */
	Vector2 GetScreenSize() { return size; };
	
	/* Returns the fps of the game */
	double GetFps() { return fps; };


private:
	Vector2 size;
	bool debug_mode;
	double fps;
};

#endif