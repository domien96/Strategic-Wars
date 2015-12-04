#include "Graphics.h"
#include "constants.h"
#include "Entity.h"
#include "World.h"
#include "PositionComponent.h"

using namespace std;

Vector2 Graphics::ToPx(Grid gridloc) {
	return Vector2(gridloc.col*GetGridSize(), gridloc.row*GetGridSize());
}

int Graphics::GetGridSize() {
	return GRID_SIZE;
}

void Graphics::LoadFonts()
{
	font = al_load_ttf_font("../assets/fonts/arialn.ttf", FONT_SIZE_SMALL, 0);
	big_font = al_load_ttf_font("../assets/fonts/arialn.ttf", FONT_SIZE_HUGE, 0);
}

void Graphics::LoadSpriteCache() {
	sprites.resize(SPRITE_LENGTH);
	sprites[SPRITE_NONE] = al_load_bitmap("../assets/images/civ/actor.png");
	sprites[SPRITE_ARCHERY] = al_load_bitmap("../assets/images/civ/archery.png");
	sprites[SPRITE_BADGE_AP] = al_load_bitmap("../assets/images/civ/badge_ap.png");
	sprites[SPRITE_BADGE_ATTACK] = al_load_bitmap("../assets/images/civ/badge_attack.png");
	sprites[SPRITE_BADGE_HP] = al_load_bitmap("../assets/images/civ/badge_hp.png");
	sprites[SPRITE_BADGE_RANGE_MAX] = al_load_bitmap("../assets/images/civ/badge_range_max.png");
	sprites[SPRITE_BADGE_RANGE_MIN] = al_load_bitmap("../assets/images/civ/badge_range_min.png");
	sprites[SPRITE_BRIDGE] = al_load_bitmap("../assets/images/civ/bridge.png");
	sprites[SPRITE_BRIDGE2] = al_load_bitmap("../assets/images/civ/bridge2.png");
	sprites[SPRITE_CROSS_GREY] = al_load_bitmap("../assets/images/civ/cross_grey.png");
	sprites[SPRITE_CROSSHAIR] = al_load_bitmap("../assets/images/civ/crosshair.png");
	sprites[SPRITE_DESTINATION] = al_load_bitmap("../assets/images/civ/destination.png");
	sprites[SPRITE_FIRE] = al_load_bitmap("../assets/images/civ/fire.png");
	sprites[SPRITE_FLAG1] = al_load_bitmap("../assets/images/civ/flag1.png");
	sprites[SPRITE_FLAG2] = al_load_bitmap("../assets/images/civ/flag2.png");
	sprites[SPRITE_HEALTH_HALF] = al_load_bitmap("../assets/images/civ/health0.5.png");
	sprites[SPRITE_HEALTH_ONE] = al_load_bitmap("../assets/images/civ/health1.png");
	sprites[SPRITE_HEALTH_TWO] = al_load_bitmap("../assets/images/civ/health2.png");
	sprites[SPRITE_HEALTH_THREE] = al_load_bitmap("../assets/images/civ/health3.png");
	sprites[SPRITE_HEALTH_FOUR] = al_load_bitmap("../assets/images/civ/health4.png");
	sprites[SPRITE_HEALTH_FIVE] = al_load_bitmap("../assets/images/civ/health5.png");
	sprites[SPRITE_HEALTH_SIX] = al_load_bitmap("../assets/images/civ/health6.png");
	sprites[SPRITE_HEALTH_SEVEN] = al_load_bitmap("../assets/images/civ/health7.png");
	sprites[SPRITE_HEALTH_EIGHT] = al_load_bitmap("../assets/images/civ/health8.png");
	sprites[SPRITE_HEALTH_NINE] = al_load_bitmap("../assets/images/civ/health9.png");
	sprites[SPRITE_HEALTH_TEN] = al_load_bitmap("../assets/images/civ/health10.png");
	sprites[SPRITE_HQ] = al_load_bitmap("../assets/images/civ/hq.png");
	sprites[SPRITE_HUD] = al_load_bitmap("../assets/images/civ/hud.png");
	sprites[SPRITE_ICON] = al_load_bitmap("../assets/images/civ/icon.png");
	sprites[SPRITE_INFANTRY] = al_load_bitmap("../assets/images/civ/infantry.png");
	sprites[SPRITE_LMB] = al_load_bitmap("../assets/images/civ/lmb.png");
	sprites[SPRITE_PATH] = al_load_bitmap("../assets/images/civ/path.png");
	sprites[SPRITE_PATH_FAR] = al_load_bitmap("../assets/images/civ/path_far.png");
	sprites[SPRITE_RMB] = al_load_bitmap("../assets/images/civ/rmb.png");
	sprites[SPRITE_ROAD_0] = al_load_bitmap("../assets/images/civ/road_0.png");
	sprites[SPRITE_ROAD_1] = al_load_bitmap("../assets/images/civ/road_1.png");
	sprites[SPRITE_ROAD_2] = al_load_bitmap("../assets/images/civ/road_2.png");
	sprites[SPRITE_ROAD_3] = al_load_bitmap("../assets/images/civ/road_3.png");
	sprites[SPRITE_ROAD_4] = al_load_bitmap("../assets/images/civ/road_4.png");
	sprites[SPRITE_ROAD_5] = al_load_bitmap("../assets/images/civ/road_5.png");
	sprites[SPRITE_ROAD_6] = al_load_bitmap("../assets/images/civ/road_6.png");
	sprites[SPRITE_ROAD_7] = al_load_bitmap("../assets/images/civ/road_7.png");
	sprites[SPRITE_ROAD_8] = al_load_bitmap("../assets/images/civ/road_8.png");
	sprites[SPRITE_ROAD_9] = al_load_bitmap("../assets/images/civ/road_9.png");
	sprites[SPRITE_ROAD_10] = al_load_bitmap("../assets/images/civ/road_10.png");
	sprites[SPRITE_ROAD_11] = al_load_bitmap("../assets/images/civ/road_11.png");
	sprites[SPRITE_ROAD_12] = al_load_bitmap("../assets/images/civ/road_12.png");
	sprites[SPRITE_ROAD_13] = al_load_bitmap("../assets/images/civ/road_13.png");
	sprites[SPRITE_ROAD_14] = al_load_bitmap("../assets/images/civ/road_14.png");
	sprites[SPRITE_ROAD_15] = al_load_bitmap("../assets/images/civ/road_15.png");
	sprites[SPRITE_ROCK] = al_load_bitmap("../assets/images/civ/rock.png");
	sprites[SPRITE_ROCK_2] = al_load_bitmap("../assets/images/civ/rock_2.png");
	sprites[SPRITE_ROCK_3] = al_load_bitmap("../assets/images/civ/rock_3.png");
	sprites[SPRITE_SELECT] = al_load_bitmap("../assets/images/civ/select.png");
	sprites[SPRITE_SELECT_HIGHLIGHT] = al_load_bitmap("../assets/images/civ/select_highlight.png");
	sprites[SPRITE_SHIELD] = al_load_bitmap("../assets/images/civ/shield.png");
	sprites[SPRITE_TARGET] = al_load_bitmap("../assets/images/civ/target.png");
	sprites[SPRITE_TARGET_BLAST] = al_load_bitmap("../assets/images/civ/target_blast.png");
	sprites[SPRITE_TERRAIN] = al_load_bitmap("../assets/images/civ/terrain.png");
	sprites[SPRITE_TERRAIN_2] = al_load_bitmap("../assets/images/civ/terrain_2.png");
	sprites[SPRITE_UNIT_37] = al_load_bitmap("../assets/images/civ/unit_37.png");
	sprites[SPRITE_UNIT_8] = al_load_bitmap("../assets/images/civ/unit_8.png");
	sprites[SPRITE_WAIT] = al_load_bitmap("../assets/images/civ/wait.png");
	sprites[SPRITE_WATER] = al_load_bitmap("../assets/images/civ/water.png");

	// World: pointer set to NULL so it can be filled in later
	sprites[SPRITE_WORLD] = NULL;
}

