#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Graphics.h"
#include "AllegroLib.h"

class RenderSystem : public System
{
public:
	RenderSystem() :
		System() {};
	~RenderSystem() {};

protected:
	virtual void Update() {
		if(AllegroLib::Instance().IsRedraw()) {
			Graphics::Instance().ClearScreen();
			updateLevel();
			updateStats();
			Graphics::Instance().ExecuteDraws();
		}
	};

	virtual void updateLevel() {

	};

	virtual void updateStats() {

	};

	virtual Type GetType() { return System::TYPE_RENDER; };

private:
};

#endif