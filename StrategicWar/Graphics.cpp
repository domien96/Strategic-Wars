#include "Graphics.h"
#include "constants.h"

using namespace std;

Vector2 Graphics::ToPx(Grid gridloc) {
	return Vector2(gridloc.col, gridloc.row);
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
	sprites[SPRITE_NONE] = al_load_bitmap("../assets/images/dev/actor.png");
	sprites[SPRITE_ARCHERY] = al_load_bitmap("../assets/images/dev/archery.png");
	sprites[SPRITE_BADGE_AP] = al_load_bitmap("../assets/images/dev/badge_ap.png");
	sprites[SPRITE_BADGE_ATTACK] = al_load_bitmap("../assets/images/dev/badge_attack.png");
	sprites[SPRITE_BADGE_HP] = al_load_bitmap("../assets/images/dev/badge_hp.png");
	sprites[SPRITE_BADGE_RANGE_MAX] = al_load_bitmap("../assets/images/dev/badge_range_max.png");
	sprites[SPRITE_BADGE_RANGE_MIN] = al_load_bitmap("../assets/images/dev/badge_range_min.png");
	sprites[SPRITE_BRIDGE] = al_load_bitmap("../assets/images/dev/bridge.png");
	sprites[SPRITE_BRIDGE2] = al_load_bitmap("../assets/images/dev/bridge2.png");
	sprites[SPRITE_CROSS_GREY] = al_load_bitmap("../assets/images/dev/cross_grey.png");
	sprites[SPRITE_CROSSHAIR] = al_load_bitmap("../assets/images/dev/crosshair.png");
	sprites[SPRITE_DESTINATION] = al_load_bitmap("../assets/images/dev/destination.png");
	sprites[SPRITE_FIRE] = al_load_bitmap("../assets/images/dev/fire.png");
	sprites[SPRITE_FLAG1] = al_load_bitmap("../assets/images/dev/flag1.png");
	sprites[SPRITE_FLAG2] = al_load_bitmap("../assets/images/dev/flag2.png");
	sprites[SPRITE_HEALTH_HALF] = al_load_bitmap("../assets/images/dev/health0.5.png");
	sprites[SPRITE_HEALTH_ONE] = al_load_bitmap("../assets/images/dev/health1.png");
	sprites[SPRITE_HEALTH_TWO] = al_load_bitmap("../assets/images/dev/health2.png");
	sprites[SPRITE_HEALTH_THREE] = al_load_bitmap("../assets/images/dev/health3.png");
	sprites[SPRITE_HEALTH_FOUR] = al_load_bitmap("../assets/images/dev/health4.png");
	sprites[SPRITE_HEALTH_FIVE] = al_load_bitmap("../assets/images/dev/health5.png");
	sprites[SPRITE_HEALTH_SIX] = al_load_bitmap("../assets/images/dev/health6.png");
	sprites[SPRITE_HEALTH_SEVEN] = al_load_bitmap("../assets/images/dev/health7.png");
	sprites[SPRITE_HEALTH_EIGHT] = al_load_bitmap("../assets/images/dev/health8.png");
	sprites[SPRITE_HEALTH_NINE] = al_load_bitmap("../assets/images/dev/health9.png");
	sprites[SPRITE_HEALTH_TEN] = al_load_bitmap("../assets/images/dev/health10.png");
	sprites[SPRITE_HQ] = al_load_bitmap("../assets/images/dev/hq.png");
	sprites[SPRITE_HUD] = al_load_bitmap("../assets/images/dev/hud.png");
	sprites[SPRITE_ICON] = al_load_bitmap("../assets/images/dev/icon.png");
	sprites[SPRITE_INFANTRY] = al_load_bitmap("../assets/images/dev/infantry.png");
	sprites[SPRITE_LMB] = al_load_bitmap("../assets/images/dev/lmb.png");
	sprites[SPRITE_PATH] = al_load_bitmap("../assets/images/dev/path.png");
	sprites[SPRITE_PATH_FAR] = al_load_bitmap("../assets/images/dev/path_far.png");
	sprites[SPRITE_RMB] = al_load_bitmap("../assets/images/dev/rmb.png");
	sprites[SPRITE_ROAD_0] = al_load_bitmap("../assets/images/dev/road_0.png");
	sprites[SPRITE_ROAD_1] = al_load_bitmap("../assets/images/dev/road_1.png");
	sprites[SPRITE_ROAD_2] = al_load_bitmap("../assets/images/dev/road_2.png");
	sprites[SPRITE_ROAD_3] = al_load_bitmap("../assets/images/dev/road_3.png");
	sprites[SPRITE_ROAD_4] = al_load_bitmap("../assets/images/dev/road_4.png");
	sprites[SPRITE_ROAD_5] = al_load_bitmap("../assets/images/dev/road_5.png");
	sprites[SPRITE_ROAD_6] = al_load_bitmap("../assets/images/dev/road_6.png");
	sprites[SPRITE_ROAD_7] = al_load_bitmap("../assets/images/dev/road_7.png");
	sprites[SPRITE_ROAD_8] = al_load_bitmap("../assets/images/dev/road_8.png");
	sprites[SPRITE_ROAD_9] = al_load_bitmap("../assets/images/dev/road_9.png");
	sprites[SPRITE_ROAD_10] = al_load_bitmap("../assets/images/dev/road_10.png");
	sprites[SPRITE_ROAD_11] = al_load_bitmap("../assets/images/dev/road_11.png");
	sprites[SPRITE_ROAD_12] = al_load_bitmap("../assets/images/dev/road_12.png");
	sprites[SPRITE_ROAD_13] = al_load_bitmap("../assets/images/dev/road_13.png");
	sprites[SPRITE_ROAD_14] = al_load_bitmap("../assets/images/dev/road_14.png");
	sprites[SPRITE_ROAD_15] = al_load_bitmap("../assets/images/dev/road_15.png");
	sprites[SPRITE_ROCK] = al_load_bitmap("../assets/images/dev/rock.png");
	sprites[SPRITE_ROCK_2] = al_load_bitmap("../assets/images/dev/rock_2.png");
	sprites[SPRITE_ROCK_3] = al_load_bitmap("../assets/images/dev/rock_3.png");
	sprites[SPRITE_SELECT] = al_load_bitmap("../assets/images/dev/select.png");
	sprites[SPRITE_SELECT_HIGHLIGHT] = al_load_bitmap("../assets/images/dev/select_highlight.png");
	sprites[SPRITE_SHIELD] = al_load_bitmap("../assets/images/dev/shield.png");
	sprites[SPRITE_TARGET] = al_load_bitmap("../assets/images/dev/target.png");
	sprites[SPRITE_TARGET_BLAST] = al_load_bitmap("../assets/images/dev/target_blast.png");
	sprites[SPRITE_TERRAIN] = al_load_bitmap("../assets/images/dev/terrain.png");
	sprites[SPRITE_TERRAIN_2] = al_load_bitmap("../assets/images/dev/terrain_2.png");
	sprites[SPRITE_UNIT_37] = al_load_bitmap("../assets/images/dev/unit_37.png");
	sprites[SPRITE_UNIT_8] = al_load_bitmap("../assets/images/dev/unit_8.png");
	sprites[SPRITE_WAIT] = al_load_bitmap("../assets/images/dev/wait.png");
	sprites[SPRITE_WATER] = al_load_bitmap("../assets/images/dev/water.png");

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
	// TODO: switch the display buffer to the screen.
	// TODO: clear the buffer
}

void Graphics::ClearScreen()
{
	// TODO: clear the buffer by painting it black;
}

void Graphics::GenerateBackgroundSprite(World * world)
{
	// TODO: Create an appropriately sized bitmap for the SPRITE_WORLD bitmap pointer
	// TODO: Set the target for draw calls to this bitmap
	// TODO: Draw all segments of the background (level)
	// TODO: Reset the target for draw calls to the backbuffer of the display
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