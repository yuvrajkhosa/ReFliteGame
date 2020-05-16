#include "Endpoint.h"
#include <iostream>


Endpoint::Endpoint(float x, float y){
	pos = Vector(x, y);
	shape.setPosition(pos.x, pos.y);
	length = 30.0f;//Although this is parent's variable, we still have to initialize it in this class
	shape.setSize(sf::Vector2f(length, length));

}


void Endpoint::applyTexture() {
	if (!texture.loadFromFile("sprites/target.png")) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	shape.setTexture(&texture);
}
