#include "AllegroLib.h"
#include "Context.h"
#include "Game.h"

int main(int argc, char** argv)
{
	// Debug flag
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Context context(Vector2(750, 600));
	context.SetDebugMode(true);
	Game game(&context);
	game.Init();
	game.LoadAssets();
	game.LoadWorld("../assets/levels/basic.world");
	game.Run();

	return 0;
}