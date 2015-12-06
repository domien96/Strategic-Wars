#ifndef GAME_H
#define GAME_H

#include <string>
#include "Engine.h"
#include "Context.h"
#include "RenderSystem.h"
#include "MouseSystem.h"
#include "AnimationSystem.h"
#include "AlSystem.h"

class Game
{
public:
	Game(Context* _context) :
		context(_context),
		size(_context->GetScreenSize()),
		engine(_context),
		fps(_context->GetFps()),
		rs(),
		ms(),
		as(),
		als()
	{};
	~Game();

	void Init();
	void LoadAssets();
	void LoadWorld(std::string world_file);
	
	void Run();

private:
	// Methods
	void CaptureInput();
	void DestroyOther();
	void DestroyEntities();

	// Fields
	World* world;
	double fps;
	Vector2 size;
	Engine engine;
	RenderSystem rs;
	MouseSystem ms;
	AnimationSystem as;
	AlSystem als;
	Context* context;
};

#endif