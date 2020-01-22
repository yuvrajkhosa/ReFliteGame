#pragma once
#include "SFML/Graphics.hpp"
#include "Vector.h"
class Launcher {
public:
	
	sf::RectangleShape launcherShape;
	sf::RectangleShape barrel;
	sf::RectangleShape shapeArr[2];
	sf::Texture texture;

	int launcherLength = 80;
	Launcher() = default;
	Launcher(float x, float y);
	Vector pos;
	void setAngle(float mouseX, float mouseY);
	void applyTexture();
	float angle = 0.0f;
};