#ifndef MOUSE_SYSTEM_H
#define MOUSE_SYSTEM_H


#include "System.h"
#include "AllegroLib.h"
#include "Graphics.h"
#include "Entity.h"
#include "Engine.h"
#include "Component.h"
#include "Vector2.h"
#include "UnitComponent.h"
#include "PositionComponent.h"
#include "Pathfinder.h"
#include "World.h"
#include "Pathfinder.h"
#include <iostream>

using namespace std;
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
				case ALLEGRO_KEY_ESCAPE:
					// deselecteer de unit
					//TODO
					break;
				}
			}
		}


		if (al->IsMouseEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();

			if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				Vector2 v = Vector2(ev.mouse.x, ev.mouse.y);
				Grid *mouseMovedGrid = &(Graphics::Instance().ToGrid(v));
			//	cout << mouseMovedGrid->col << " - " << mouseMovedGrid->row << endl;

				World *world = engine->world;
				Entity* blue_selector = new Entity();
				blue_selector->Add(new TextureComponent(Graphics::Sprite::SPRITE_SELECT));
				blue_selector->Add(new PositionComponent(*mouseMovedGrid, 3));
				world->GetWorldEntities(3)->clear();
				world->GetWorldEntities(3)->push_back(blue_selector);

				Graphics::Instance().ExecuteHudDraws(world);

				// TODO Show blue selection square at the correct square.


			}
				

		}

			ALLEGRO_MOUSE_STATE state;
			al_get_mouse_state(&state);

		if (state.buttons && 1) { // linker // KLIK 1
				cout << "Button down" << endl;
				
				Vector2 v = Vector2(state.x, state.y);

				Grid* clickedGrid = &Graphics::Instance().ToGrid(v);
				World* world = engine->world;
				Entity* clickedEntity = world->getWorldEntity(clickedGrid->row, clickedGrid->col, 1);

			if (selectedUnit == (Entity*)0xcccccccc) {
					// Er is nog geen unit geselecteerd. 
					// Als er nu een unit wordt aangeklikt, geven we de statistieken hiervan weer op het scherm.
					// Als we niet op een unit geklikt hebben, geven we lege statistieken weer.
					if (clickedEntity) {
						// TODO:: show stats van deze unit
						// TODO:: teken het select-icoontje 
						Graphics::Instance().DrawBitmap(Graphics::Sprite::SPRITE_SELECT, clickedGrid->row, clickedGrid->col);

						// Als deze unit van de human player is en geen hoofdkwartier, dan wordt die bijgehouden als een veld, zodat
						// bij de volgende klik we de unit kunnen verplaatsen of doen aanvallen.
						UnitComponent* uc = (UnitComponent*)clickedEntity->GetComponent(Component::Tag::UNIT);
						int player = uc == NULL ? 0 : uc->player;
						if (uc != NULL) {
							UnitComponent::UnitType type = uc->type;

							if ((player == 0) && (UnitComponent::UnitType::HQ != type)) {
								selectedUnit = clickedEntity;
								selectedGrid = clickedGrid;
							}
						}
					}
				}
				else {

					// Er is wel een unit geselecteerd. 

					Pathfinder finder;
					Path* p = finder.find_path(*world, *(UnitComponent*) selectedUnit->GetComponent(Component::UNIT),
						*selectedGrid, *clickedGrid);
					finder.reset();

					//TODO: find_path moet opgeroepen worden zelfs als er nog geen bevestigende klik is geweest.

					int path_length = p->cost;
					Grid* steps = p->steps;

					//Zet mag enkel toegelaten worden als path_length <= range vd unit

 
					// Kan je er naartoe verplaatsen?
					// doe dit
					// methode ergens aanroepen of via Path* path = Pathfinder::find_path(world, selectedUnitComponent, selectedGrid, clickedPosition);
					// teken voetjes op alle Grid's die path teruggeeft 
					// Is het een vijand?
					// val aan
					// Is het een ander mannetje van jezelf
					// zie dit als een nieuwe klik en bekijk de vorige if constructie

					// un-select na verplaatsing en aanval
				}
			}
		}
	virtual Type GetType() { return System::TYPE_MOUSE; };

private:
	Entity* selectedUnit;
	Grid* selectedGrid;
};

#endif