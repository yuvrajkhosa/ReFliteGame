#pragma once
#include "Vector.h"

#include <SFML/Graphics.hpp>

class Laser {
private:
	
public:
	float xBuffer = 0.0f;
	float yBuffer = 0.0f;
	float angle = 0.0f;
	float radius = 0.0f;
	float initX = 0.0f;
	float initY = 0.0f;
	Vector pos;
	sf::CircleShape beam;
	static int laserSpeed;
	Laser() = default;
	Laser(float x, float y);

	void setAngle(int a, float _xBuffer, float _yBuffer);
	void move();
};

