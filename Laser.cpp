#include "Laser.h"
#include "Trail.h"
#include "Surface.h"
#include <cmath>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>

extern int windowWidth;
extern int windowHeight;

//Laser::Laser(float _a, float _r)
//	: angle(_a), radius(_r){
//	beam.setRadius(5);
//	setAngle(_a, (float)windowWidth / 2.0f, (float)windowHeight / 2.0f);
//	beam.setOrigin(2.5, 2.5);
//}

Laser::Laser(float x, float y){

	pos.x = x;
	pos.y = y;
	xBuffer = x;
	xBuffer = y;
	beam.setRadius(5);
	//setAngle(360.0f, (float)pos.x, (float)pos.y);
	beam.setOrigin(2.5, 2.5);
}

void Laser::setAngle(float a, float _xBuffer, float _yBuffer){
	
	xBuffer = _xBuffer;
	yBuffer = _yBuffer;
	radius = 2.0f; //Reset the length of the "triangle" since we have a new center point
	//angle = ((a) * 3.14159f / 180.0f) ;//Convert to radians since WE are workin in degrees. Subtract 90.0f so we are aligned to Game Development compass system. 90 to the right
	angle = a;
	//Trail::newVertex(beam.getPosition().x, beam.getPosition().y);
	//std::cout << " | New angle of beam set to: " << a + 180 << std::endl << std::endl;// subtract 140 so it matches how I think about it
}

void Laser::move() {
	//pos = Vector((sin(angle) * radius) + (windowWidth / 2), (cos(angle) * radius) + (windowHeight / 2)); //Laser::Laser(float _x, float _y) : pos(_x, _y) {} This is better
	radius += laserSpeed;

	//Here getting the angle from setAngle() and converting it into radians and also adding 90 degrees to account for MY coordinate system. Where 90 degrees is left.
	pos.x = (cos(angle * (3.14159f / 180.0f) + 1.5708f) * radius) + (xBuffer);//Convert from cartesian to polar coordinates !!! THE MATH FUNCTIONS GIVE US A RESULT BACK IN RADIANS !!!
	pos.y = (sin(angle * (3.14159f / 180.0f) + 1.5708f) * radius) + (yBuffer);
	if (pos.x < 0 || pos.x > windowWidth || pos.y < 0 || pos.y > windowHeight) {
		//std::cout << "THE LASER IS OUT OF SCREEN" << std::endl;
	}
	beam.setPosition(pos.x, pos.y);
}
