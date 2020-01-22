#pragma once
#include "Key.h"
#include "Vector.h"
class Endpoint : public Key{
public:
	Endpoint(float x, float y);
	Endpoint() = default;
	sf::Texture texture;
	void applyTexture();
	
};

