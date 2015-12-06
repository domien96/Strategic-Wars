#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>

#include "allegro5\allegro.h"
#include "AllegroLib.h"
#include "Color.h"
#include "Grid.h"
#include "Entity.h"

// Forward declaration
class World;

class Graphics
{
public:
	static Graphics& Instance()
	{
		static Graphics instance;
		return instance;
	}

	~Graphics() {};

	enum Sprite {
		SPRITE_NONE,
		SPRITE_ARCHERY,
		SPRITE_BADGE_AP,
		SPRITE_BADGE_ATTACK,
		SPRITE_BADGE_HP,
		SPRITE_BADGE_RANGE_MAX,
		SPRITE_BADGE_RANGE_MIN,
		SPRITE_BRIDGE,
		SPRITE_BRIDGE2,
		SPRITE_CROSS_GREY,
		SPRITE_CROSSHAIR,
		SPRITE_DESTINATION,
		SPRITE_FIRE,
		SPRITE_FLAG1,
		SPRITE_FLAG2,
		SPRITE_HEALTH_HALF,
		SPRITE_HEALTH_ONE,
		SPRITE_HEALTH_TEN,
		SPRITE_HEALTH_TWO,
		SPRITE_HEALTH_THREE,
		SPRITE_HEALTH_FOUR,
		SPRITE_HEALTH_FIVE,
		SPRITE_HEALTH_SIX,
		SPRITE_HEALTH_SEVEN,
		SPRITE_HEALTH_EIGHT,
		SPRITE_HEALTH_NINE,
		SPRITE_HQ,
		SPRITE_HUD,
		SPRITE_ICON,
		SPRITE_INFANTRY,
		SPRITE_LMB,
		SPRITE_PATH,
		SPRITE_PATH_FAR,
		SPRITE_RMB,
		SPRITE_ROAD_0,
		SPRITE_ROAD_1,
		SPRITE_ROAD_10,
		SPRITE_ROAD_11,
		SPRITE_ROAD_12,
		SPRITE_ROAD_13,
		SPRITE_ROAD_14,
		SPRITE_ROAD_15,
		SPRITE_ROAD_2,
		SPRITE_ROAD_3,
		SPRITE_ROAD_4,
		SPRITE_ROAD_5,
		SPRITE_ROAD_6,
		SPRITE_ROAD_7,
		SPRITE_ROAD_8,
		SPRITE_ROAD_9,
		SPRITE_ROCK,
		SPRITE_ROCK_2,
		SPRITE_ROCK_3,
		SPRITE_SELECT,
		SPRITE_SELECT_HIGHLIGHT,
		SPRITE_SHIELD,
		SPRITE_TARGET,
		SPRITE_TARGET_BLAST,
		SPRITE_TERRAIN,
		SPRITE_TERRAIN_2,
		SPRITE_UNIT_37,
		SPRITE_UNIT_8,
		SPRITE_WAIT,
		SPRITE_WATER,
		SPRITE_BACKGROUND,
		SPRITE_LENGTH
	};

	enum Align {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	void LoadFonts();
	void LoadSpriteCache();
	void UnLoadFonts();
	void UnLoadSpriteCache();

	void ExecuteDraws();
	void ClearScreen();

	void GenerateBackgroundSprite(World* world);

	void DrawBitmap(Sprite sprite, float dx, float dy);
	void DrawString(std::string str, float dx, float dy, Color c, Align align, bool hugeFont);
	void DrawRectangle(float dx, float dy, float width, float height, Color c, float thickness);

	Vector2 ToPx(Grid gridloc);
	Grid ToGrid(Vector2 vector);
	int GetGridSize();
	float GetFps();

private:
	bool redraw;
	AllegroLib* al;
	std::vector<ALLEGRO_BITMAP *> sprites;
	ALLEGRO_FONT* font;
	ALLEGRO_FONT* big_font;

	/* Private constructor to make sure no extra instances are made */
	Graphics(): 
		redraw(true), 
		al(&AllegroLib::Instance()) {
	};
	/* Private copy constructor to make sure no extra instances are made */
	Graphics(Graphics const&) {};
	/* Private assignation operator to make sure no extra instances are made */
	void operator=(Graphics const&) {};
	
};

#endif