#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"

class RenderSystem : public System
{
public:
	RenderSystem() :
		System() {};
	~RenderSystem() {};

protected:
	virtual void Update() {};

	virtual Type GetType() { return System::TYPE_RENDER; };

private:
};

#endif