#include "Key.h"

#include <iostream>


Key::Key(float x, float y){
	pos = Vector(x, y);
	shape.setPosition(x, y);
	
	length = 30.0f;
	shape.setSize(sf::Vector2f(length, length));
	
	
}

bool Key::checkCollision(Laser& l) {
	if (l.pos.x > pos.x && l.pos.y > pos.y && l.pos.x < pos.x + length && l.pos.y < pos.y + length) {
		return(true);
	}
	return(false);
}

void Key::applyTexture() {
	if (!texture.loadFromFile("sprites/key.png")) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	shape.setTexture(&texture);
}

void Key::updateSprite() {
	shape.setPosition(pos.x, pos.y);
	std::cout << "update sprite" << std::endl;
}
