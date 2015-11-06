#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>

#include "allegro5\allegro.h"
#include "AllegroLib.h"
#include "Color.h"
#include "Grid.h"

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
		SPRITE_WORLD,
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
	int GetGridSize();

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