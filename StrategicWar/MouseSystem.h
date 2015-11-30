#ifndef MOUSE_SYSTEM_H
#define MOUSE_SYSTEM_H

#include "System.h"
#include "AllegroLib.h"
#include "Graphics.h"

class MouseSystem : public System
{
public:
	MouseSystem() :
		System() {};
	~MouseSystem() {};

protected:
	virtual void Update() {
		AllegroLib* al = &AllegroLib::Instance();
		// Mark changes in keymap
		if (al->IsKeyboardEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (ev.keyboard.keycode) {
					//TODO opzoeken op welke keys er allemaal gereageerd moet worden
				case ALLEGRO_KEY_ESCAPE:
					break;
				}
			}
		}

		if (al->IsMouseEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				switch (ev.mouse.button) {
					// buttons worden genummerd vanaf 1
					//TODO
				case 1:
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) { 
				//TODO
				//Graphics::DrawBitmap(Graphics::Sprite::SPRITE_SELECT, ev.mouse.dx, ev.mouse.dy);
			}
		}
	};

	virtual Type GetType() { return System::TYPE_MOUSE; };

private:
};

#endif