#include "Key.h"

#include <iostream>


Key::Key(float x, float y){
	pos = Vector(x, y);
	shape.setPosition(x, y);
	std::cout << "Key" << pos.x << " " << pos.y << std::endl;
	shape.setOrigin(length / 2 ,height / 2);//Draw key at center
	shape.setSize(sf::Vector2f(length, height));
	
	
}

bool Key::checkCollision(Laser& l) {
	if (l.pos.x > pos.x && l.pos.y > pos.y && l.pos.x < pos.x + length && l.pos.y < pos.y + length) {
		isGotten = true;
		return(true);
	}
	return(false);
}


void Key::applyTexture() {
	if (!texture.loadFromFile("sprites/keySheet.png")) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	std::cout << "Applying KeyTexture" << std::endl;
	shape.setTextureRect(sf::IntRect(0, 0, frameLength, frameHeight));//Only need to change the first parameter because the width is the third one, so it always stays infront of that first parameter;
	shape.setTexture(&texture);
}

void Key::updateAnimation() {
	if (animationTime * frameLength > length) {
		animationTime = 0;
	}
	else {
		animationTime++;
	}
	shape.setTextureRect(sf::IntRect(animationTime * frameLength, 0, frameLength, frameHeight));
}


void Key::updateSprite() {
	shape.setPosition(pos.x, pos.y);

}
