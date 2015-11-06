#include "Game.h"
#include "AllegroLib.h"
#include "Graphics.h"
#include "Entity.h"
#include "EntityStream.h"
#include "Vector2.h"
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;

Game::~Game()
{
	DestroyOther();
	DestroyEntities();
}

/* Start the actual gameloop */
void Game::Run()
{
	AllegroLib::Instance().StartLoop();
	bool quit = AllegroLib::Instance().IsWindowClosed();
	while (!quit) {
		// Fetch event
		AllegroLib::Instance().StartIteration();

		// Update all Systems
		engine.Update();

		CaptureInput();

		// Check for window close
		quit = AllegroLib::Instance().IsWindowClosed();
	}
}

/* Inititalise allegro and show the display. */
void Game::Init()
{
	AllegroLib::Instance().Init(size, fps);
}

/* Loads sprites, fonts and adds systems to the Engine */
void Game::LoadAssets()
{
	Graphics::Instance().LoadSpriteCache();
	Graphics::Instance().LoadFonts();

	// TODO: Add all required systems
	engine.AddSystem(&rs);		// Renders all to screen
}

/* Load the level from file */
void Game::LoadWorld(string world_file)
{
	// TODO: Load the level
}

/* Captures input */
void Game::CaptureInput()
{
	AllegroLib* al = &AllegroLib::Instance();
	// Mark changes in keymap
	if (al->IsKeyboardEvent()) {
		ALLEGRO_EVENT ev = al->GetCurrentEvent();
		// TODO: Register which keys were pressed
	}

	if (al->IsMouseEvent()) {
		ALLEGRO_EVENT ev = al->GetCurrentEvent();
		// TODO: Register mouse movements or button clicks
	}
}

void Game::DestroyEntities()
{
	// TODO: Destroy all Entities still attached to the Engine.
}

/* Destroys everything else */
void Game::DestroyOther()
{
	Graphics::Instance().UnLoadFonts();
	Graphics::Instance().UnLoadSpriteCache();

	// Remove systems
	// TODO: remove all added systems
	engine.RemoveSystem(&rs);		

	AllegroLib::Instance().Destroy();
}