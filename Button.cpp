#include "Button.h"
#include "Vector.h"
#include <string>
#include <iostream>
Button::Button(float _x, float _y, float _width, float _height, std::string _textureAddress) :
	pos(Vector(_x, _y)), width(_width), height(_height), spriteLocation(_textureAddress)
{
	pos = Vector(_x - _width / 2, _y - _height / 2);//Make origin center
	width = _width;
	height = _height;
	spriteLocation = _textureAddress;
	rect.setPosition(sf::Vector2f(pos.x, pos.y));
	rect.setSize(sf::Vector2f(width, height));
}

void Button::applyTexture() {
	std::string tempLocation = "sprites/" + spriteLocation + ".png";
	if (!texture.loadFromFile(tempLocation)) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	
	rect.setTexture(&texture);
}

bool Button::isClicked(float mouseX, float mouseY) {//Check if mouse is at button when it is clicked
	
	bool click = false;
	if (mouseX > pos.x && mouseX < pos.x + width && mouseY > pos.y && mouseY < pos.y + height) {
		click = true;
	}
	return(click);
}