void Graphics::UnLoadFonts()
{
	al_destroy_font(font);
	al_destroy_font(big_font);
}

void Graphics::UnLoadSpriteCache() {
	// we gaan ervan uit dat de enum Sprites geen vreemde waarden toegekend krijgt, dus geen SPRITE_WORLD = 100
	// zodat we kunnen itereren
	for (int i = 0; i < SPRITE_LENGTH; i++) {
		al_destroy_bitmap(sprites[i]);
	}
}

/* Small function that converts our own Graphics::Align enum to allegro enums */
int getAlign(Graphics::Align align)
{
	int a;
	switch (align) {
	case Graphics::ALIGN_LEFT:
		a = ALLEGRO_ALIGN_LEFT;
		break;
	case Graphics::ALIGN_CENTER:
		a = ALLEGRO_ALIGN_CENTRE;
		break;
	case Graphics::ALIGN_RIGHT:
		a = ALLEGRO_ALIGN_RIGHT;
		break;
	}
	return a;
}

void Graphics::ExecuteDraws()
{
	
	vector<Entity*> entities = engine->GetEntities();
	for (vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
		TextureComponent *tc = static_cast<TextureComponent*>((*it)->GetComponent(Component::TEXTURE));
		Vector2 v2 = ToPx(pc->pos);
		DrawBitmap(tc->texture, v2.x, v2.y);

	}

	// Reset the target for draw calls to the backbuffer of the display
	al_set_target_bitmap(al_get_backbuffer(al->display));
	al_draw_bitmap(sprites[SPRITE_WORLD], 0, 0, 0);
	al_flip_display();
}

