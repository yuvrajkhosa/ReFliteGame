#pragma once
#include "SFML/Graphics.hpp"
#include "Laser.h"
#include "Vector.h"
class Key {
public:
	Key(float x, float y);
	Key() = default;
	sf::RectangleShape shape;
	sf::Texture texture;
	bool checkCollision(Laser& l);
	Vector pos;
	void applyTexture();
};

