#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Graphics.h"
#include "AllegroLib.h"
#include "PositionComponent.h"
#include "TextureComponent.h"
#include "MouseSystem.h"
#include "Tags.h"

class RenderSystem : public System
{
public:
	RenderSystem() :
		System() {};
	~RenderSystem() {};

protected:
	virtual void Update() {
		if (AllegroLib::Instance().IsRedraw()) {
			Graphics::Instance().ClearScreen();
			// world
			loadBackground();
			updateLevel();
			updateStats();
			// stream
			drawStreamEntities();
			Graphics::Instance().ExecuteDraws();
		}
	};
	void loadBackground() {
		Graphics::Instance().DrawBitmap(Graphics::SPRITE_BACKGROUND, 0, 0);
	}

	void updateLevel() {
		World* world = engine->GetContext()->getworld();
		Graphics& drawer = Graphics::Instance();

		for (int depth = 1; depth <= MAX_CELL_DEPTH; ++depth) {
			vector<Entity*> v = *(world->GetWorldEntities(depth));
			for (vector<Entity*>::iterator it = v.begin(); it != v.end(); ++it) {
				if ((*it) != nullptr) {
					PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
					TextureComponent *tc = static_cast<TextureComponent*>((*it)->GetComponent(Component::TEXTURE));
					Vector2 v2 = drawer.ToPx(pc->pos);
					drawer.DrawBitmap(tc->texture, v2.x, v2.y);
				}

			}
		}
	}

	void updateStats() {
		Graphics& drawer = Graphics::Instance();
		World* world = engine->GetContext()->getworld();
		Entity* entity = dynamic_cast<MouseSystem*>(engine->GetSystem(System::TYPE_MOUSE))->GetSelectedUnit();

		string HP = "N/A", ATT = "N/A", MIN_RANGE = "N/A", MAX_RANGE = "N/A", AP = "N/A";
		if (entity != nullptr) {
			UnitComponent *uc = dynamic_cast<UnitComponent*>(entity->GetComponent(Component::UNIT));
			if (uc != nullptr) {
				HP = to_string(uc->hp);
				ATT = to_string(uc->hp);
				MIN_RANGE = to_string(uc->dp);
				MAX_RANGE = to_string(uc->range_min);
				AP = to_string(uc->range_max);

				//drawer.DrawString(uc->type, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE)
			}
			TextureComponent *tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));
			if (tc != nullptr) {
				drawer.DrawBitmap(tc->texture, 0, world->getRows()*GRID_SIZE + 1);
			}
		}

		drawer.DrawString(to_string((int)floor(engine->GetContext()->GetFps() + 0.5)) + " fps", 5, world->getRows()*GRID_SIZE + 2 + 6 * (GRID_SIZE + 2), Color(1, 1, 0, 1), Graphics::ALIGN_LEFT, false);

		drawer.DrawString(HP, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE + 2 + (GRID_SIZE + 2), Color(1, 1, 1, 1), Graphics::ALIGN_LEFT, false);
		drawer.DrawString(ATT, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE + 2 + 2 * (GRID_SIZE + 2), Color(1, 1, 1, 1), Graphics::ALIGN_LEFT, false);
		drawer.DrawString(MIN_RANGE, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE + 2 + 3 * (GRID_SIZE + 2), Color(1, 1, 1, 1), Graphics::ALIGN_LEFT, false);
		drawer.DrawString(MAX_RANGE, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE + 2 + 4 * (GRID_SIZE + 2), Color(1, 1, 1, 1), Graphics::ALIGN_LEFT, false);
		drawer.DrawString(AP, 2 * (GRID_SIZE + 2), world->getRows()*GRID_SIZE + 2 + 5 * (GRID_SIZE + 2), Color(1, 1, 1, 1), Graphics::ALIGN_LEFT, false);
	};

	void drawStreamEntities() {
		set<Component::Tag> tags = Tags(Component::TEXTURE).And(Component::POSITION).List();
		set<Entity*> ents = engine->GetEntityStream()->WithTags(tags);
		Graphics& drawer = Graphics::Instance();

		for (set<Entity*>::iterator it = ents.begin(); it != ents.end(); ++it) {
			if ((*it) != nullptr) { // Double-check: should not be false normally.
				PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
				TextureComponent *tc = static_cast<TextureComponent*>((*it)->GetComponent(Component::TEXTURE));
				Vector2 v2 = drawer.ToPx(pc->pos);
				drawer.DrawBitmap(tc->texture, v2.x, v2.y);
			}
		}
	}

	virtual Type GetType() { return System::TYPE_RENDER; };

private:
};

#endif