#include "Surface.h"
#include "Laser.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Trail.h"

extern int windowWidth;
extern int windowHeight;
Surface::Surface(float _x, float _y, float _angle, bool rotate)
	: pos(Vector(_x, _y)), angle(_angle), rotatable(rotate) {
	plane.setSize(sf::Vector2f(length, width));
	plane.setFillColor(sf::Color::White);
	plane.setOrigin(length / 2, width / 2);
	plane.setFillColor(rotatable ? sf::Color(66, 245, 236) : sf::Color(255, 255, 255, 150));//If rotatable make color to white, otherwise make color to aqua white
	updatePoints();
	point1.setRadius(6.0f);
	point1.setOrigin(0.0f, 0.0f);
	point1.setFillColor(sf::Color::Red);
	point1.setPosition(_x - (point1.getRadius()), _y - point1.getRadius());//I may remove everything to do with points, only there for debugging
	alreadyChecked = false;
	plane.setPosition(pos.x, pos.y);
}


void Surface::updatePoints() {
	//amountToadd = atan(width / length);//Only convert the angle to radians. amountToAdd is already in radians.
	//point.setPosition(pos.x - (cos(toRad(angle) + amountToAdd) * sqrt((width * width) + (length * length))) / 2 ,pos.y - (sin(toRad(angle) + amountToadd) * sqrt((width * width) + (length * length))) / 2);
	tiltedPoints = new Vector[4] { Vector(pos.x - getPoints(toRad(angle))[0] / 2, pos.y - getPoints(toRad(angle))[1] / 2), Vector(pos.x + getPoints(toRad(angle) - amountToAdd * 2)[0] / 2, pos.y + getPoints(toRad(angle) - amountToAdd * 2)[1] / 2), Vector(pos.x + getPoints(toRad(angle))[0] / 2, pos.y + getPoints(toRad(angle))[1] / 2), Vector(pos.x - getPoints(toRad(angle) - amountToAdd * 2)[0] / 2, pos.y - getPoints(toRad(angle) - amountToAdd * 2)[1] / 2)};
	plane.setRotation(angle + 90.0f);
	//point1.setPosition(tiltedPoints[0].x, tiltedPoints[0].y);

}

float* Surface::getPoints(float angle) {
	angle -= 1.5708f;//This is 90 degrees in radians
	float arr[2] = { (float)(cos(angle + amountToAdd) * sqrt((width * width) + (length * length))), (float)(sin(angle + amountToAdd) * sqrt((width * width) + (length * length))) };
	return(arr);
}

float Surface::toRad(float deg) {
	return(deg * (3.141592f / 180.0f));
}

void Surface::checkCollision(Laser& other) {//IMPLEMENT TIMER SYSTEM WHERE ANOTHER COLLISION CAN NOT BE REGISTERD FOR MAYBE 0.1 SECONDS TO REMOVE CONSTANT SET ANGLES.| Passing the other thing as a reference to the actual laser beam.
	
	/*
	Usefull links
	https://editor.p5js.org/yuvrajkhosa/sketches/pPG1OX-ma
	https://ricktu288.github.io/ray-optics/simulator/
	http://www.jeffreythompson.org/collision-detection/table_of_contents.php
	https://github.com/yuvrajkhosa/Yuvraj-Repo-All/blob/master/RayReflectionThesis.txt
	*/

	Vector* linePos;//Make pointer so we can initialize Vector Array later.
	Vector lineVec;
	Vector otherPos = Vector(other.pos.x, other.pos.y);
	if (clock() - timer < 45) {//If 45ms has not passed, do not run. This is to prevent extra detection on same collision | Perfect balance between detecting ALL bounces but not extra collision
		//std::cout << timer << std::endl;
		return;
	}
	
	alreadyChecked = false;
	

	for (int i = 0; i < 2; i++) {
		if (i % 2 == 0) {
			linePos = new Vector[2]{ Vector(tiltedPoints[0].x, tiltedPoints[0].y), Vector(tiltedPoints[1].x, tiltedPoints[1].y) };
			lineVec = Vector::sub(Vector(tiltedPoints[1].x, tiltedPoints[1].y), Vector(tiltedPoints[0].x, tiltedPoints[0].y));
			isOppositeSide = false;
		} 
		else {
			linePos = new Vector[2]{ Vector(tiltedPoints[2].x, tiltedPoints[2].y), Vector(tiltedPoints[3].x, tiltedPoints[3].y) };
			lineVec = Vector::sub(linePos[1], linePos[0]);
			isOppositeSide = true;
		}
		
		
		
		Vector lx = lineVec;
		lineVec = Vector::normalize(lineVec);

		Vector mouseVec = Vector::sub(otherPos, linePos[0]);

		lineVec = Vector::mult(lineVec, Vector::dot(mouseVec, lineVec));
		Vector newVec = Vector::add(linePos[0], lineVec);
		newVec = Vector::sub(otherPos, newVec);
		if (Vector::getMag(newVec) <= 1.5f) {
			Vector d1 = Vector::sub(otherPos, linePos[1]);
			Vector d2 = Vector::sub(otherPos, linePos[0]);
			if ((unsigned __int64)Vector::getMag(d1) + (unsigned __int64)Vector::getMag(d2) >= (unsigned __int64)Vector::getMag(lx) - (unsigned __int64) 1 && (unsigned __int64)Vector::getMag(d1) + (unsigned __int64)Vector::getMag(d2) <= (unsigned __int64)Vector::getMag(lx) + (unsigned __int64) 1) {
				float reflectionAngle = (180.0f - other.angle) + (angle * 2);
				//Adjustments made for coordinate system. Right now, 0 is up 90 is right. Plane is tilted to the angle CLOCKWISE. Beam is GOING TOWARDS ANGLE. Ex. 90 degrees beam is to the RIGHT.
																		//I had a stupid bug where I was using setAngle to set the variable 'newAngle' to the new angle. But then in Surface::CheckCollision I was using the laser's 'angle' variable to get its angle when that would never change. It would be 'newAngle' taht was changing. So I just got rid of 'newAngle'
				
				printf("Beam angle: %0.2f this angle: %0.2f\n", other.angle, angle);
				//std::cout << "Reflection Angle: " << reflectionAngle << std::endl;
				
				//std::cout << "Then 360 + " << reflectionAngle << " = " << 360 + reflectionAngle << " - 180 = " << 360 + reflectionAngle - 180 << std::endl;
				other.setAngle(360.0f + reflectionAngle - 180.f, other.pos.x, other.pos.y);//FINALLY THE RIGHT FORMULA. GOD DAMN.

				timer = clock();
				alreadyChecked = true;
				Trail::newVertex(other.pos.x, other.pos.y);
				return;
			}

		}

	}
	//if (beam.pos.x < pos.x - width) {
	//	printf("Beam x: %0.2f. Surface x: %0.2f\n", beam.pos.x,  (float)pos.x - width);
	//	 so that the angle is not set twice because the ball has not left collision point. 
	//	
	//	//std::cout << beam.angle << std::endl;
	//}
}

bool Surface::canRotate() {
	return(rotatable ? true : false);
}



