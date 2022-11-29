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
	float length = 128.0f;
	float height = 128.0f;
	int frameLength = 64;
	int frameHeight = 64;
	int animationTime = 0;
	bool checkCollision(Laser& l);
	bool isGotten = false;
	Vector pos;
	void applyTexture();
	void updateSprite();
	void updateAnimation();
};