void Graphics::ClearScreen()
{
	// clear the buffer by painting it black
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Graphics::GenerateBackgroundSprite(World * world, Engine* engine)
{
	// Create an appropriately sized bitmap for the SPRITE_WORLD bitmap pointer
	sprites[SPRITE_WORLD] = al_create_bitmap(world->getColumns()*GetGridSize(), world->getRows()*GetGridSize());
	// Set the target for draw calls to this bitmap
	al_set_target_bitmap(sprites[SPRITE_WORLD]);
	// eerst garbage wegdoen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	// TODO: Draw all segments of the background (level)
	this->engine = engine;
	vector<Entity*> v = world->GetWorldEntities();
	for (vector<Entity*>::iterator it = v.begin(); it != v.end(); ++it) {
		PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
		TextureComponent *tc = static_cast<TextureComponent*>((*it)->GetComponent(Component::TEXTURE));
		Vector2 v2 = ToPx(pc->pos);
		DrawBitmap(tc->texture, v2.x, v2.y);
	}
	// Reset the target for draw calls to the backbuffer of the display
	al_set_target_bitmap(al_get_backbuffer(al->display));
	al_draw_bitmap(sprites[SPRITE_WORLD], 0, 0, 0);
	al_flip_display();
}


void Graphics::DrawBitmap(Sprite sprite, float dx, float dy)
{
	al_draw_bitmap(sprites[sprite], dx, dy, 0);

}

void Graphics::DrawString(string str, float dx, float dy, Color c, Align align, bool hugeFont)
{
	char const *text = str.c_str();
	if (!hugeFont) { //geen hugefont
		al_draw_text(font, al_map_rgba(c.r, c.g, c.b, c.a), dx, dy, getAlign(align), text);
	} else { //wel hugefont
		al_draw_text(big_font, al_map_rgba(c.r, c.g, c.b, c.a), dx, dy, getAlign(align), text);
	}
}

void Graphics::DrawRectangle(float dx, float dy, float width, float height, Color c, float thickness)
{
	al_draw_rectangle(dx, dy, dx + width, dy + height, al_map_rgba(c.r, c.g, c.b, c.a), thickness);
}