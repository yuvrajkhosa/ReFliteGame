#include "Key.h"

#include <iostream>


Key::Key(float x, float y){
	pos = Vector(x, y);
	shape.setPosition(x, y);
	
	
	shape.setSize(sf::Vector2f(30.0f, 30.0f));
	
	
}

bool Key::checkCollision(Laser& l) {
	if (l.pos.x > pos.x && l.pos.y > pos.y && l.pos.x < pos.x + 30.0f && l.pos.y < pos.y + 30.0f) {
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
