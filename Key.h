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
	float length;
	bool checkCollision(Laser& l);
	bool isGotten = false;
	Vector pos;
	void applyTexture();
	void updateSprite();
};

