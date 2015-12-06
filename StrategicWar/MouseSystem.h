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
#include "pathComponent.h"
#include "Pathfinder.h"
#include "World.h"
#include "Pathfinder.h"
#include <iostream>

using namespace std;
class MouseSystem : public System
{
public:
	MouseSystem() : selectedUnit(nullptr), hoveredCell(nullptr), System() {
	};
	~MouseSystem() {
		delete 	selectedUnit;
		delete hoveredCell;
		delete finder;
	};

	Entity* GetSelectedUnit() { return selectedUnit; }

	enum ActionType {
		MOVE,
		SELECT,
		DESELECT,
		ATTACK,
		NONE
	};

	/* Geeft de mogelijke actie als string van de linkermuisknop */
	string GetLeftButtonActionString()
	{
		switch (leftbutton) {
		case MOVE:
			return "Move";
			break;
		case ATTACK:
			return "Attack";
			break;
		case SELECT:
			return "Select";
			break;
		case NONE:
			return "-";
			break;
		default:
			return "-";
			break;
		}
	}

	/* Geeft de mogelijke actie als string van de rechtermuisknop */
	string GetRightButtonActionString()
	{
		switch (rightbutton) {
		case DESELECT:
			return "Deselect";
			break;
		case NONE:
			return "-";
			break;
		default:
			return "-";
			break;
		}
	}

protected:
	virtual void Update() {
		AllegroLib* al = &AllegroLib::Instance();
		World *world = engine->GetContext()->getworld();

		// ACTIONS AANPASSEN
		// MERK OP: Ook AI UNITS kunnen gesleecteerd worden, maar niet mee gespeeld worden.
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
			leftbutton = NONE;
			if (world->isHuman(selectedUnit)) {
				Grid selectedGrid = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION))->pos; if (hoveredCell) {
					Grid hoveredGrid = dynamic_cast<PositionComponent*>(hoveredCell->GetComponent(Component::POSITION))->pos;
					if (!world->isHuman(hoveredCell) && world->isUnit(hoveredCell)) {
						leftbutton = ATTACK;
					}
					else if (hoveredCell && world->unit_can_walk_over(selectedGrid, hoveredGrid)) {
						leftbutton = MOVE;
					}
				}
			}
		}

		// Mark changes in keymap
		if (al->IsKeyboardEvent()) {
			ALLEGRO_EVENT ev = al->GetCurrentEvent();
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_ESCAPE:
					// deselecteer de unit
					engine->GetEntityStream()->EntityRemoved(selectedUnit);
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
				if (hoveredCell == nullptr || !(*mouseMovedGrid == dynamic_cast<PositionComponent*>(hoveredCell->GetComponent(Component::POSITION))->pos)) {
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

					if (selectedUnit != nullptr && world->isValidGrid(mouseMovedGrid)) {
						// EERST VORIGE PATH VERWIJDEREN
						set<Entity*> path = engine->GetEntityStream()->WithTag(Component::PATH);
						for (set<Entity*>::iterator it = path.begin(); it != path.end(); it++) {
							engine->GetEntityStream()->EntityRemoved(*it);
							delete (*it);
						}

						// NIEUW PAD MAKEN
						finder = new Pathfinder();
						UnitComponent *uc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
						Grid selectedGrid = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION))->pos;

						Path* p = finder->find_path(*world, *uc, selectedGrid, *mouseMovedGrid);

						delete finder;
						if (p != nullptr) {
							int path_length = p->cost;
							vector<Grid> steps = p->steps;
							int max_cost = uc->ap;
							int length_move = min(path_length, max_cost);

							for (int i = 0; i < length_move; i++) {
								Grid g = steps[i];
								Entity* green_step = new Entity();
								green_step->Add(new TextureComponent(Graphics::Sprite::SPRITE_PATH));
								green_step->Add(new PositionComponent(g, 3));
								green_step->Add(new PathComponent());
								engine->AddEntity(green_step);
							}
							for (int j = length_move + 1; j < path_length; j++) {
								Grid h = steps[j];
								Entity* yellow_step = new Entity();
								yellow_step->Add(new TextureComponent(Graphics::Sprite::SPRITE_PATH_FAR));
								yellow_step->Add(new PositionComponent(h, 3));
								yellow_step->Add(new PathComponent());
								engine->AddEntity(yellow_step);
							}
							delete p;
							//Zet mag enkel toegelaten worden als path_length <= range vd unit
						}
					}
				}
			} else 
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				ALLEGRO_MOUSE_EVENT mouseEvent = ev.mouse;
				Grid* clickedGrid = &Graphics::Instance().ToGrid(v);
				if (mouseEvent.button == 1) {
					// LEFT BUTTON
					switch (leftbutton) {
					case NONE:
						break;
					case SELECT:
						selectedUnit = world->getWorldEntity(clickedGrid->row, clickedGrid->col, 1);
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
				else if (mouseEvent.button == 2) {
					// RIGHT BUTTON
					switch (rightbutton) {
					case NONE:
						break;
					case DESELECT:
						// deselecteer de unit
						engine->GetEntityStream()->EntityRemoved(selectedUnit);
						selectedUnit = nullptr;
						break;
					default:
						break;
					}
				}
				

			}

			/*
			//  Show yellow selection square at the correct square.
			Entity* yellow_selector = new Entity();
			yellow_selector->Add(new TextureComponent(Graphics::Sprite::SPRITE_SELECT_HIGHLIGHT));
			yellow_selector->Add(new PositionComponent(*clickedGrid, 3));
			engine->AddEntity(yellow_selector);
			*/

		}

	}

	virtual Type GetType() { return System::TYPE_MOUSE; };

private:
	Entity* selectedUnit;
	Entity* hoveredCell;
	Pathfinder* finder;
	ActionType leftbutton, rightbutton;
};

#endif