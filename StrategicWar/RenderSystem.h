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
			Graphics::Instance().ExecuteDraws();
		}
	};

	virtual Type GetType() { return System::TYPE_RENDER; };

private:
};

#endif