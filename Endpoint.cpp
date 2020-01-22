#include "Endpoint.h"
#include <iostream>


Endpoint::Endpoint(float x, float y){
	pos = Vector(x, y);
	shape.setPosition(pos.x, pos.y);
	shape.setSize(sf::Vector2f(30.0f, 30.0f));

}


void Endpoint::applyTexture() {
	if (!texture.loadFromFile("sprites/target.png")) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	shape.setTexture(&texture);
}