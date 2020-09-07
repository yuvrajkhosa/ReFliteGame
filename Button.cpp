#include "Button.h"
#include "Vector.h"
#include <string>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"

Button::Button(float _x, float _y, float _width, float _height, std::string _textureAddress) ://sprite is defaulted to true in the declaration
	pos(Vector(_x - width / 2, _y - height / 2)), width(_width), height(_height), spriteLocation(_textureAddress)
{

	rect.setPosition(sf::Vector2f(pos.x, pos.y));
	rect.setSize(sf::Vector2f(width, height));


	
	
}


void Button::applyTexture() {//Which of these two methods are activated will dictate whether button has text

	std::string tempLocation = "sprites/" + spriteLocation + ".png";
	if (!texture.loadFromFile(tempLocation)) {
		std::cout << "Error Key sprite not loaded" << std::endl;
	}
	rect.setTexture(&texture);
	
	
}

void Button::applyTexture(sf::Font* font) {
	text.setFont(*font);//Dereference the font. This way the font is only loaded once and then passed on as a reference. Before I was making the sf::Font font a static variable, but when calling it's loadFromFile method it would try to make a new one in Button. So it dont work cuz
	
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(24);
	text.setPosition(sf::Vector2f(pos.x + 5.0f, pos.y + 5.0f));

	update();

}


bool Button::isClicked(float mouseX, float mouseY) {//Check if mouse is at button when it is clicked
	
	bool click = false;
	if (mouseX > pos.x && mouseX < pos.x + width && mouseY > pos.y && mouseY < pos.y + height) {
		click = true;
	}
	return(click);
}

void Button::update() {
	width = (spriteLocation.length() + 1) * 15;
	rect.setSize(sf::Vector2f(width, height));
	text.setString(spriteLocation);
}