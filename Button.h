#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>
#include "Vector.h"

class Button {
private:
	float width;
	float height;


public:
	Vector pos;
	sf::Text text;
	sf::Font f;

	sf::Texture texture;
	sf::RectangleShape rect;
	std::string spriteLocation;
	Button(float _x, float _y, float _width, float _height, std::string _textureAddress);
	
	void update();
	void applyTexture();//Use this one when only using sprite.
	void applyTexture(sf::Font* font);//use this one when referencing the main font. gives a pointer to the main font
	bool isClicked(float _mouseX, float mouseY);
};
