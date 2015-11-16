#include "Graphics.h"
#include "constants.h"

using namespace std;

Vector2 Graphics::ToPx(Grid gridloc) {
	// TODO
	return Vector2();
}

int Graphics::GetGridSize() {
	// TODO
	return 0;
}

void Graphics::LoadFonts()
{
	// TODO
}

void Graphics::LoadSpriteCache() {
	sprites.resize(SPRITE_LENGTH);
	sprites[SPRITE_NONE] = al_load_bitmap("../assets/images/dev/actor.png");
	// TODO: initialize all sprites as a cache.
	
	// World: pointer set to NULL so it can be filled in later
	sprites[SPRITE_WORLD] = NULL;
}

void Graphics::UnLoadFonts()
{
	// TODO: destroy the loaded fonts
}

void Graphics::UnLoadSpriteCache() {
	// TODO: destroy all the loaded bitmaps
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
	// TODO: draw a bitmap by using the Sprite enum as index.
}

void Graphics::DrawString(string str, float dx, float dy, Color c, Align align, bool hugeFont)
{
	// TODO: draw a string
}

void Graphics::DrawRectangle(float dx, float dy, float width, float height, Color c, float thickness)
{
	//al_map_rgba geeft een ALLEGRO_COLOR terug
	al_draw_rectangle(dx, dy, dx + width, dy + height, al_map_rgba(c.a, c.g, c.b, c.a), thickness);
}