#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "Component.h"
#include "Graphics.h"
#include "Color.h"

class TextureComponent : public Component
{
public:
	Graphics::Sprite texture;
	Color color;

	TextureComponent(Graphics::Sprite _sprite): texture(_sprite) {};
	TextureComponent(Color _color) : color(_color), texture(Graphics::SPRITE_NONE) {};
	virtual ~TextureComponent() {};
	virtual Tag GetTag() { return TEXTURE; };
};

#endif