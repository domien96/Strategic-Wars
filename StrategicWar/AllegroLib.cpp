#include "AllegroLib.h"
#include "Graphics.h"

using namespace std;

void AllegroLib::Init(Vector2 _screensize, float _fps)
{
	// Init variables
	display = NULL;
	timer = NULL;
	event_queue = NULL;
	screensize = _screensize;
	fps = _fps;

	// Init allegro
	if (!al_init()) {
		ShowError("Failed to initialize allegro!");
		exit(-1);
	}

	// Timer
	timer = al_create_timer(1.0 / fps);
	if (!timer) {
		ShowError("Failed to create the timer!");
		exit(-1);
	}

	// Image addon
	if (!al_init_image_addon()) {
		ShowError("Failed to initialize al_init_image_addon!");
		al_destroy_timer(timer);
		exit(-1);
	}

	// Primitives
	if (!al_init_primitives_addon()) {
		ShowError("Failed to initialize al_init_primitives_addon!");
		al_destroy_timer(timer);
		exit(-1);
	}

	// Fonts
	al_init_font_addon();
	al_init_ttf_addon();

	// Keyboard
	if (!al_install_keyboard()) {
		ShowError("Failed to initialize the keyboard!");
		exit(-1);
	}

	// Mouse
	if (!al_install_mouse()) {
		ShowError("Failed to initialize the mouse!");
		exit(-1);
	}

	// Create Display
	display = al_create_display(screensize.x, screensize.y);
	if (!display) {
		ShowError("Failed to initialize display!");
		al_destroy_timer(timer);
		exit(-1);
	}

	// Event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		ShowError("Failed to create event_queue!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		exit(-1);
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

}

void AllegroLib::StartLoop() 
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
}

void AllegroLib::StartIteration() {
	al_wait_for_event(event_queue, &event);
	if (event.type == ALLEGRO_EVENT_TIMER)
		redraw = true;
	else
		redraw = false;
}

void AllegroLib::Destroy() 
{
	// Event queue
	if (event_queue != NULL)
	{
		al_unregister_event_source(event_queue, al_get_keyboard_event_source());
		al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
		al_unregister_event_source(event_queue, al_get_display_event_source(display));
		al_destroy_event_queue(event_queue);
	}

	// Addons
	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();

	// Keyboard
	al_uninstall_keyboard();

	// Display
	al_destroy_display(display);
}

ALLEGRO_EVENT AllegroLib::GetCurrentEvent() 
{
	return event;
}

bool AllegroLib::IsWindowClosed()
{
	return event.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

bool AllegroLib::IsTimerEvent()
{
	return event.type == ALLEGRO_EVENT_TIMER;
}

bool AllegroLib::IsKeyboardEvent()
{
	return event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP;
}

bool AllegroLib::IsMouseEvent()
{
	return event.type == ALLEGRO_EVENT_MOUSE_AXES;
}

bool AllegroLib::IsRedraw()
{
	return redraw && al_is_event_queue_empty(event_queue);
}


void AllegroLib::ShowError(string msg) 
{
	al_show_native_message_box(display, "Error", "Error", msg.c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
}