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
					selectedUnit = nullptr;
					selectedGrid = nullptr;
					break;
				}
			}
		}


		if (al->IsMouseEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();

			if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				Vector2 v = Vector2(ev.mouse.x, ev.mouse.y);
				Grid *mouseMovedGrid = &(Graphics::Instance().ToGrid(v));

				World *world = engine->GetContext()->getworld();
				world->GetWorldEntities(3)->clear();

				//  Show blue selection square at the correct square.
				Entity* blue_selector = new Entity();
				blue_selector->Add(new TextureComponent(Graphics::Sprite::SPRITE_SELECT));
				blue_selector->Add(new PositionComponent(*mouseMovedGrid, 3));
				world->GetWorldEntities(3)->push_back(blue_selector);

				// At the same time update the cell info the gui.
		/*		Entity *e = world->getWorldEntity(mouseMovedGrid->row, mouseMovedGrid->col, 1);

				if (e == nullptr)
				{
					e = world->getWorldEntity(mouseMovedGrid->row, mouseMovedGrid->col, 0);
				}

				if (e != nullptr) {
					Entity *bottom_image = new Entity();
					Grid *b = new Grid(12, 0);
					TextureComponent *tc = static_cast<TextureComponent*>(e->GetComponent(Component::TEXTURE));
					bottom_image->Add(tc);
					bottom_image->Add(new PositionComponent(*b, 3));
					world->GetWorldEntities(3)->push_back(bottom_image);
				}*/

				//Draw foots if there is a selectedUnit.
				if (selectedUnit != nullptr) {

					Pathfinder finder;
					UnitComponent *uc = static_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));

					Path* p = finder.find_path(*world, *uc,*selectedGrid, *mouseMovedGrid);

					finder.reset();

					int path_length = p->cost;
					Grid* steps = p->steps;

					for (int i = 0; i < path_length; i++) {
						Grid g = steps[i];
						cout << g.col << " - " << g.row << endl;
					}

					//Zet mag enkel toegelaten worden als path_length <= range vd unit

				}

				Graphics::Instance().ExecuteHudDraws(world,nullptr);

			}


		}

		ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);

		if (state.buttons && 1) { // linker // KLIK 1				
				Vector2 v = Vector2(state.x, state.y);
				Grid* clickedGrid = &Graphics::Instance().ToGrid(v);
				cout << "Button down" << clickedGrid->col << "-" << clickedGrid->row << "!" << endl;

				World* world = engine->GetContext()->getworld();
				Entity* clickedEntity = world->getWorldEntity(clickedGrid->row, clickedGrid->col, 1);
				
				if (clickedEntity != nullptr) {
					UnitComponent *uc = static_cast<UnitComponent*>(clickedEntity->GetComponent(Component::UNIT));
					
					if (uc != nullptr) {

						//  Show yellow selection square at the correct square.
						Entity* yellow_selector = new Entity();
						yellow_selector->Add(new TextureComponent(Graphics::Sprite::SPRITE_SELECT_HIGHLIGHT));
						yellow_selector->Add(new PositionComponent(*clickedGrid, 3));
						world->GetWorldEntities(3)->push_back(yellow_selector);
						
						int player = uc == NULL ? 0 : uc->player;
						if (uc != NULL) {
							UnitComponent::UnitType type = uc->type;

							if ((player == 0) && (UnitComponent::UnitType::HQ != type)) {
								selectedUnit = clickedEntity;
								selectedGrid = clickedGrid;
							}
						}

						Graphics::Instance().ExecuteHudDraws(world, clickedEntity);
					}
				
				}
		}
		else {
			/*
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
	}*/
		}
	}
	virtual Type GetType() { return System::TYPE_MOUSE; };

private:
	Entity* selectedUnit = nullptr;
	Grid* selectedGrid = nullptr;
};

#endif