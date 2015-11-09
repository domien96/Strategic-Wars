#ifndef SYSTEM_H
#define SYSTEM_H

#include "Context.h"

// Forward declaration
class Engine;

class System
{
public:
	enum Type {
		TYPE_RENDER
	};

	System() {};
	virtual ~System() {};

	virtual void Update() = 0;
	virtual Type GetType() = 0;

	void SetEngine(Engine* _engine) { engine = _engine; };

protected:
	Engine* engine;
};

#endif