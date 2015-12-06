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
	MouseSystem() : selectedUnit(nullptr), hoveredCell(nullptr) , System() {
	};
	~MouseSystem() {};

	Entity* GetSelectedUnit() { return selectedUnit; }

	enum ActionType {
		MOVE,
		SELECT,
		DESELECT,
		ATTACK,
		NONE
	};

protected:
	virtual void Update() {
		AllegroLib* al = &AllegroLib::Instance();
		World *world = engine->GetContext()->getworld();

		// ACTIONS AANPASSEN
		if (selectedUnit == nullptr) {
				rightbutton = NONE;
			if (hoveredCell && world->isUnit(hoveredCell)) {
				leftbutton = SELECT;
			}
			else {
				leftbutton = NONE;
			}
		}
		else {
			rightbutton = DESELECT;
			if(hoveredCell && world->isUnit(hoveredCell)){
				leftbutton = ATTACK;
			}
			else {
				leftbutton = MOVE;
			}
		}

		// Mark changes in keymap
		if (al->IsKeyboardEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					// deselecteer de unit
					//TODO
					selectedUnit = nullptr;
					break;
				}
			}
		}



		if (al->IsMouseEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();
			Vector2 v = Vector2(ev.mouse.x, ev.mouse.y);

			if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				Grid *mouseMovedGrid = &(Graphics::Instance().ToGrid(v));

				//  Update selection square's position
				if (world->isValidGrid(mouseMovedGrid)) {
					if (hoveredCell) {
						dynamic_cast<PositionComponent*>(hoveredCell->GetComponent(Component::POSITION))->pos = *mouseMovedGrid;
						engine->GetEntityStream()->EntityChanged(hoveredCell);
					}
					else {
						hoveredCell = new Entity();
						hoveredCell->Add(new PositionComponent(*mouseMovedGrid, 3));
						hoveredCell->Add(new TextureComponent(Graphics::SPRITE_SELECT));
						engine->GetEntityStream()->EntityAdded(hoveredCell);
					}
				}
				else {
					engine->GetEntityStream()->EntityRemoved(hoveredCell);
					delete hoveredCell;
					hoveredCell = nullptr;
				}


				// PAD TONEN INDIEN EEN UNIT IS GESELECTEERD

				if (selectedUnit != nullptr) {

					Pathfinder* finder = new Pathfinder();
					UnitComponent *uc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
					Grid selectedGrid = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION))->pos;

					Path* p = finder->find_path(*world, *uc, selectedGrid, *mouseMovedGrid);

					delete finder;

					int path_length = p->cost;
					vector<Grid> steps = p->steps;

					for (int i = 0; i < path_length; i++) {
						Grid g = steps[i];
						cout << g.col << " - " << g.row << endl;
					}
					delete p;
					//Zet mag enkel toegelaten worden als path_length <= range vd unit
				}
			}



			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				ALLEGRO_MOUSE_EVENT mouseEvent = ev.mouse;
				Grid* clickedGrid = &Graphics::Instance().ToGrid(v);
				if (mouseEvent.button == 0) {
					// LEFT BUTTON
					switch (leftbutton) {
					case NONE:
						break;
					case MOVE:
						// TODO
						break;
					case ATTACK:
						// TODO
						break;
					default:
						break;
					}
				}
				else if (mouseEvent.button == 1) {
					// RIGHT BUTTON
					switch (rightbutton) {
					case NONE:
						break;
					case DESELECT:
						// TODO
						break;
					default:
						break;
					}
				}
				//cout << "Button down" << clickedGrid->col << "-" << clickedGrid->row << "!" << endl;

				Entity* clickedEntity = world->getWorldEntity(clickedGrid->row, clickedGrid->col, 1);

				/*if (clickedEntity != nullptr) {
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
								selectedUnit = new Entity(*clickedEntity);
							}
						}

					}

				}*/

			}


		}

		/*ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);

		if (state.buttons && 1) { // linker // KLIK 1				
				Vector2 v = Vector2(state.x, state.y);
				Grid* clickedGrid = &Graphics::Instance().ToGrid(v);
				cout << "Button down" << clickedGrid->col << "-" << clickedGrid->row << "!" << endl;

				World* world = engine->GetContext()->getworld();
				Entity* clickedEntity = world->getWorldEntity(clickedGrid->row, clickedGrid->col, 1);
				
				
				
				}
		}
		else /*if (selectedUnit != nullptr && selectedGrid->col >0)// != nullptr) {
			
			// Er is wel een unit geselecteerd.

			/*cout << "yes" << endl;

			Pathfinder finder;
			Path* p = finder.find_path(*engine->GetContext()->getworld(),
				*(UnitComponent*) selectedUnit->GetComponent(Component::UNIT),
				*selectedGrid, *&Graphics::Instance().ToGrid(Vector2(state.x, state.y)));
			finder.reset();*/
			//delete finder;

			//TODO: find_path moet opgeroepen worden zelfs als er nog geen bevestigende klik is geweest.

			//int path_length = p->cost;
			//Grid* steps = p->steps;

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

	virtual Type GetType() { return System::TYPE_MOUSE; };

private:
	Entity* selectedUnit ;
	Entity* hoveredCell;
	ActionType leftbutton, rightbutton;
};

#endif