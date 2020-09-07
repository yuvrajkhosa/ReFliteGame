#pragma once
#include <SFML/Graphics.hpp>
#include "Vector.h"
#include "Laser.h"
//#include <ctime>

#include <iostream>
class Surface {
public:
	static float width;
	static float length;
	
	static float amountToAdd;
	Vector* tiltedPoints;
	float pointsArray[2];
	//std::clock_t timer ;
	bool alreadyChecked;
	bool isOppositeSide;
	Vector pos;
	float angle;
	bool rotatable;
	sf::Texture texture;
	sf::RectangleShape plane;
	sf::CircleShape point;
	sf::CircleShape point1;

	Surface() = default;
	Surface(float _x, float _y, float _angle, bool rotate);
	void applyTexture();
	void updatePoints();
	float* getPoints(float iAngle);
	static float toRad(float rad);
	bool checkCollision(Laser& other);//Use REFERNCE of beam otherwise new beam is created IDK why
	bool canRotate();
};

