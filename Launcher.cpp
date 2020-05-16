#include "Launcher.h"
#include <iostream> 
#include "Vector.h"
#include <cmath>
#include "Surface.h"
using std::cout;
using std::endl;
extern int windowWidth;
extern int windowHeight;
Launcher::Launcher(float x, float y) {
	pos.x = x;
	pos.y = y;

	launcherShape.setSize(sf::Vector2f((float)launcherLength, (float)launcherLength));//500
	launcherShape.setPosition(x, y);
	
	barrel.setSize(sf::Vector2f(100.0f, 20.0f));
	barrel.setOrigin(100.0f, 10.0f);
	barrel.setPosition(x + 40.0f, y + 40.0f);
	barrel.setFillColor(sf::Color(255, 133, 133, 255));
	shapeArr[0] = barrel;
	shapeArr[1] = launcherShape;
}

void Launcher::setAngle(float mouseX, float mouseY) {
	float x = mouseX - pos.x - (launcherLength / 2);//Adjusting to make launcher the origin. Current origin at bottom left
	float y = windowHeight - mouseY - (windowHeight - pos.y) + launcherLength / 2;//
	Vector vec(x, y);
	if (vec.y < 0) vec.y = 0;
	
	//vec.print();
	//cout << asin(vec.x / Vector::getMag(vec)) * 180.0f / 3.141515f << endl;
	angle = asin(vec.x / Vector::getMag(vec)) * 180.0f / 3.141515f;
	//angle = atan(vec.y / vec.x) * 180.0f / 3.1415f;
	//cout << angle << endl;
	shapeArr[0].setRotation(angle + 90.0f);


}

void Launcher::applyTexture() {
	if (!texture.loadFromFile("sprites/emitter.png")) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	shapeArr[1].setTexture(&texture);
	
}

void Launcher::updateSprite() {
	shapeArr[0].setPosition(pos.x + launcherLength / 2, pos.y + launcherLength / 2);
	shapeArr[1].setPosition(pos.x, pos.y);
}
