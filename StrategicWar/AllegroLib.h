#ifndef ALLEGRO_LIB_H
#define ALLEGRO_LIB_H

#include <string>
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_primitives.h"
#include "allegro5\allegro_native_dialog.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include "Vector2.h"

class Graphics;

class AllegroLib
{
	friend class Graphics;
public:
	/* Returns the only instance of this AllegroLib */
	static AllegroLib& Instance()
	{
		static AllegroLib instance;
		return instance;
	}

	~AllegroLib() {};

	enum Align {
		LEFT,
		CENTER,
		RIGHT
	};

	/* Initializes allegro and its systems, also creates a display of _screensize and inits the timer at _fps frames per second */
	void Init(Vector2 _screensize, float _fps);

	/* This function must be called before the starting of the gameloop, to start the timer with as little delay as possible */
	void StartLoop();

	/* this function must be called at the start of every iteration of the gameloop, to regulate waiting the correct amount of time for the events */
	void StartIteration();

	/* Destroys allegro by cleaning up where needed */
	void Destroy();

	/* Checks whether the window has been closed by pressing the X on the top right */
	bool IsWindowClosed();

	/* Checks whether the current event is a TIMER event */
	bool IsTimerEvent();

	/* Checks whether the current event is a KEYBOARD event */
	bool IsKeyboardEvent();

	/* Checks whether the current event is a MOUSE event */
	bool IsMouseEvent();

	/* Checks whether it is appropriate to redraw the frame now (is the event queue also empty?)*/
	bool IsRedraw();

	/* Returns the current event as an ALLEGRO_EVENT for further processing*/
	ALLEGRO_EVENT GetCurrentEvent();

private:
	/* Private constructor to make sure no extra instances are made */
	AllegroLib() {};
	/* Private copy constructor to make sure no extra instances are made */
	AllegroLib(AllegroLib const&) {};
	/* Private assignation operator to make sure no extra instances are made */
	void operator=(AllegroLib const&) {};

	/* Easily show a native error dialog. */
	void ShowError(std::string msg);

	// Fields
	ALLEGRO_DISPLAY* display;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_EVENT event;
	
	Vector2 screensize;
	float fps;
	bool redraw = true;
};

#endif