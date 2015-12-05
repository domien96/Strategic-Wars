#include "Game.h"
#include "AllegroLib.h"
#include "Graphics.h"
#include "Entity.h"
#include "EntityStream.h"
#include "Vector2.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "World.h"


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
	rs.SetEngine(&engine);
	ms.SetEngine(&engine);
	// TODO: Add all required systems
	engine.AddSystem(&rs);		// Renders all to screen
}

/* Load the level from file */
void Game::LoadWorld(string world_file)
{
	// Load the level
	World world(world_file);
	engine.SetWorld(&world);
	Graphics::Instance().GenerateBackgroundSprite(&world);
}

/* Captures input */
void Game::CaptureInput()
{
	AllegroLib* al = &AllegroLib::Instance();
	// Mark changes in keymap
	if (al->IsKeyboardEvent()) {
		ALLEGRO_EVENT ev = al->GetCurrentEvent();
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
				//TODO opzoeken op welke keys er allemaal gereageerd moet worden
				case ALLEGRO_KEY_ESCAPE:
					break;
			}
		}
	}

	if (al->IsMouseEvent()) {
		ALLEGRO_EVENT ev = al->GetCurrentEvent();
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			switch (ev.mouse.button) {
				// buttons worden genummerd vanaf 1
				//TODO
				case 1:
					break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) { //muis bewogen
			//TODO
		}
	}
}

void Game::DestroyEntities()
{
	std::vector<Entity*> v = engine.GetEntities();
	// remove all entities
	std::vector<Entity*>::iterator it;
	for (it = v.begin(); it < v.end(); ++it) {
		engine.RemoveEntity(*it);
	}
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